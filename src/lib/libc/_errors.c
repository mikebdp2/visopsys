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
//  _errors.c
//

// A global table of errors, and a function for indexing them

#include <stdlib.h>
#include <sys/errors.h>

errorStringTableEntry errorStringTable[] = {
	{ /* 0   */ ERR_NOERROR,		"ERR_NOERROR",
		"No error." },
	{ /* -1  */ ERR_ERROR, 			"ERR_ERROR",
		"General error - no additional information." },
	{ /* -2  */ ERR_INVALID,		"ERR_INVALID",
		"Invalid operation." },
	{ /* -3  */ ERR_PERMISSION,		"ERR_PERMISSION",
		"Permission denied." },
	{ /* -4  */ ERR_MEMORY,			"ERR_MEMORY",
		"Memory allocation or freeing error." },
	{ /* -5  */ ERR_BUSY,			"ERR_BUSY",
		"The resource is busy." },
	{ /* -6  */ ERR_NOSUCHENTRY,	"ERR_NOSUCHENTRY",
		"Object does not exist." },
	{ /* -7  */ ERR_BADADDRESS,		"ERR_BADADDRESS",
		"Invalid memory address." },
	{ /* -8  */ ERR_TIMEOUT,		"ERR_TIMEOUT",
		"Operation timed out." },
	{ /* -9  */ ERR_NOTINITIALIZED,	"ERR_NOTINITIALIZED",
		"Resource has not been initialized." },
	{ /* -10 */ ERR_NOTIMPLEMENTED,	"ERR_NOTIMPLEMENTED",
		"Requested functionality not implemented." },
	{ /* -11 */ ERR_NULLPARAMETER,	"ERR_NULLPARAMETER",
		"Required parameter was NULL." },
	{ /* -12 */ ERR_NODATA,			"ERR_NODATA",
		"No data supplied." },
	{ /* -13 */ ERR_BADDATA,		"ERR_BADDATA",
		"Corrupt data encountered." },
	{ /* -14 */ ERR_ALIGN,			"ERR_ALIGN",
		"Memory alignment error." },
	{ /* -15 */ ERR_NOFREE,			"ERR_NOFREE",
		"No free resources." },
	{ /* -16 */ ERR_DEADLOCK,		"ERR_DEADLOCK",
		"Deadlock situation avoided." },
	{ /* -17 */ ERR_PARADOX,		"ERR_PARADOX",
		"Requested action is paradoxical." },
	{ /* -18 */ ERR_NOLOCK,			"ERR_NOLOCK",
		"Resource lock could not be obtained." },
	{ /* -19 */ ERR_NOVIRTUAL,		"ERR_NOVIRTUAL",
		"Virtual memory error." },
	{ /* -20 */ ERR_EXECUTE,		"ERR_EXECUTE",
		"Command could not be executed." },
	{ /* -21 */ ERR_NOTEMPTY,		"ERR_NOTEMPTY",
		"Object is not empty." },
	{ /* -22 */ ERR_NOCREATE,		"ERR_NOCREATE",
		"Cannot create." },
	{ /* -23 */ ERR_NODELETE,		"ERR_NODELETE",
		"Cannot delete." },
	{ /* -24 */ ERR_IO,				"ERR_IO",
		"Device input/output error." },
	{ /* -25 */ ERR_BOUNDS,			"ERR_BOUNDS",
		"Out of bounds error." },
	{ /* -26 */ ERR_ARGUMENTCOUNT,	"ERR_ARGUMENTCOUNT",
		"Incorrect number of parameters." },
	{ /* -27 */ ERR_ALREADY,		"ERR_ALREADY",
		"Requested action is unnecessary." },
	{ /* -28 */ ERR_DIVIDEBYZERO,	"ERR_DIVIDEBYZERO",
		"Divide by zero error." },
	{ /* -29 */ ERR_DOMAIN,			"ERR_DOMAIN",
		"Math operation is not in the domain." },
	{ /* -30 */ ERR_RANGE,			"ERR_RANGE",
		"Math operation is out of range." },
	{ /* -31 */ ERR_CANCELLED,		"ERR_CANCELLED",
		"Operation was cancelled." },
	{ /* -32 */ ERR_KILLED,			"ERR_KILLED",
		"Process killed." },
	{ /* -33 */ ERR_NOMEDIA,		"ERR_NOMEDIA",
		"No media present." },
	{ /* -34 */ ERR_NOSUCHFILE,		"ERR_NOSUCHFILE",
		"No such file." },
	{ /* -35 */ ERR_NOSUCHDIR,		"ERR_NOSUCHDIR",
		"No such directory." },
	{ /* -36 */ ERR_NOTAFILE,		"ERR_NOTAFILE",
		"Object is not a file." },
	{ /* -37 */ ERR_NOTADIR,		"ERR_NOTADIR",
		"Object is not a directory." },
	{ /* -38 */ ERR_NOWRITE,		"ERR_NOWRITE",
		"Cannot write data." },
	{ /* -39 */ ERR_HOSTUNKNOWN,	"ERR_HOSTUNKNOWN",
		"Host lookup failed." },
	{ /* -40 */ ERR_NOROUTETOHOST,	"ERR_NOROUTETOHOST",
		"No route to host." },
	{ /* -41 */ ERR_NOCONNECTION,	"ERR_NOCONNECTION",
		"Couldn't connect." },
	{ /* -42 */ ERR_NOSUCHUSER,		"ERR_NOSUCHUSER",
		"No such user." },
	{ /* -43 */ ERR_NOSUCHPROCESS,	"ERR_NOSUCHPROCESS",
		"No such process." },
	{ /* -44 */ ERR_NOSUCHDRIVER,	"ERR_NOSUCHDRIVER",
		"There is no driver for this operation." },
	{ /* -45 */ ERR_NOSUCHFUNCTION,	"ERR_NOSUCHFUNCTION",
		"Operation not supported." },
	{ /* -46 */ ERR_BUG,			"ERR_BUG",
		"Internal error (bug)." },
	{ 0, NULL, NULL }
};


errorStringTableEntry *_getErrorStringTableEntry(int error)
{
	int count;

	for (count = 0; errorStringTable[count].string; count ++)
	{
		if (errorStringTable[count].code == error)
			return (&errorStringTable[count]);
	}

	// Not found.  Don't change errno.
	return (NULL);
}

