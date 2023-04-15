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

git clone --depth 1 --recursive -b develop https://github.com/DISTRHO/DPF.git

for PLUGIN in ${PLUGINS[@]}; do
  git clone --depth 1 https://github.com/DISTRHO/${PLUGIN}.git
done

# special case for ProM
cd ProM
git submodule init
git submodule update
cd ..

cd ..

rm -rf dpf
rm -rf modguis
rm -rf plugins
mkdir plugins

mv repos/DPF dpf
rm -rf dpf/.git*
rm -rf dpf/.travis*
rm -rf dpf/dgl/src/pugl-upstream/bindings
rm -rf dpf/dgl/src/pugl-upstream/doc
rm -rf dpf/dgl/src/pugl-upstream/examples
rm -rf dpf/dgl/src/pugl-upstream/meson*
rm -rf dpf/dgl/src/pugl-upstream/resources
rm -rf dpf/dgl/src/pugl-upstream/scripts
rm -rf dpf/dgl/src/pugl-upstream/test
rm -rf dpf/examples
rm -rf dpf/tests
rm -f  dpf/Makefile

for PLUGIN in ${PLUGINS[@]}; do
  for f in $(ls repos/${PLUGIN}/plugins/); do
    mv repos/${PLUGIN}/plugins/${f} plugins/${f}
    if [ -d plugins/${f} ]; then
      cp repos/${PLUGIN}/LICENSE plugins/${f}/LICENSE
    fi
  done
done

rm -rf repos
rm plugins/common/LICENSE
rm plugins/*/CMakeLists.txt
