# Bitmap Displayer

**A work in progress.** 

![Bitmap Displayer](/Screenshots/BitmapDisplayer1.png?raw=true "Screenshot")

Displays bitmap images (currently limited to 24 bit colour) in the command line.

## Current Limitations
* You will need 
[Windows Insiders build 14931](https://blogs.msdn.microsoft.com/commandline/2016/09/22/24-bit-color-in-the-windows-console/)
or later to run this.
* For larger images to work, the font size will need to be manually decreased, 
the horizontal and vertical screen buffers increased, and the command-line window size increased.
* Text wrap needs to be disabled manually.
* Usage of fonts that do not have glyphs which are exactly twice the height as they are wide will
result in distorted images.
* The filename needs to be entered in the source code and cannot be changed without compiling. 
Currently, the program tries to open `"TestImage.bmp"`.

## Todo list

- [ ] Allow for user input of filenames.
- [ ] Support non 24 bit colour bitmap images with colour tables.
- [ ] Automatically change font size, buffer size, and window size.
- [ ] Add cmake file.
