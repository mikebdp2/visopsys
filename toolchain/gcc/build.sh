#!/bin/sh
##
##  Visopsys
##  Copyright (C) 1998-2023 J. Andrew McLaughlin
##
##  build.sh
##

# Builds the toolchain gcc for the speficied platform
#
# https://wiki.osdev.org/GCC_Cross-Compiler
#
# requires: libgmp3-dev libmpfr-dev libmpc-dev

ROOT=$(realpath ../..)
ARCH="$1"

if [ -z "$ARCH" ] ; then
	echo "Target architecture not specified"
	echo "  example: x86"
	exit 1
fi

case "$ARCH" in
	"x86")
		TARGET=i686-elf
		;;

	*)
		echo "Invalid target architecture specified"
		echo "  example: x86"
		exit 1
esac

export PREFIX="$ROOT/toolchain/$ARCH"
export TARGET
export PATH="$PREFIX/bin:$PATH"

# The $PREFIX/bin dir must be in the PATH.  We did that above.
if [ ! `which "$TARGET"-as` ] ; then
	echo "$TARGET"-as is not in the PATH
	exit 1
fi

mkdir -p build
chmod -R u+w build
rm -Rf build/*
cd build

chmod +x ../src/configure
../src/configure --target="$TARGET" --prefix="$PREFIX" --disable-nls \
	--enable-languages=c,c++ --without-headers --disable-werror \
	--disable-shared --disable-gcov --disable-multilib --disable-tls \
	--disable-tm-clone-registry --disable-libsanitizer --disable-libssp \
	--disable-libgomp --disable-libvtv --disable-libquadmath --disable-cet \
	--with-as="$PREFIX/bin/$TARGET"-as

make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc

chmod +x "$PREFIX/bin/$TARGET"-*
cd .. ; chmod -R u+w build ; rm -Rf build

