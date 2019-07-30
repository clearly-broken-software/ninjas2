#!/bin/sh

# Copyright (C) 2008-2016 Erik de Castro Lopo <erikd@mega-nerd.com>
#
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in
#       the documentation and/or other materials provided with the
#       distribution.
#     * Neither the author nor the names of any contributors may be used
#       to endorse or promote products derived from this software without
#       specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
# TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


HOST_TRIPLET=x86_64-w64-mingw32
PACKAGE_VERSION=1.0.28
LIB_VERSION=$(echo $PACKAGE_VERSION | sed "s/[a-z].*//")

if test -f tests/sfversion.exe ; then
	cd tests
	fi

if test ! -f sfversion.exe ; then
	echo "Not able to find test executables."
	exit 1
	fi

if test -f libsndfile.so.$LIB_VERSION ; then
	# This will work on Linux, but not on Mac.
	# Windows is already sorted out.
	export LD_LIBRARY_PATH=`pwd`
	if test ! -f libsndfile.so.1 ; then
		ln -s libsndfile.so.$LIB_VERSION libsndfile.so.1
		fi
	fi

sfversion=$(./sfversion.exe | grep libsndfile | sed "s/-exp$//")

if test "$sfversion" != libsndfile-$PACKAGE_VERSION ; then
	echo "Error : sfversion ($sfversion) and PACKAGE_VERSION ($PACKAGE_VERSION) don't match."
	exit 1
	fi

# Force exit on errors.
set -e

# Generic-tests
uname -a

# Check the header file.
sh pedantic-header-test.sh

# Need this for when we're running from files collected into the
# libsndfile-testsuite-1.0.28 tarball.
if test -x test_main.exe ; then
	echo "Running unit tests from src/ directory of source code tree."
	./test_main.exe
	echo
	echo "Running end-to-end tests from tests/ directory."
	fi

./error_test.exe
./pcm_test.exe
./ulaw_test.exe
./alaw_test.exe
./dwvw_test.exe
./command_test.exe ver
./command_test.exe norm
./command_test.exe format
./command_test.exe peak
./command_test.exe trunc
./command_test.exe inst
./command_test.exe cue
./command_test.exe current_sf_info
./command_test.exe bext
./command_test.exe bextch
./command_test.exe chanmap
./command_test.exe cart
./floating_point_test.exe
./checksum_test.exe
./scale_clip_test.exe
./headerless_test.exe
./rdwr_test.exe
./locale_test.exe
./win32_ordinal_test.exe
./external_libs_test.exe
./format_check_test.exe
./channel_test.exe

# The w64 G++ compiler requires an extra runtime DLL which we don't have,
# so skip this test.
case "$HOST_TRIPLET" in
	x86_64-w64-mingw32)
		;;
	i686-w64-mingw32)
		;;
	*)
		./cpp_test.exe
		;;
	esac

echo "----------------------------------------------------------------------"
echo "  $sfversion passed common tests."
echo "----------------------------------------------------------------------"

# aiff-tests
./write_read_test.exe aiff
./lossy_comp_test.exe aiff_ulaw
./lossy_comp_test.exe aiff_alaw
./lossy_comp_test.exe aiff_gsm610
echo "=========================="
echo "./lossy_comp_test.exe aiff_ima"
echo "=========================="
./peak_chunk_test.exe aiff
./header_test.exe aiff
./misc_test.exe aiff
./string_test.exe aiff
./multi_file_test.exe aiff
./aiff_rw_test.exe
./chunk_test.exe aiff
echo "----------------------------------------------------------------------"
echo "  $sfversion passed tests on AIFF files."
echo "----------------------------------------------------------------------"

# au-tests
./write_read_test.exe au
./lossy_comp_test.exe au_ulaw
./lossy_comp_test.exe au_alaw
./lossy_comp_test.exe au_g721
./lossy_comp_test.exe au_g723
./header_test.exe au
./misc_test.exe au
./multi_file_test.exe au
echo "----------------------------------------------------------------------"
echo "  $sfversion passed tests on AU files."
echo "----------------------------------------------------------------------"

# caf-tests
./write_read_test.exe caf
./lossy_comp_test.exe caf_ulaw
./lossy_comp_test.exe caf_alaw
./header_test.exe caf
./peak_chunk_test.exe caf
./misc_test.exe caf
./chunk_test.exe caf
./string_test.exe caf
./long_read_write_test.exe alac
echo "----------------------------------------------------------------------"
echo "  $sfversion passed tests on CAF files."
echo "----------------------------------------------------------------------"

# wav-tests
./write_read_test.exe wav
./lossy_comp_test.exe wav_pcm
./lossy_comp_test.exe wav_ima
./lossy_comp_test.exe wav_msadpcm
./lossy_comp_test.exe wav_ulaw
./lossy_comp_test.exe wav_alaw
./lossy_comp_test.exe wav_gsm610
./lossy_comp_test.exe wav_g721
./peak_chunk_test.exe wav
./header_test.exe wav
./misc_test.exe wav
./string_test.exe wav
./multi_file_test.exe wav
./chunk_test.exe wav
echo "----------------------------------------------------------------------"
echo "  $sfversion passed tests on WAV files."
echo "----------------------------------------------------------------------"

# w64-tests
./write_read_test.exe w64
./lossy_comp_test.exe w64_ima
./lossy_comp_test.exe w64_msadpcm
./lossy_comp_test.exe w64_ulaw
./lossy_comp_test.exe w64_alaw
./lossy_comp_test.exe w64_gsm610
./header_test.exe w64
./misc_test.exe w64
echo "----------------------------------------------------------------------"
echo "  $sfversion passed tests on W64 files."
echo "----------------------------------------------------------------------"

