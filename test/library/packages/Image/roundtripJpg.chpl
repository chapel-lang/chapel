use Image;
use IO;

// because JPG is potentially lossy, we can't guarantee that the binary image will match with a diff
// so we write it out, read it back in, and compare the arrays

var colors: [1..#3, 1..#3] 3*int;

colors[1, ..] = [(0xFF,0,0), (0,0xFF,0), (0,0,0xFF)];
colors[2, ..] = [(0,0xFF,0), (0,0,0xFF), (0xFF,0,0)];
colors[3, ..] = [(0,0,0xFF), (0xFF,0,0), (0,0xFF,0)];

var pixels = colorToPixel(colors, format=(rgbColor.blue, rgbColor.green, rgbColor.red));

writeImage("pixels.jpg", imageType.jpg, pixels);

const pixels2 = readImage("pixels.jpg", imageType.jpg);
const colors2 = pixelToColor(pixels2, format=(rgbColor.blue, rgbColor.green, rgbColor.red));

writeln(&& reduce compareColor(colors, colors2));
proc compareColor(c1: 3*int, c2: c1.type, epsilon = 2) {
  for i in 0..#c1.size {
    if abs(c1[i] - c2[i]) > epsilon {
      return false;
    }
  }
  return true;
}
