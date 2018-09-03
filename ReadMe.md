# UploadIt

UploadIt is a Tracker Add-On that uploads a single file to the online service *The Null Pointer* at https://0x0.st . The URL for the uploaded file is put into the clipboard after the upload has finished, ready to be pasted into an email, a chat window or forum post.

According to *The Null Pointer* website:

* Maximum file size: 512.0 MiB
* Not allowed: application/x-dosexec, child pornography
* File URLs are valid for at least 30 days and up to a year, depending on file size:
```
   days
    365 |  \
        |   \
        |    \
        |     \
        |      \
        |       \
        |        ..
        |          \
  197.5 | ----------..-------------------------------------------
        |             ..
        |               \
        |                ..
        |                  ...
        |                     ..
        |                       ...
        |                          ....
        |                              ......
     30 |                                    ....................
          0                        256                        512
                                                              MiB
```

### Tips

* Install Clipdinger from HaikuDepot. It includes a "Clipboard Monitor" that you can embed as Replicant on the Desktop. When you see the upload URL appearing there, you know the file was uploaded successfully and the URL can be pasted.

* UploadIt uses a curl command line that does all the work. You can put the line into your ```~/config/settings/profile``` and use "upload" from Terminal:
```
function upload() {
    curl -F'file=@'$1 https://0x0.st | clipboard -i
}
```
### Manual build/install

* Build with a simple "make".
* Copy UploadIt to ```~/config/non-packaged/add-ons/Tracker.```
* For it to actually work, you need to have the curl package installed (```pkgman install cmd:curl```)
