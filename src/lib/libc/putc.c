//
//  Visopsys
//  Copyright (C) 1998-2023 J. Andrew McLaughlin
//
//  This library is free software; you can redistribute it and/or modify it
//  under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation; either version 2.1 of the License, or (at
//  your option) any later version.
//
//  This library is distributed in the hope that it will be useful, but
//  WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser
//  General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with this library; if not, write to the Free Software Foundation,
//  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
//  putc.c
//

// This is the standard "putc" function, as found in standard C libraries

#include <stdio.h>
#include <errno.h>
#include <sys/api.h>


int putc(int c, FILE *theStream)
{
	// putc() is equivalent to fputc() except that it may be implemented as a
	// macro which evaluates stream more than once.  OK, it's not a macro, and
	// it's only going to be partially implemented right now.

	int status = 0;

	if (visopsys_in_kernel)
		return (errno = ERR_BUG);

	if (theStream != stdin)
	{
		// We are only implementing for stdin at the moment
		errno = ERR_NOTIMPLEMENTED;
		return (EOF);
	}

	// Get a character from the text input stream
	status = textPutc(c);
	if (status < 0)
	{
		errno = status;
		return (EOF);
	}

	return (c);
}

