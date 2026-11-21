// Thin JNI wrapper around FluidSynth: the same synth (and the same soundfont) as the desktop app, so it sounds the same.
#include <jni.h>
#include <android/log.h>
#include <fluidsynth.h>

#define LOG(...) __android_log_print(ANDROID_LOG_INFO, "JamSlateSynth", __VA_ARGS__)

struct Synth { fluid_settings_t* settings; fluid_synth_t* synth; int sfid; };

extern "C" {

JNIEXPORT jlong JNICALL Java_com_digitalslate_slate_synth_FluidSynthSink_nativeCreate(JNIEnv* env, jobject, jstring path, jint sampleRate) {
    auto* s = new Synth();
    s->settings = new_fluid_settings();
    fluid_settings_setnum(s->settings, "synth.sample-rate", sampleRate);
    fluid_settings_setnum(s->settings, "synth.gain", 0.8);
    fluid_settings_setint(s->settings, "synth.reverb.active", 1);
    fluid_settings_setint(s->settings, "synth.chorus.active", 0);
    s->synth = new_fluid_synth(s->settings);
    const char* p = env->GetStringUTFChars(path, nullptr);
    s->sfid = fluid_synth_sfload(s->synth, p, 1);
    env->ReleaseStringUTFChars(path, p);
    if (s->sfid < 0) { LOG("could not load the soundfont"); delete_fluid_synth(s->synth); delete_fluid_settings(s->settings); delete s; return 0; }
    return reinterpret_cast<jlong>(s);
}

JNIEXPORT void JNICALL Java_com_digitalslate_slate_synth_FluidSynthSink_nativeDestroy(JNIEnv*, jobject, jlong h) {
    auto* s = reinterpret_cast<Synth*>(h); if (!s) return;
    delete_fluid_synth(s->synth); delete_fluid_settings(s->settings); delete s;
}

JNIEXPORT void JNICALL Java_com_digitalslate_slate_synth_FluidSynthSink_nativeReset(JNIEnv*, jobject, jlong h) { fluid_synth_system_reset(reinterpret_cast<Synth*>(h)->synth); }

JNIEXPORT void JNICALL Java_com_digitalslate_slate_synth_FluidSynthSink_nativeProgramSelect(JNIEnv*, jobject, jlong h, jint ch, jint bank, jint prog) {
    auto* s = reinterpret_cast<Synth*>(h); fluid_synth_program_select(s->synth, ch, s->sfid, bank, prog);
}
JNIEXPORT void JNICALL Java_com_digitalslate_slate_synth_FluidSynthSink_nativeNoteOn(JNIEnv*, jobject, jlong h, jint ch, jint key, jint vel) { fluid_synth_noteon(reinterpret_cast<Synth*>(h)->synth, ch, key, vel); }
JNIEXPORT void JNICALL Java_com_digitalslate_slate_synth_FluidSynthSink_nativeNoteOff(JNIEnv*, jobject, jlong h, jint ch, jint key) { fluid_synth_noteoff(reinterpret_cast<Synth*>(h)->synth, ch, key); }
JNIEXPORT void JNICALL Java_com_digitalslate_slate_synth_FluidSynthSink_nativeCc(JNIEnv*, jobject, jlong h, jint ch, jint num, jint val) { fluid_synth_cc(reinterpret_cast<Synth*>(h)->synth, ch, num, val); }
JNIEXPORT void JNICALL Java_com_digitalslate_slate_synth_FluidSynthSink_nativeAllNotesOff(JNIEnv*, jobject, jlong h, jint ch) { fluid_synth_all_notes_off(reinterpret_cast<Synth*>(h)->synth, ch); }

// Renders `frames` stereo frames (16-bit, interleaved) into out[at*2 ...].
JNIEXPORT void JNICALL Java_com_digitalslate_slate_synth_FluidSynthSink_nativeRender(JNIEnv* env, jobject, jlong h, jshortArray out, jint at, jint frames) {
    auto* s = reinterpret_cast<Synth*>(h);
    auto* buf = static_cast<jshort*>(env->GetPrimitiveArrayCritical(out, nullptr));
    fluid_synth_write_s16(s->synth, frames, buf + at * 2, 0, 2, buf + at * 2, 1, 2);
    env->ReleasePrimitiveArrayCritical(out, buf, 0);
}

}
