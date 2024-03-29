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
//  pthread_exit.c
//

// This is the "pthread_exit" function as found in POSIX thread libraries

#include <pthread.h>
#include <sys/api.h>


void pthread_exit(void *exitCode)
{
	// Excerpted from the POSIX programming manual:
	//
	// The pthread_exit() function shall terminate the calling thread and make
	// the value exitCode available to any successful join with the
	// terminating thread.  Thread termination does not release any
	// application-visible process resources.
	//
	// An implicit call to pthread_exit() is made when a thread other than the
	// thread in which main() was first invoked returns from the start routine
	// that was used to create it.  The function's return value shall serve as
	// the thread's exit status.

	while (1)
	{
		multitaskerTerminate((int) exitCode);
		multitaskerYield();
	}
}

