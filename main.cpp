/*
 * Copyright 2018-2023. All rights reserved.
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
#include <Application.h>
#include <Bitmap.h>
#include <Catalog.h>
#include <Clipboard.h>
#include <Entry.h>
#include <IconUtils.h>
#include <NetworkInterface.h>
#include <NetworkRoster.h>
#include <Notification.h>
#include <Path.h>
#include <Rect.h>
#include <Roster.h>
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
		if ((flags & IFF_LOOPBACK) == 0 && (flags & (IFF_UP | IFF_LINK)) == (IFF_UP | IFF_LINK))
			return true;
	}
	// No network connection detected, cannot continue
	return false;
}


BString
GetStdoutFromCommand(BString cmd)
{

	BString data;
	FILE* stream;
	const int max_buffer = 256;
	char buffer[max_buffer];

	stream = popen(cmd.String(), "r");

	if (stream) {
		while (!feof(stream))
			if (fgets(buffer, max_buffer, stream) != NULL)
				data.Append(buffer);
		pclose(stream);
	}
	return data;
}


void
CopyToClipboard(BString text)
{
	ssize_t textLen = text.Length();
	BMessage* message = (BMessage*) NULL;

	if (be_clipboard->Lock()) {
		be_clipboard->Clear();
		if ((message = be_clipboard->Data())) {
			message->AddData("text/plain", B_MIME_TYPE, text.String(), textLen);
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

	// Add time for a reasonably unique MessageID
	bigtime_t time(real_time_clock());
	BString msgID("UploadIt-");
	msgID << time;

	BNotification notification(B_INFORMATION_NOTIFICATION);
	notification.SetMessageID(msgID);
	notification.SetGroup("UploadIt");

	BBitmap* icon = new BBitmap(BRect(0, 0, B_LARGE_ICON - 1, B_LARGE_ICON - 1), B_RGBA32);
	BMimeType ourself("application/x-vnd.humdinger-UploadIt");

	if (ourself.GetIcon(icon, B_LARGE_ICON) == B_OK)
		notification.SetIcon(icon);

	if (msg->FindRef("refs", &file_ref) == B_NO_ERROR) {
		BString title;
		BString content;
		if (CheckNetworkConnection() == false) {
			title = B_TRANSLATE("Upload failed");
			content = B_TRANSLATE("Online service not available");
		} else {
			BEntry entry(&file_ref);
			entry.GetPath(&path);
			notification.SetTitle(B_TRANSLATE("Uploading " B_UTF8_ELLIPSIS));
			notification.SetContent(path.Leaf());
			notification.Send(600000000);

			BString command("curl -F 'file=@'\"%FILEPATH%\" https://0x0.st");
			if (entry.IsDirectory()) {
				BPath parent;
				if (path.GetParent(&parent) == B_OK) {
					command.Prepend("cd \"%PARENTPATH%\" && zip -qry - \"%FOLDERNAME%\" | ");
					command.ReplaceFirst("%PARENTPATH%", parent.Path());
					command.ReplaceFirst("%FOLDERNAME%", path.Leaf());
					command.ReplaceLast("%FILEPATH%", "-");
				}
			} else
				command.ReplaceFirst("%FILEPATH%", path.Path());

			content = GetStdoutFromCommand(command.String());
			content.ReplaceLast("\n", "");

			title = B_TRANSLATE("Upload failed");
			if (content.FindFirst("413 Request Entity Too Large") != B_ERROR) {
				content = B_TRANSLATE("'%FILENAME%' is too large");
				content.ReplaceFirst("%FILENAME%", path.Leaf());
			} else if (content.FindFirst("https://0x0.st/") == 0) {
				title = (B_TRANSLATE_COMMENT("Finished. URL in clipboard:",
					"As short as possible, not much space in a Notification."));
			}
		}
		notification.SetTitle(title);
		notification.SetContent(content);
		notification.Send();
		CopyToClipboard(content);
	}
}


int
main(int, char**)
{
	fprintf(stderr, B_TRANSLATE("UploadIt can only be used as a Tracker add-on.\n"));
	return -1;
}
