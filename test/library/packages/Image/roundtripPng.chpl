use Image;
use IO;

// read in the image and write it back out
const pixels = readImage("roundtripPng.good", imageType.png);
writeImage("pixels.png", imageType.png, pixels);
