#!/bin/bash

# ---------------------------------------------------------------------------------------------------------------------
# stop on error

set -e

# ---------------------------------------------------------------------------------------------------------------------
# cd to correct path

cd $(dirname $0)

# ---------------------------------------------------------------------------------------------------------------------
# set variables

source common.env

# ---------------------------------------------------------------------------------------------------------------------
# function to remove old stuff

cleanup_prefix()
{

rm -rf ${TARGETDIR}/ninjas2-w32nosse ${TARGETDIR}/ninjas2-w32 ${TARGETDIR}/ninsj2-w64

}

cleanup_pkgs()
 {
   cd ${TARGETDIR}/libsamplerate-master
make clean
cd ..
cd libsndfile-${LIBSNDFILE_VERSION}
make clean
cd ..
cd flac-${FLAC_VERSION}
make clean
cd ..
cd libvorbis-${LIBVORBIS_VERSION}
make clean
cd ..
cd libogg-${LIBOGG_VERSION}
make clean
cd ..
}

cleanup()
{

 cleanup_prefix
 cleanup_pkgs
 exit 0

}

# ------------------------------------------------------------------------------------
# function to build base libs

build_base()
{
# ---------------------------------------------------------------------------------------------------------------------
# clean env

unset AR
unset CC
unset CXX
unset STRIP
unset WINDRES

unset PKG_CONFIG_PATH

unset CFLAGS
unset CPPFLAGS
unset CXXFLAGS
unset LDFLAGS

export PREFIX=${TARGETDIR}/ninjas2-w${ARCH_PREFIX}
export PATH=${PREFIX}/bin:/usr/sbin:/usr/bin:/sbin:/bin
export PKG_CONFIG_PATH=${PREFIX}/lib/pkgconfig

# ---------------------------------------------------------------------------------------------------------------------
# setup

if [ x"${ARCH}" != x"32" ]; then
  CPUARCH="x86_64"
else
  CPUARCH="i686"
fi

HOST_ARCH=$(dpkg-architecture -qDEB_BUILD_GNU_TYPE)
MINGW_PREFIX="${CPUARCH}-w64-mingw32"

export AR=${MINGW_PREFIX}-ar
export CC=${MINGW_PREFIX}-gcc
export CXX=${MINGW_PREFIX}-g++
export STRIP=${MINGW_PREFIX}-strip
export WINDRES=${MINGW_PREFIX}-windres

export PATH=/opt/mingw${ARCH}/${MINGW_PREFIX}/bin:/opt/mingw${ARCH}/bin:${PATH}

if [ -z "${NOSSE}" ]; then
export CFLAGS="-O3 -mtune=generic -msse -msse2 -mfpmath=sse -fvisibility=hidden -fdata-sections -ffunction-sections"
else
export CFLAGS="-O2 -fvisibility=hidden -fdata-sections -ffunction-sections"
fi

export CFLAGS="${CFLAGS} -DNDEBUG -DPTW32_STATIC_LIB -I${PREFIX}/include -I/opt/mingw${ARCH}/include"
export CXXFLAGS="${CFLAGS} -fvisibility-inlines-hidden"
export CPPFLAGS="-DPIC -DNDEBUG -DPTW32_STATIC_LIB -I${PREFIX}/include -I/opt/mingw${ARCH}/include"

export LDFLAGS="-fdata-sections -ffunction-sections -Wl,--gc-sections -Wl,-O1 -Wl,--as-needed -Wl,--strip-all"
export LDFLAGS="${LDFLAGS} -L${PREFIX}/lib -L/opt/mingw${ARCH}/lib"

# ---------------------------------------------------------------------------------------------------------------------
# libogg
 cd libogg-${LIBOGG_VERSION}
  ./configure --enable-static --disable-shared --prefix=${PREFIX} \
    --target=${MINGW_PREFIX} --host=${MINGW_PREFIX} --build=${HOST_ARCH}
  make ${MAKE_ARGS}
  make install
  cd ..
# ---------------------------------------------------------------------------------------------------------------------
# libvorbis
  cd libvorbis-${LIBVORBIS_VERSION}
  ./configure --enable-static --disable-shared --prefix=${PREFIX} \
    --target=${MINGW_PREFIX} --host=${MINGW_PREFIX} --build=${HOST_ARCH}
  make ${MAKE_ARGS}
  make install
  cd ..
# ---------------------------------------------------------------------------------------------------------------------
# flac
  cd flac-${FLAC_VERSION}
  chmod +x configure install-sh
  ./configure --enable-static --disable-shared --prefix=${PREFIX} \
    --target=${MINGW_PREFIX} --host=${MINGW_PREFIX} --build=${HOST_ARCH} \
    --disable-cpplibs
  make ${MAKE_ARGS}
  make install
  cd ..

# ---------------------------------------------------------------------------------------------------------------------
# libsndfile
  cd libsndfile-${LIBSNDFILE_VERSION}
  ./configure --enable-static --disable-shared --prefix=${PREFIX} \
    --target=${MINGW_PREFIX} --host=${MINGW_PREFIX} --build=${HOST_ARCH} \
    --disable-full-suite --disable-alsa --disable-sqlite
  make ${MAKE_ARGS}
  make install
  cd ..
# ---------------------------------------------------------------------------------------------------------------------
# libsamplerate
cd libsamplerate-master
./configure --enable-static --disable-shared --prefix=${PREFIX} \
--target=${MINGW_PREFIX} --host=${MINGW_PREFIX} --build=${HOST_ARCH} \
--disable-alsa
make ${MAKE_ARGS}
make install

}

# ------------------------------------------------------------------------------------
# build base libs

cleanup_prefix
cleanup_pkgs

#export ARCH=32
#export ARCH_PREFIX=32nosse
#export NOSSE=1
#build_base
#cleanup_pkgs

#unset NOSSE

#export ARCH=32
#export ARCH_PREFIX=32
#build_base
#cleanup_pkgs

export ARCH=64
export ARCH_PREFIX=64
build_base
#export WIN32=true
#make clean
#make ${MAKE_ARGS}
cleanup_pkgs

# ---------------------------------------------------------------------------------------------------------------------
