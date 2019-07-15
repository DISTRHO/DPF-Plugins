#!/bin/bash

set -e

# --------------------------------------------------------------------------------------------------------------------------------
# extract debs and pack them

function compressFolderAsZip() {
rm -f "$1.zip"
zip -X -r "$1.zip" "$1"
rm -r "$1"
}

# --------------------------------------------------------------------------------------------------------------------------------

if [ "$1" == "" ]; then
echo Missing argument
exit
fi

# --------------------------------------------------------------------------------------------------------------------------------

NAME="$1"

rm -rf "$NAME-win32bit"
rm -rf "$NAME-win64bit"

_mingw32-build make clean
_mingw32-build make HAVE_JACK=false HAVE_LIBLO=false HAVE_PROJM=false HAVE_CAIRO=false WINDOWS=true -j8
for i in `ls bin/*-vst.dll`; do mv $i `echo $i | awk 'sub("-vst","")'`; done
rm -rf bin/*ladspa* bin/*dssi* bin/*vst*
mkdir -p "$NAME-win32bit"
mv bin/*.dll bin/*.lv2/ "$NAME-win32bit"
cp utils/README-Windows.txt "$NAME-win32bit/README.txt"
compressFolderAsZip "$NAME-win32bit"

_mingw64-build make clean
_mingw64-build make HAVE_JACK=false HAVE_LIBLO=false HAVE_PROJM=false HAVE_CAIRO=false WINDOWS=true -j8
for i in `ls bin/*-vst.dll`; do mv $i `echo $i | awk 'sub("-vst","")'`; done
rm -rf bin/*ladspa* bin/*dssi* bin/*vst*
mkdir -p "$NAME-win64bit"
mv bin/*.dll bin/*.lv2/ "$NAME-win64bit"
cp utils/README-Windows.txt "$NAME-win64bit/README.txt"
compressFolderAsZip "$NAME-win64bit"

_mingw64-build make clean

# --------------------------------------------------------------------------------------------------------------------------------
