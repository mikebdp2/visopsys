#!/bin/sh
##
##  Visopsys
##  Copyright (C) 1998-2023 J. Andrew McLaughlin
##
##  build.sh
##

# Builds the toolchain binutils for the speficied platform
#
# https://wiki.osdev.org/GCC_Cross-Compiler
#
# requires: texinfo

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

mkdir -p build
chmod -R u+w build
rm -Rf build/*
cd build

chmod +x ../src/configure
../src/configure --target="$TARGET" --prefix="$PREFIX" --with-sysroot \
	--disable-nls --disable-werror

make
make install

chmod +x "$PREFIX/bin/$TARGET"-*
cd .. ; chmod -R u+w build ; rm -Rf build

