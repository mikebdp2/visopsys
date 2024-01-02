/*
 * device_io.h - Exports for default device io. Part of the Linux-NTFS project.
 *
 * Copyright (c) 2000-2006 Anton Altaparmakov
 *
 * This program/include file is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program/include file is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (in the main directory of the Linux-NTFS
 * distribution in the file COPYING); if not, write to the Free Software
 * Foundation,Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Modified 01/2007 by Andy McLaughlin for Visopsys port.
 */

#ifndef _NTFS_DEVICE_IO_H
#define _NTFS_DEVICE_IO_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifndef NO_NTFS_DEVICE_DEFAULT_IO_OPS

#if !defined(__CYGWIN32__) && !defined(__VISOPSYS__)

/* Not on Cygwin; use standard Unix style low level device operations. */
#define ntfs_device_default_io_ops ntfs_device_unix_io_ops

#else

#ifndef HDIO_GETGEO
#	define HDIO_GETGEO	0x301
/**
 * struct hd_geometry -
 */
struct hd_geometry {
	unsigned char heads;
	unsigned char sectors;
	unsigned short cylinders;
	unsigned long start;
};
#endif
#ifndef BLKGETSIZE
#	define BLKGETSIZE	0x1260
#endif
#ifndef BLKSSZGET
#	define BLKSSZGET	0x1268
#endif
#ifndef BLKGETSIZE64
#	define BLKGETSIZE64	0x80041272
#endif
#ifndef BLKBSZSET
#	define BLKBSZSET	0x40041271
#endif

#if defined(__VISOPSYS__)

#define ntfs_device_default_io_ops ntfs_device_visopsys_io_ops

#else /* __CYGWIN32__ */

/* On Cygwin; use Win32 low level device operations. */
#define ntfs_device_default_io_ops ntfs_device_win32_io_ops

#endif /* __VISOPSYS__ */

#endif /* __CYGWIN32__ || __VISOPSYS__ */

/* Forward declaration. */
struct ntfs_device_operations;

extern struct ntfs_device_operations ntfs_device_default_io_ops;

#endif /* NO_NTFS_DEVICE_DEFAULT_IO_OPS */

#endif /* defined _NTFS_DEVICE_IO_H */
