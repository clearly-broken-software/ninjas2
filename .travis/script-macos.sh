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

export PKG_CONFIG="pkg-config --static"
export PKG_CONFIG_PATH="/opt/local/lib/pkgconfig"

export PATH=/opt/local/bin:"$PATH"

if [ -z "$NOSSE" ]; then
export CFLAGS="-O3 -mtune=generic -msse -msse2 -mfpmath=sse -fvisibility=hidden -fdata-sections -ffunction-sections"
else
export CFLAGS="-O2 -fvisibility=hidden -fdata-sections -ffunction-sections"
fi

export CFLAGS="$CFLAGS -DNDEBUG -I/opt/local/include"
export CXXFLAGS="$CFLAGS -fvisibility-inlines-hidden"
export CPPFLAGS="-DPIC -DNDEBUG -I/opt/local/include"

export LDFLAGS="-fdata-sections -ffunction-sections -Wl,-dead_strip -Wl,-dead_strip_dylibs"
export LDFLAGS="$LDFLAGS -L/opt/local/lib"

# ---------------------------------------------------------------------------------------------------------------------
# build the dependencies

build_auto_dep() {
    cd "$1"
    shift
    ./configure --enable-static --disable-shared --prefix=/opt/local "$@"
    make $MAKE_ARGS
    sudo make install
    cd ..
}

# extract
tar xzvf download/libogg-"$LIBOGG_VERSION".tar.gz
tar xzvf download/libvorbis-"$LIBVORBIS_VERSION".tar.gz
tar xJvf download/flac-"$FLAC_VERSION".tar.xz
tar xzvf download/libsamplerate-"$LIBSAMPLERATE_VERSION".tar.gz
tar xzvf download/libsndfile-"$LIBSNDFILE_VERSION".tar.gz

build_auto_dep libogg-"$LIBOGG_VERSION"
build_auto_dep libvorbis-"$LIBVORBIS_VERSION"
build_auto_dep flac-"$FLAC_VERSION"
build_auto_dep libsamplerate-"$LIBSAMPLERATE_VERSION"
build_auto_dep libsndfile-"$LIBSNDFILE_VERSION"

# sndfile.pc needs fixing...
cat /opt/local/lib/pkgconfig/sndfile.pc | egrep -v '^Libs.private:' | egrep -v '^Requires.private:' > sndfile.pc
echo 'Requires.private: ogg vorbis vorbisenc flac' >> sndfile.pc
sudo mv -f sndfile.pc /opt/local/lib/pkgconfig/sndfile.pc

# ---------------------------------------------------------------------------------------------------------------------
# build the plugin

MACOS=true make $MAKE_ARGS
