# UploadIt

UploadIt is a Tracker Add-On that uploads a single file to the online service https://oshi.at. The URL for the uploaded file is put into the clipboard after the upload has finished, ready to be pasted into an email, a chat window or forum post.

Uploaded files can be up to 1000 MiB in size and are stored for 14 days.

### Tips

* Install Clipdinger from HaikuDepot. It includes a "Clipboard Monitor" that you can embed as Replicant on the Desktop. When you see the upload URL appearing there, you know the file was uploaded successfully and the URL can be pasted.

* UploadIt uses a curl command line that does all the work. You can put the line into your ```~/config/settings/profile``` and use "upload" from Terminal:
   <pre>
function upload() {
	curl https://oshi.at -F f=@$1 -F expire=20160 | grep "DL:" | awk '{ print $2; }' | clipboard -i
	clipboard -p
}
</pre>

   The expiration time is set in minutes, 20160 being 14 days

### Manual build/install

* Build with a simple "make".
* Copy UploadIt to ```~/config/non-packaged/add-ons/Tracker/```.
* For it to actually work, you need to have the curl package installed (```pkgman install cmd:curl```)
