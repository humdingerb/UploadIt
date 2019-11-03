# UploadIt

UploadIt is a Tracker Add-On that uploads a single file to the online service *Linx* at https://linx.li. The URL for the uploaded file is put into the clipboard after the upload has finished, ready to be pasted into an email, a chat window or forum post.

Currently the expiration time for the uploaded file is hard-coded to 1 week.

### Tips

* Install Clipdinger from HaikuDepot. It includes a "Clipboard Monitor" that you can embed as Replicant on the Desktop. When you see the upload URL appearing there, you know the file was uploaded successfully and the URL can be pasted.

* UploadIt uses a curl command line that does all the work. You can put the line into your ```~/config/settings/profile``` and use "upload" from Terminal:
```
function upload() {
	curl -H "Linx-Expiry: 604800" -T "$1" https://linx.li/upload/ | clipboard -i
	clipboard -p
}
```

The number for *Linx-Expiry* is the expiration time in seconds (604800 = 1 week).

### Manual build/install

* Build with a simple "make".
* Copy UploadIt to ```~/config/non-packaged/add-ons/Tracker.```
* For it to actually work, you need to have the curl package installed (```pkgman install cmd:curl```)
