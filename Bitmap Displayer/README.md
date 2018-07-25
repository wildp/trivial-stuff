# Bitmap Displayer

**A work in progress.** 

![BitmapDisplayer](/Screenshots/BitmapDisplayer1.png?raw=true "Screenshot")

Displays bitmap images (currently limited to 24 bit colour) in the command line.

## Current Limitations
* Your console will need to support escape codes and 24 bit colour.
* For larger images to work, the font size will need to be manually decreased, 
the horizontal and vertical screen buffers increased, and the command-line window size increased.
* Text wrap needs to be disabled manually.
* Usage of fonts that do not have glyphs which are exactly twice the height as they are wide will
result in distorted images.
* The filename needs to be entered in the source code and cannot be changed without compiling.

## Todo list

- [ ] Allow for user input of filenames.
- [ ] Support non 24 bit colour bitmap images with colour tables.
- [ ] Automatically change font size, buffer size, and window size.
- [ ] Add cmake file.
