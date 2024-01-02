//
//  Visopsys
//  Copyright (C) 1998-2023 J. Andrew McLaughlin
//
//  This program is free software; you can redistribute it and/or modify it
//  under the terms of the GNU General Public License as published by the Free
//  Software Foundation; either version 2 of the License, or (at your option)
//  any later version.
//
//  This program is distributed in the hope that it will be useful, but
//  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
//  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
//  for more details.
//
//  You should have received a copy of the GNU General Public License along
//  with this program; if not, write to the Free Software Foundation, Inc.,
//  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
//  errno.c
//

// This is the UNIX-style command for printing the error message associated
// with an error code

/* This is the text that appears when a user requests help about this program
<help>

 -- errno --

Print the error message associated with a numeric error code.

Usage:
  errno <error number>

</help>
*/

#include <errno.h>
#include <libintl.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/env.h>

#define _(string) gettext(string)


static void usage(char *name)
{
	fprintf(stderr, _("usage:\n%s <error number>\n"), name);
}


int main(int argc, char *argv[])
{
	int status = 0;
	int error = 0;
	errorStringTableEntry *entry = NULL;

	setlocale(LC_ALL, getenv(ENV_LANG));
	textdomain("errno");

	if ((argc < 2) || !argv[1])
	{
		usage(argv[0]);
		return (status = ERR_NODATA);
	}

	errno = 0;
	error = strtol(argv[1], NULL /* endString */, 10 /* base */);
	if (errno)
	{
		fprintf(stderr, _("ERROR: Not understood: %s\n"), argv[1]);
		usage(argv[0]);
		return (status = ERR_DOMAIN);
	}

	// If the user omitted the minus sign, fix it
	if (error > 0)
		error *= -1;

	entry = _getErrorStringTableEntry(error);
	if (!entry)
		return (status = ERR_NOSUCHENTRY);

	// Turn it into a string and print it
	printf("%s %d %s\n", entry->name, entry->code, entry->string);

	return (status = 0);
}