# rf64-tests
./write_read_test.exe rf64
./header_test.exe rf64
./misc_test.exe rf64
./string_test.exe rf64
./peak_chunk_test.exe rf64
./chunk_test.exe rf64
echo "----------------------------------------------------------------------"
echo "  $sfversion passed tests on RF64 files."
echo "----------------------------------------------------------------------"

# raw-tests
./write_read_test.exe raw
./lossy_comp_test.exe raw_ulaw
./lossy_comp_test.exe raw_alaw
./lossy_comp_test.exe raw_gsm610
./lossy_comp_test.exe vox_adpcm
./raw_test.exe
echo "----------------------------------------------------------------------"
echo "  $sfversion passed tests on RAW (header-less) files."
echo "----------------------------------------------------------------------"

# paf-tests
./write_read_test.exe paf
./header_test.exe paf
./misc_test.exe paf
echo "----------------------------------------------------------------------"
echo "  $sfversion passed tests on PAF files."
echo "----------------------------------------------------------------------"

# svx-tests
./write_read_test.exe svx
./header_test.exe svx
./misc_test.exe svx
echo "----------------------------------------------------------------------"
echo "  $sfversion passed tests on SVX files."
echo "----------------------------------------------------------------------"

# nist-tests
./write_read_test.exe nist
./lossy_comp_test.exe nist_ulaw
./lossy_comp_test.exe nist_alaw
./header_test.exe nist
./misc_test.exe nist
echo "----------------------------------------------------------------------"
echo "  $sfversion passed tests on NIST files."
echo "----------------------------------------------------------------------"

# ircam-tests
./write_read_test.exe ircam
./lossy_comp_test.exe ircam_ulaw
./lossy_comp_test.exe ircam_alaw
./header_test.exe ircam
./misc_test.exe ircam
echo "----------------------------------------------------------------------"
echo "  $sfversion passed tests on IRCAM files."
echo "----------------------------------------------------------------------"

# voc-tests
./write_read_test.exe voc
./lossy_comp_test.exe voc_ulaw
./lossy_comp_test.exe voc_alaw
./header_test.exe voc
./misc_test.exe voc
echo "----------------------------------------------------------------------"
echo "  $sfversion passed tests on VOC files."
echo "----------------------------------------------------------------------"

# mat4-tests
./write_read_test.exe mat4
./header_test.exe mat4
./misc_test.exe mat4
echo "----------------------------------------------------------------------"
echo "  $sfversion passed tests on MAT4 files."
echo "----------------------------------------------------------------------"

# mat5-tests
./write_read_test.exe mat5
./header_test.exe mat5
./misc_test.exe mat5
echo "----------------------------------------------------------------------"
echo "  $sfversion passed tests on MAT5 files."
echo "----------------------------------------------------------------------"

# pvf-tests
./write_read_test.exe pvf
./header_test.exe pvf
./misc_test.exe pvf
echo "----------------------------------------------------------------------"
echo "  $sfversion passed tests on PVF files."
echo "----------------------------------------------------------------------"

# xi-tests
./lossy_comp_test.exe xi_dpcm
echo "----------------------------------------------------------------------"
echo "  $sfversion passed tests on XI files."
echo "----------------------------------------------------------------------"

# htk-tests
./write_read_test.exe htk
./header_test.exe htk
./misc_test.exe htk
echo "----------------------------------------------------------------------"
echo "  $sfversion passed tests on HTK files."
echo "----------------------------------------------------------------------"

# avr-tests
./write_read_test.exe avr
./header_test.exe avr
./misc_test.exe avr
echo "----------------------------------------------------------------------"
echo "  $sfversion passed tests on AVR files."
echo "----------------------------------------------------------------------"

# sds-tests
./write_read_test.exe sds
./header_test.exe sds
./misc_test.exe sds
echo "----------------------------------------------------------------------"
echo "  $sfversion passed tests on SDS files."
echo "----------------------------------------------------------------------"

# sd2-tests
./write_read_test.exe sd2
echo "----------------------------------------------------------------------"
echo "  $sfversion passed tests on SD2 files."
echo "----------------------------------------------------------------------"

# wve-tests
./lossy_comp_test.exe wve
echo "----------------------------------------------------------------------"
echo "  $sfversion passed tests on WVE files."
echo "----------------------------------------------------------------------"

# mpc2k-tests
./write_read_test.exe mpc2k
./header_test.exe mpc2k
./misc_test.exe mpc2k
echo "----------------------------------------------------------------------"
echo "  $sfversion passed tests on MPC 2000 files."
echo "----------------------------------------------------------------------"

# flac-tests
./write_read_test.exe flac
./compression_size_test.exe flac
./string_test.exe flac
echo "----------------------------------------------------------------------"
echo "  $sfversion passed tests on FLAC files."
echo "----------------------------------------------------------------------"

# vorbis-tests
./ogg_test.exe
./compression_size_test.exe vorbis
./lossy_comp_test.exe ogg_vorbis
./string_test.exe ogg
./misc_test.exe ogg
echo "----------------------------------------------------------------------"
echo "  $sfversion passed tests on OGG/VORBIS files."
echo "----------------------------------------------------------------------"

# io-tests
./stdio_test.exe
./pipe_test.exe
./virtual_io_test.exe
echo "----------------------------------------------------------------------"
echo "  $sfversion passed stdio/pipe/vio tests."
echo "----------------------------------------------------------------------"


