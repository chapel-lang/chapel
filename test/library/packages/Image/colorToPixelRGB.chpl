use Image;
use IO;

var colors: [1..#3, 1..#3] 3*int;

colors[1, ..] = [(0xFF,0,0), (0,0xFF,0), (0,0,0xFF)];
colors[2, ..] = [(0,0xFF,0), (0,0,0xFF), (0xFF,0,0)];
colors[3, ..] = [(0,0,0xFF), (0xFF,0,0), (0,0xFF,0)];

var pixels = colorToPixel(colors/*rgb is default*/);

writeImage("pixels.bmp", imageType.bmp, pixels);

// roundtrip the pixels back to colors and make sure everything matches
var colors2 = pixelToColor(pixels);
assert(colors == colors2);
