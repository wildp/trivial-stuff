# Bitmap Displayer

![Bitmap Displayer](/.screenshots/BitmapDisplayer1.png?raw=true "Screenshot")

Displays bitmap images (currently limited to 24 bit true colour and 8, 4 & 1 bit indexed colour) in the command line.

## Limitations
* Requires virtual terminal to support wide characters and ANSI escape codes.
* For larger images to work, the font size will need to be manually decreased, 
the horizontal and vertical screen buffers increased, and the virtual terminal window size increased.
* Text wrap needs to be disabled.
* Use of fonts that do not have glyphs which are exactly twice the height as they are wide will
result in distorted images.
* Images with padding are not supported and will display incorrectly.
* Only supports bitmap images with the `BITMAPINFOHEADER` structure.

