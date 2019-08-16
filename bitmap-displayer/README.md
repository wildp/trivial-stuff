# Bitmap Displayer

**A work in progress.** 

![Bitmap Displayer](/Screenshots/BitmapDisplayer1.png?raw=true "Screenshot")

Displays bitmap images (currently limited to 24 bit true colour and 8, 4 & 1 bit indexed colour) in the command line.

## Current Limitations
* You will need 
[Windows Insiders build 14931](https://blogs.msdn.microsoft.com/commandline/2016/09/22/24-bit-color-in-the-windows-console/)
or later to run this. I have not tested this on GNU\Linux.
* For larger images to work, the font size will need to be manually decreased, 
the horizontal and vertical screen buffers increased, and the command-line window size increased.
* Text wrap needs to be disabled manually.
* Usage of fonts that do not have glyphs which are exactly twice the height as they are wide will
result in distorted images.
* ~~The filename needs to be entered in the source code and cannot be changed without compiling.~~
~~Currently, the program tries to open `"TestImage.bmp"`.~~ The filename can now be added through
command line arguments.
* Images with padding are not supported and will display incorrectly.
* Only supports bitmap images with the `BITMAPINFOHEADER` structure.

## Todo list

- [x] Allow for user input of filenames. *(can be done via command line arguments)*
- [x] Support non 24 bit colour bitmap images with colour tables.
- [ ] Automatically change font size, buffer size, and window size.
- [ ] Add cmake file.
