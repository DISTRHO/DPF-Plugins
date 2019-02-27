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

sed -i -e "s|-MD -MP|-D_MD_MP_WORKAROUND|" */*.mk

# --------------------------------------------------------------------------------------------------------------------------------

NAME="$1"

rm -rf "$NAME-macOS"

export CFLAGS="-mmacosx-version-min=10.5 -DMAC_OS_X_VERSION_MAX_ALLOWED=1060 -arch i386 -arch x86_64 -mfpmath=sse"
export CXXFLAGS="$CFLAGS"
export LDFLAGS="$CFLAGS"
export MACOS="true"
export MACOS_OLD="true"

make clean
make HAVE_JACK=false HAVE_LIBLO=false HAVE_PROJM=false -j 2
rm -rf bin/*ladspa* bin/*dssi*
mkdir -p "$NAME-macOS"
mv bin/*.lv2 bin/*.vst "$NAME-macOS"
cp utils/README-MacOS.txt "$NAME-macOS/README.txt"
compressFolderAsZip "$NAME-macOS"

make clean

# --------------------------------------------------------------------------------------------------------------------------------

sed -i -e "s|-D_MD_MP_WORKAROUND|-MD -MP|" */*.mk
