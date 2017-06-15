#!/bin/bash

set -e

MODGUIS=("Kars" "MVerb" "MVerb" "Nekobi" "PingPongPan")

# --------------------------------------------------------------------------------------------------------------------------------
# extract debs and pack them

function compressFolderAsTarGz() {
rm -f "$1.tar.gz"
tar cf "$1.tar.gz" "$1"
rm -r "$1"
}

# --------------------------------------------------------------------------------------------------------------------------------

if [ "$1" == "" ]; then
echo Missing argument
exit
fi

# --------------------------------------------------------------------------------------------------------------------------------

cd bin

mkdir -p tmp
rm -rf tmp/*

NAME="$1"

make -C .. clean
CFLAGS="-m32" CXXFLAGS="-m32" LDFLAGS="-m32" make HAVE_JACK=false HAVE_PROJM=false -C .. -j 8
mkdir -p "$NAME-linux32bit/ladspa"
mkdir -p "$NAME-linux32bit/dssi"
mkdir -p "$NAME-linux32bit/lv2"
mkdir -p "$NAME-linux32bit/vst"
mv *-ladspa.so "$NAME-linux32bit/ladspa"
mv *-dssi *-dssi.so "$NAME-linux32bit/dssi"
mv *.lv2/ "$NAME-linux32bit/lv2"
mv *-vst.so "$NAME-linux32bit/vst"
for MODGUI in ${MODGUIS[@]}; do
  cp -r ../modguis/$MODGUI.modgui/modgui "$NAME-linux32bit"/lv2/$MODGUI.lv2/
  cp ../modguis/$MODGUI.modgui/manifest.ttl "$NAME-linux32bit"/lv2/$MODGUI.lv2/modgui.ttl
done
cp "../utils/README-Linux.txt" "$NAME-linux32bit/README.txt"
compressFolderAsTarGz "$NAME-linux32bit"
rm -rf tmp/*

make -C .. clean
CFLAGS="-m64" CXXFLAGS="-m64" LDFLAGS="-m64" make HAVE_JACK=false HAVE_PROJM=false -C .. -j 8
mkdir -p "$NAME-linux64bit/ladspa"
mkdir -p "$NAME-linux64bit/dssi"
mkdir -p "$NAME-linux64bit/lv2"
mkdir -p "$NAME-linux64bit/vst"
mv *-ladspa.so "$NAME-linux64bit/ladspa"
mv *-dssi *-dssi.so "$NAME-linux64bit/dssi"
mv *.lv2/ "$NAME-linux64bit/lv2"
mv *-vst.so "$NAME-linux64bit/vst"
for MODGUI in ${MODGUIS[@]}; do
  cp -rv ../modguis/$MODGUI.modgui/modgui "$NAME-linux64bit"/lv2/$MODGUI.lv2/
  cp -v ../modguis/$MODGUI.modgui/manifest.ttl "$NAME-linux64bit"/lv2/$MODGUI.lv2/modgui.ttl
done
cp "../utils/README-Linux.txt" "$NAME-linux64bit/README.txt"
compressFolderAsTarGz "$NAME-linux64bit"
rm -rf tmp/*

make -C .. clean

cd ..

# --------------------------------------------------------------------------------------------------------------------------------
