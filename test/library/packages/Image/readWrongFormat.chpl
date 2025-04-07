use Image;
use IO;


proc main() {

  var colors: [1..#3, 1..#3] 3*int;

  colors[1, ..] = [(0xFF,0,0), (0,0xFF,0), (0,0,0xFF)];
  colors[2, ..] = [(0,0xFF,0), (0,0,0xFF), (0xFF,0,0)];
  colors[3, ..] = [(0,0,0xFF), (0xFF,0,0), (0,0xFF,0)];

  var pixels = colorToPixel(colors, format=(rgbColor.blue, rgbColor.green, rgbColor.red));

  writeImage("pixels.jpg", imageType.jpg, pixels);
  writeImage("pixels.png", imageType.png, pixels);
  writeImage("pixels.bmp", imageType.bmp, pixels);


  // try and read the BMP as a JPG
  try {
    readImage("pixels.bmp", imageType.jpg);
    writeln("read BMP as JPG");
  } catch e: Error {
    writeln("Error reading BMP as JPG: ", e.message());
  }

  // try and read the BMP as a PNG
  try {
    readImage("pixels.bmp", imageType.png);
    writeln("read BMP as PNG");
  } catch e: Error {
    writeln("Error reading BMP as PNG: ", e.message());
  }

  // try and read the JPG as a PNG
  try {
    readImage("pixels.jpg", imageType.png);
    writeln("read JPG as PNG");
  } catch e: Error {
    writeln("Error reading JPG as PNG: ", e.message());
  }

  // try and read the PNG as a JPG
  try {
    readImage("pixels.png", imageType.jpg);
    writeln("read PNG as JPG");
  } catch e: Error {
    writeln("Error reading PNG as JPG: ", e.message());
  }


  // TODO: BMP reading is not yet supported
  // try and read the JPG as a BMP
  // try {
  //   readImage("pixels.jpg", imageType.bmp);
  //   writeln("read JPG as BMP");
  // } catch e: Error {
  //   writeln("Error reading JPG as BMP: ", e.message());
  // }
  // try and read the PNG as a BMP
  // try {
  //   readImage("pixels.png", imageType.bmp);
  //   writeln("read PNG as BMP");
  // } catch e: Error {
  //   writeln("Error reading PNG as BMP: ", e.message());
  // }

  // GOOD CASES
  // try and read the JPG as a JPG
  try {
    readImage("pixels.jpg", imageType.jpg);
    writeln("read JPG as JPG");
  } catch e: Error {
    writeln("Error reading JPG as JPG: ", e.message());
  }
  // try and read the PNG as a PNG
  try {
    readImage("pixels.png", imageType.png);
    writeln("read PNG as PNG");
  } catch e: Error {
    writeln("Error reading PNG as PNG: ", e.message());
  }
   // TODO: BMP reading is not yet supported
  // try and read the BMP as a BMP
  // try {
  //   readImage("pixels.bmp", imageType.bmp);
  //   writeln("read BMP as BMP");
  // } catch e: Error {
  //   writeln("Error reading BMP as BMP: ", e.message());
  // }

}
