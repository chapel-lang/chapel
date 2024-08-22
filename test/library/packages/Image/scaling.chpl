use Image;
use IO;

var arr: [1..#3, 1..#3] int;

arr[1, ..] = [0xFF0000, 0x00FF00, 0x0000FF];
arr[2, ..] = [0x00FF00, 0x0000FF, 0xFF0000];
arr[3, ..] = [0x0000FF, 0xFF0000, 0x00FF00];

writeImage(stdout, imageType.bmp, scale(arr, 2));
