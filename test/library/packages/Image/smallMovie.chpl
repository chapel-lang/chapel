
use Image;
use IO;

config const filename = "smallMovie.mp4";
config const factor = 1;

var pipe = new mediaPipe(filename, imageType.bmp, framerate=1);

const white = 0xFFFFFF;
const blue = 0xFF0000;

var frame: [1..#2, 0..#4] real;

frame[1, ..] = 1.0;
frame[2, ..] = 1.0;
pipe.writeFrame(
  scale(interpolateColor(frame, white, blue, colorRange=(0.0,1.0)), factor));

frame[1, ..] = 0.5;
frame[2, ..] = 0.5;
pipe.writeFrame(
  scale(interpolateColor(frame, white, blue, colorRange=(0.0,1.0)), factor));

frame[1, ..] = 0.0;
frame[2, ..] = 0.0;
pipe.writeFrame(
  scale(interpolateColor(frame, white, blue, colorRange=(0.0,1.0)), factor));

pipe.finish();

