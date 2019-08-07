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

test ! -z "$_BITS"

# ---------------------------------------------------------------------------------------------------------------------
# setup

export PKG_CONFIG="pkg-config"

case "$_BITS" in
    32)
        export PKG_CONFIG_LIBDIR="/usr/lib/i386-linux-gnu/pkgconfig:/usr/lib/pkgconfig:/usr/share/pkgconfig"
        ;;
    64)
        export PKG_CONFIG_LIBDIR="/usr/lib/x86_64-linux-gnu/pkgconfig:/usr/lib/pkgconfig:/usr/share/pkgconfig"
        ;;
esac

if [ -z "$NOSSE" ]; then
export CFLAGS="-O3 -mtune=generic -msse -msse2 -mfpmath=sse -fvisibility=hidden -fdata-sections -ffunction-sections"
else
export CFLAGS="-O2 -fvisibility=hidden -fdata-sections -ffunction-sections"
fi

export CFLAGS="$CFLAGS -DNDEBUG"
export CFLAGS="$CFLAGS -m$_BITS"
export CXXFLAGS="$CFLAGS -fvisibility-inlines-hidden"
export CPPFLAGS="-DPIC -DNDEBUG"

export LDFLAGS="-fdata-sections -ffunction-sections -Wl,--gc-sections -Wl,-O1 -Wl,--as-needed -Wl,--strip-all"
export LDFLAGS="$LDFLAGS -m$_BITS"

# ---------------------------------------------------------------------------------------------------------------------
# build the plugin

LINUX=true make $MAKE_ARGS
