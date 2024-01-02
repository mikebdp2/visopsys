#!/bin/sh
##
##  Visopsys
##  Copyright (C) 1998-2023 J. Andrew McLaughlin
##
##  check-patchset.sh
##

# Checks that the patchset for a branch release is valid.  Unzip the previous
# source release, copy the patches (from main) into the top-level directory,
# and run this script from there.

for PATCH in *.patch ; do
	echo "$PATCH"
	patch -p0 --dry-run < "$PATCH"
	if [ $? != 0 ] ; then
		echo "$PATCH dry-run failed"
		exit 1
	fi

	patch -p0 < "$PATCH"
	if [ $? != 0 ] ; then
		echo "$PATCH failed"
		exit 1
	fi
done

