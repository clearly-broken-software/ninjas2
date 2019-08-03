#!/bin/bash

# ---------------------------------------------------------------------------------------------------------------------
# stop on error

set -e

# ---------------------------------------------------------------------------------------------------------------------
# print shell commands

set -x

# ---------------------------------------------------------------------------------------------------------------------
# set variables

source common.env

test ! -z "$_MINGW"
test ! -z "$_HOST"

# ---------------------------------------------------------------------------------------------------------------------
# download sources in the cache directory

install -d download

wget -c -O {download/,http://downloads.xiph.org/releases/ogg/}libogg-"$LIBOGG_VERSION".tar.gz
wget -c -O {download/,http://downloads.xiph.org/releases/vorbis/}libvorbis-"$LIBVORBIS_VERSION".tar.gz
wget -c -O {download/,https://ftp.osuosl.org/pub/xiph/releases/flac/}flac-"$FLAC_VERSION".tar.xz
wget -c -O {download/,http://www.mega-nerd.com/SRC/}libsamplerate-"$LIBSAMPLERATE_VERSION".tar.gz
wget -c -O {download/,http://www.mega-nerd.com/libsndfile/files/}libsndfile-"$LIBSNDFILE_VERSION".tar.gz

# ---------------------------------------------------------------------------------------------------------------------
# setup

export AR="$_HOST"-ar
export RANLIB="$_HOST"-ranlib
export CC="$_HOST"-gcc
export CXX="$_HOST"-g++
export STRIP="$_HOST"-strip
export WINDRES="$_HOST"-windres
export PKG_CONFIG="$_HOST"-pkg-config

export PKG_CONFIG="$PKG_CONFIG --static"

export PATH=/opt/"$_MINGW"/"$_HOST"/bin:/opt/"$_MINGW"/bin:"$PATH"

if [ -z "$NOSSE" ]; then
export CFLAGS="-O3 -mtune=generic -msse -msse2 -mfpmath=sse -fvisibility=hidden -fdata-sections -ffunction-sections"
else
export CFLAGS="-O2 -fvisibility=hidden -fdata-sections -ffunction-sections"
fi

export CFLAGS="$CFLAGS -DNDEBUG -DPTW32_STATIC_LIB -I/opt/$_MINGW/include"
export CXXFLAGS="$CFLAGS -fvisibility-inlines-hidden"
export CPPFLAGS="-DPIC -DNDEBUG -DPTW32_STATIC_LIB -I/opt/$_MINGW/include"

export LDFLAGS="-fdata-sections -ffunction-sections -Wl,--gc-sections -Wl,-O1 -Wl,--as-needed -Wl,--strip-all"
export LDFLAGS="$LDFLAGS -L/opt/$_MINGW/lib"

# ---------------------------------------------------------------------------------------------------------------------
# build the dependencies

build_auto_dep() {
    cd "$1"
    shift
    ./configure --enable-static --disable-shared --prefix=/opt/"$_MINGW" --host="$_HOST" "$@"
    make $MAKE_ARGS
    make install DESTDIR="`pwd`/install-target"
    sudo cp -drfv install-target/* /
    rm -rf install-target
    cd ..
}

# extract
tar xzvf download/libogg-"$LIBOGG_VERSION".tar.gz
tar xzvf download/libvorbis-"$LIBVORBIS_VERSION".tar.gz
tar xJvf download/flac-"$FLAC_VERSION".tar.xz
tar xzvf download/libsamplerate-"$LIBSAMPLERATE_VERSION".tar.gz
tar xzvf download/libsndfile-"$LIBSNDFILE_VERSION".tar.gz

# apply needed patches
case "$LIBSAMPLERATE_VERSION" in
    0.1.9)
        wget -c -O download/libsamplerate-fix-windows.diff https://github.com/erikd/libsamplerate/commit/477ce36f8e4bd6a177727f4ac32eba11864dd85d.diff
        patch -d libsamplerate-"$LIBSAMPLERATE_VERSION" -p1 -i ../download/libsamplerate-fix-windows.diff
        ;;
    *)
        ;;
esac

build_auto_dep libogg-"$LIBOGG_VERSION"
build_auto_dep libvorbis-"$LIBVORBIS_VERSION"
build_auto_dep flac-"$FLAC_VERSION"
build_auto_dep libsamplerate-"$LIBSAMPLERATE_VERSION"
build_auto_dep libsndfile-"$LIBSNDFILE_VERSION"

# sndfile.pc needs fixing...
cp /opt/"$_MINGW"/lib/pkgconfig/sndfile.pc .
sed -i '/^Libs.private:/d' sndfile.pc
sed -i '/^Requires.private:/d' sndfile.pc
echo 'Requires.private: ogg vorbis vorbisenc flac' >> sndfile.pc
sudo mv -f sndfile.pc /opt/"$_MINGW"/lib/pkgconfig/sndfile.pc

# ---------------------------------------------------------------------------------------------------------------------
# prepare

# invoke the Windows ttl generator using wine
sed -i 's/"$GEN"/wine "$GEN"/g' dpf/utils/generate-ttl.sh

# ---------------------------------------------------------------------------------------------------------------------
# build the plugin

CROSS_COMPILING=true WINDOWS=true make $MAKE_ARGS
