
use Image;
use IO;

var arr: [1..#1, 0..#3] real;

arr[1, ..] = [1.0, 0.5, 0.0];

const white = 0xFFFFFF;
const blue = 0xFF0000;
writeImage(stdout, imageType.bmp, interpolateColor(arr, white, blue));
