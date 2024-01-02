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
//  windowCancelDialog.c
//

// This contains functions for user programs to operate GUI components

#include <libintl.h>
#include <string.h>
#include <sys/api.h>
#include <sys/errors.h>
#include <sys/window.h>

#define _(string) gettext(string)

extern int libwindow_initialized;
extern void libwindowInitialize(void);

static void (*cancelCallBack)(objectKey);
static objectKey dialogWindow = NULL;
static objectKey cancelButton = NULL;
static int threadPid = 0;


static void cancelThread(void)
{
	// This thread checks for the user pressing the Cancel button, and if so,
	// calls the callback function

	int status = 0;
	windowEvent event;

	while (1)
	{
		// Check for our Cancel button
		status = windowComponentEventGet(cancelButton, &event);
		if ((status < 0) ||
			((status > 0) && (event.type == WINDOW_EVENT_MOUSE_LEFTUP)))
		{
			cancelCallBack(dialogWindow);
			break;
		}

		// Not finished yet
		multitaskerYield();
	}

	// Exit
	multitaskerTerminate(0);
}


/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
//
// Below here, the functions are exported for external use
//
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

_X_ objectKey windowNewCancelDialog(objectKey parentWindow, const char *title, const char *message, void (*callBack)(objectKey))
{
	// Desc: Create a 'cancel' dialog box, with the parent window 'parentWindow', and the given titlebar text and main message.  The supplied 'callBack' function is called if the user presses the Cancel button.  If 'parentWindow' is NULL, the dialog box is actually created as an independent window that looks the same as a dialog.  This is a non-blocking call that returns the object key of the dialog window.  The caller must destroy the window when finished with it.

	objectKey container = NULL;
	componentParameters params;

	if (!libwindow_initialized)
		libwindowInitialize();

	// Check params.  It's okay for parentWindow to be NULL.
	if (!title || !message || !callBack)
		return (dialogWindow = NULL);

	cancelCallBack = callBack;

	memset(&params, 0, sizeof(componentParameters));
	params.gridWidth = 1;
	params.gridHeight = 1;
	params.padLeft = 5;
	params.padRight = 5;
	params.padTop = 5;
	params.padBottom = 5;
	params.orientationX = orient_center;
	params.orientationY = orient_middle;

	// Create the dialog.  Arbitrary size and coordinates.
	if (parentWindow)
		dialogWindow = windowNewDialog(parentWindow, title);
	else
		dialogWindow = windowNew(multitaskerGetCurrentProcessId(), title);
	if (!dialogWindow)
		return (dialogWindow);

	// Get a container to pack everything into
	container = windowNewContainer(dialogWindow, "container", &params);
	if (!container)
		return (dialogWindow = NULL);

	params.padLeft = params.padRight = params.padTop = 0;

	// Create the label
	if (!windowNewTextLabel(container, message, &params))
		return (dialogWindow = NULL);

	// Create the button
	params.gridY++;
	params.padBottom = 0;
	params.flags |= (COMP_PARAMS_FLAG_FIXEDWIDTH |
		COMP_PARAMS_FLAG_FIXEDHEIGHT);
	cancelButton = windowNewButton(container, _("Cancel"), NULL, &params);
	if (!cancelButton)
		return (dialogWindow = NULL);

	// No need for a close button because there's no handler for it
	windowRemoveCloseButton(dialogWindow);

	// If there's no parentWindow, the dialog will center on the screen
	windowCenterDialog(parentWindow, dialogWindow);

	windowSetVisible(dialogWindow, 1);

	// Spawn our thread to watch for cancel
	threadPid = multitaskerSpawn(cancelThread, "cancel thread",
		0 /* no args */, NULL /* no args */, 1 /* run */);
	if (threadPid < 0)
	{
		windowDestroy(dialogWindow);
		return (dialogWindow = NULL);
	}

	return (dialogWindow);
}


_X_ int windowCancelDialogDestroy(objectKey window)
{
	// Desc: Given the objectKey for a cancel dialog 'window' previously returned by windowNewCancelDialog(), destroy and deallocate the window.

	int status = 0;

	if (!libwindow_initialized)
		libwindowInitialize();

	if (!window)
		return (status = ERR_NULLPARAMETER);

	if (window != dialogWindow)
		return (status = ERR_INVALID);

	if (threadPid && multitaskerProcessIsAlive(threadPid))
	{
		// Kill our thread
		status = multitaskerKillProcess(threadPid);
	}

	// Destroy the window
	windowDestroy(dialogWindow);

	dialogWindow = NULL;
	cancelButton = NULL;
	threadPid = 0;

	return (status);
}

