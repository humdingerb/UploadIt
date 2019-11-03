/*
 * Copyright 2018. All rights reserved.
 * Distributed under the terms of the MIT license.
 *
 * Author:
 *	Humdinger, humdingerb@gmail.com
 *
 * Modified code from CopyNameToClipboard
 * _CheckNetworkConnection() borrowed from SoftwareUpdater
 */
 
#include <stdio.h>
#include <stdlib.h>

#include <Alert.h>
#include <Catalog.h>
#include <Clipboard.h>
#include <Entry.h>
#include <NetworkInterface.h>
#include <NetworkRoster.h>
#include <Path.h>
#include <String.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "Add-On"


bool
CheckNetworkConnection()
{
	BNetworkRoster& roster = BNetworkRoster::Default();
	BNetworkInterface interface;
	uint32 cookie = 0;
	while (roster.GetNextInterface(&cookie, interface) == B_OK) {
		uint32 flags = interface.Flags();
		if ((flags & IFF_LOOPBACK) == 0
			&& (flags & (IFF_UP | IFF_LINK)) == (IFF_UP | IFF_LINK)) {
			return true;
		}
	}
	// No network connection detected, cannot continue
	return false;
}


void
CopyToClipboard(BString text)
{
	ssize_t textLen = text.Length();
	BMessage* message = (BMessage *)NULL;

	if (be_clipboard->Lock()) {
		be_clipboard->Clear();
		if ((message = be_clipboard->Data())) {
			message->AddData("text/plain", B_MIME_TYPE, text.String(),
				textLen);
			be_clipboard->Commit();
		}
		be_clipboard->Unlock();
	}
}


extern "C" void
process_refs(entry_ref directoryRef, BMessage* msg, void*)
{
	BPath path;
	entry_ref file_ref;

	if (msg->FindRef("refs", &file_ref) == B_NO_ERROR) {
		BEntry entry(&file_ref);
		if (entry.IsDirectory()) {
			BString text(B_TRANSLATE(
				"UploadIt only works on a single file, no folders"));
			CopyToClipboard(text);
		} else if (CheckNetworkConnection() == false) {
			BString text(B_TRANSLATE(
				"Online upload service not available"));
			CopyToClipboard(text);
		} else {
			entry.GetPath(&path);
			BString command(
//				0x0.st has been quite unreliable
//				"curl -F'file=@'\"%FILENAME%\" https://0x0.st | clipboard -i ; "
//				switching to linx.li instead (expire in one week (60*60*24*7)
				"curl -H \"Linx-Expiry: 604800\" -T \"%FILENAME%\" https://linx.li/upload/ | clipboard -i ; "
				"exit");
			command.ReplaceAll("%FILENAME%", path.Path());
			system(command.String());
		}
	}
}


int
main(int, char**)
{
	fprintf(stderr, B_TRANSLATE("UploadIt can only be used as a Tracker add-on.\n"));
	return -1; 
}
