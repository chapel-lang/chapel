use Image;

// create the input image
{
  var colors: [1..#3, 1..#3] 3*int;
  colors[1, ..] = [(0xFF,0,0), (0,0xFF,0), (0,0,0xFF)];
  colors[2, ..] = [(0,0xFF,0), (0,0,0xFF), (0xFF,0,0)];
  colors[3, ..] = [(0,0,0xFF), (0xFF,0,0), (0,0xFF,0)];
  var pixels = colorToPixel(colors, format=(rgbColor.blue, rgbColor.green, rgbColor.red));
  writeImage("input.png", imageType.png, pixels);
}


//
// the second example from the Image module docs
//
var arr = readImage("input.png", imageType.png);
const fmt = (rgbColor.red, rgbColor.green, rgbColor.blue);
var colors = pixelToColor(arr, format=fmt);
[c in colors] c(1) = 0;
arr = colorToPixel(colors, format=fmt);
writeImage("output.jpg", imageType.jpg, arr);


// clean up file not needed
use FileSystem;
remove("input.png");
