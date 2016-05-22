#!/bin/bash

set -e

MODGUIS=("Kars" "MVerb" "MVerb" "Nekobi" "PingPongPan")

# --------------------------------------------------------------------------------------------------------------------------------
# extract debs and pack them

function compressFolderAsZip() {
rm -f "$1.zip"
zip -X -r "$1" "$1"
rm -r "$1"
}

# --------------------------------------------------------------------------------------------------------------------------------

if [ "$1" == "" ]; then
echo Missing argument
exit
fi

sed -i "s|-MD -MP|-D_MD_MP_WORKAROUND|" */*.mk dpf/dgl/*.mk

# --------------------------------------------------------------------------------------------------------------------------------

cd bin

mkdir -p tmp
rm -rf tmp/*

NAME="$1"

export CFLAGS="-mmacosx-version-min=10.5 -DMAC_OS_X_VERSION_MAX_ALLOWED=1050 -arch i386 -arch x86_64"
export CXXFLAGS=$CFLAGS
export LDFLAGS=$CFLAGS
export MACOS="true"
export MACOS_OLD="true"

make -C .. clean
make HAVE_JACK=false HAVE_LIBLO=false HAVE_PROJM=false -C .. -j 4
rm -rf *ladspa* *dssi*
mkdir -p "$NAME-macos"
mv *.lv2 *.vst "$NAME-macos"
for MODGUI in ${MODGUIS[@]}; do
  cp -r ../modguis/$MODGUI.modgui/modgui "$NAME-macos"/$MODGUI.lv2/
  cp ../modguis/$MODGUI.modgui/manifest.ttl "$NAME-macos"/$MODGUI.lv2/modgui.ttl
done
cp "../utils/README-MacOS.txt" "$NAME-macos/README.txt"
compressFolderAsZip "$NAME-macos"
rm -rf tmp/*

make -C .. clean

cd ..

# --------------------------------------------------------------------------------------------------------------------------------

sed -i -e "s|-D_MD_MP_WORKAROUND|-MD -MP|" */*.mk dpf/dgl/*.mk
