use Image;

/* START_EXAMPLE */
var color: [1..3, 1..3] 3*int;

color[1, ..] = [(0xFF,0,0), (0,0xFF,0), (0,0,0xFF)];
color[2, ..] = [(0,0xFF,0), (0,0,0xFF), (0xFF,0,0)];
color[3, ..] = [(0,0,0xFF), (0xFF,0,0), (0,0xFF,0)];

var format = (rgbColor.blue, rgbColor.green, rgbColor.red);
var arr = colorToPixel(color, format=format);

writeImage("pixels.bmp", imageType.bmp, arr);
writeImage("pixels2.bmp", imageType.bmp, scale(arr, 2));
/* STOP_EXAMPLE */

