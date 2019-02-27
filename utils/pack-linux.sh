#!/bin/bash

set -e

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

NAME="$1"

rm -rf "$NAME-linux32bit"
rm -rf "$NAME-linux64bit"

make clean
CFLAGS="-m32" CXXFLAGS="-m32" LDFLAGS="-m32" make HAVE_JACK=false HAVE_PROJM=false -j 8
mkdir -p "$NAME-linux32bit/ladspa"
mkdir -p "$NAME-linux32bit/dssi"
mkdir -p "$NAME-linux32bit/lv2"
mkdir -p "$NAME-linux32bit/vst"
mv bin/*-ladspa.so "$NAME-linux32bit/ladspa"
mv bin/*-dssi bin/*-dssi.so "$NAME-linux32bit/dssi"
mv bin/*.lv2/ "$NAME-linux32bit/lv2"
mv bin/*-vst.so "$NAME-linux32bit/vst"
cp utils/README-Linux.txt "$NAME-linux32bit/README.txt"
compressFolderAsTarGz "$NAME-linux32bit"

make clean
CFLAGS="-m64" CXXFLAGS="-m64" LDFLAGS="-m64" make HAVE_JACK=false HAVE_PROJM=false -j 8
mkdir -p "$NAME-linux64bit/ladspa"
mkdir -p "$NAME-linux64bit/dssi"
mkdir -p "$NAME-linux64bit/lv2"
mkdir -p "$NAME-linux64bit/vst"
mv bin/*-ladspa.so "$NAME-linux64bit/ladspa"
mv bin/*-dssi bin/*-dssi.so "$NAME-linux64bit/dssi"
mv bin/*.lv2/ "$NAME-linux64bit/lv2"
mv bin/*-vst.so "$NAME-linux64bit/vst"
cp utils/README-Linux.txt "$NAME-linux64bit/README.txt"
compressFolderAsTarGz "$NAME-linux64bit"

make clean

# --------------------------------------------------------------------------------------------------------------------------------
