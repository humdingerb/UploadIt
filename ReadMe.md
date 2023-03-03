# ![UploadIt icon](./icon.png) UploadIt

UploadIt is a Tracker Add-On that uploads a single file or folder to the online service [The Null Pointer](https://0x0.st/). A folder gets automatically zipped up before uploading. The URL for the uploaded file is put into the clipboard after the upload has finished, ready to be pasted into an email, a chat window or forum post.

Depending on the size of the file, uploaded files are retained for 30 days to one year. The maximum file size is 512 MiB. 0x0.st is NOT a platform for:

* piracy
* pornography
* extremist material of any kind
* malware / botnet C&C
* anything related to crypto currencies
* backups
* CI build artifacts
* doxxing, database dumps containing personal information
* anything illegal under German law

Uploads found to be in violation of these rules will be removed, and the originating IP address blocked from further uploads.

More details at [The Null Pointer](http://0x0.st/).


### Tips

* Install Clipdinger from HaikuDepot. It includes a "Clipboard Monitor" that you can embed as Replicant on the Desktop. When you see the upload URL appearing there, you know the file was uploaded successfully and the URL can be pasted.

* UploadIt uses a curl command line that does all the work. You can put the line into your ```~/config/settings/profile``` and use "upload" from Terminal:

```
function upload() {
	curl -F 'file=@'"$1" http://0x0.st | clipboard -i
	clipboard -p
}
```


### Manual build/install

* Build with a simple "make". To add the minimal localization, do "make bindcatalogs" after that.
* Copy UploadIt to ```~/config/non-packaged/add-ons/Tracker/```.
* For it to actually work, you need to have the 'curl' and 'zip' package installed (```pkgman install cmd:curl cmd:zip```)
