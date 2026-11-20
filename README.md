# gameslate-blip.github.io (the main website, domain root)

This repo serves https://gameslate-blip.github.io/ : the Digital Slate Database home page with the app chooser, the app pages and the legal pages, in English and Portuguese (`/pt/`).

**Do not edit the HTML here.** It is generated from the repo gameslate-blip/digitalslate (`tools/build_site.py`, `content/`, `assets/`):

    cd ~/Projects/digitalslate
    python3 tools/build_site.py --root-site --out ../gameslate-blip.github.io   # this repo
    python3 tools/build_site.py                                                  # the /digitalslate/ copy

Then commit and push both repos. The copy at https://gameslate-blip.github.io/digitalslate/ must stay: Google Play listings and the apps link to its privacy pages.

`app-ads.txt` is the AdMob authorized sellers file and must stay at the domain root (it is not generated).
