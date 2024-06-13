
use Image;
use IO;

var arr: [1..#5, 0..#3] real;

arr[1, ..] = [1.0, 1.0, 1.0];
arr[2, ..] = [0.75, 0.75, 0.75];
arr[3, ..] = [0.5, 0.5, 0.5];
arr[4, ..] = [0.25, 0.25, 0.25];
arr[5, ..] = [0.0, 0.0, 0.0];

writeImage(stdout, imageType.bmp, downSample(arr));
