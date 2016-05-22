#!/bin/bash

set -e

PLUGINS=("glBars" "Kars" "Mini-Series" "MVerb" "Nekobi" "ProM" "ndc-Plugs" "DPF-Max-Gen")

if [ ! -f Makefile ]; then
  echo "Makefile not found, please run this script from DPF-Plugins root source dir"
  exit
fi

rm -rf repos
mkdir repos
cd repos

git clone --depth 1 git://github.com/DISTRHO/DPF

for PLUGIN in ${PLUGINS[@]}; do
  git clone --depth 1 git://github.com/DISTRHO/$PLUGIN
done

cd ..

rm -rf dpf
rm -rf modguis
rm -rf plugins
mkdir modguis
mkdir plugins

mv repos/DPF dpf
rm -rf dpf/.git*

for PLUGIN in ${PLUGINS[@]}; do
  if [ -d repos/$PLUGIN/modguis ]; then
    mv repos/$PLUGIN/modguis/* modguis/
  fi
  mv repos/$PLUGIN/plugins/* plugins/
  mv plugins/Makefile.mk plugins/Makefile.$PLUGIN.mk
  sed -i -e "s/Makefile.mk/Makefile.$PLUGIN.mk/" plugins/*/Makefile
done

rm -rf repos
