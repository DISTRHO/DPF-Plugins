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

# --------------------------------------------------------------------------------------------------------------------------------

cd bin

mkdir -p tmp
rm -rf tmp/*

NAME="$1"

_mingw32-build make -C .. clean
_mingw32-build make -C ..
for i in `ls *-vst.dll`; do mv $i `echo $i | awk 'sub("-vst","")'`; done
rm -rf *ladspa* *dssi* *vst*
mkdir -p "$NAME-win32bit"
mv *.dll *.lv2/ "$NAME-win32bit"
for MODGUI in ${MODGUIS[@]}; do
  cp -r ../modguis/$MODGUI.modgui/modgui "$NAME-win32bit"/$MODGUI.lv2/
  cp ../modguis/$MODGUI.modgui/manifest.ttl "$NAME-win32bit"/$MODGUI.lv2/modgui.ttl
done
cp "../utils/README-Windows.txt" "$NAME-win32bit/README.txt"
compressFolderAsZip "$NAME-win32bit"
rm -rf tmp/*

_mingw64-build make -C .. clean
_mingw64-build make -C .. -j4
for i in `ls *-vst.dll`; do mv $i `echo $i | awk 'sub("-vst","")'`; done
rm -rf *ladspa* *dssi* *vst*
mkdir -p "$NAME-win64bit"
mv *.dll *.lv2/ "$NAME-win64bit"
for MODGUI in ${MODGUIS[@]}; do
  cp -rv ../modguis/$MODGUI.modgui/modgui "$NAME-win64bit"/$MODGUI.lv2/
  cp -v ../modguis/$MODGUI.modgui/manifest.ttl "$NAME-win64bit"/$MODGUI.lv2/modgui.ttl
done
cp "../utils/README-Windows.txt" "$NAME-win64bit/README.txt"
compressFolderAsZip "$NAME-win64bit"
rm -rf tmp/*

_mingw64-build make -C .. clean

cd ..

# --------------------------------------------------------------------------------------------------------------------------------
