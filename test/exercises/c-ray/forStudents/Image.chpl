//
// Configuration params/types
// (Override defaults on compiler line using -s<cfg>=<val>)
//
config type pixelType = int;

config param bitsPerColor = 8;

//
// Define config-dependent types and params.
//
param colorMask = (0x1 << bitsPerColor) - 1;

type colorType = uint(bitsPerColor);


//
// set helper params for colors
//
param red = 1,        // names for referring to colors
      green = 2,
      blue = 3,
      numColors = 3;

//
// Verify that config-specified pixelType is appropriate for storing colors
//
if (isIntegral(pixelType)) {
  if (numColors*bitsPerColor > numBits(pixelType)) then
    compilerError("pixelType '" + pixelType:string +
                  "' isn't big enough to store " +
                  bitsPerColor + " bits per color");
} else {
  compilerError("pixelType must be an integral type");
}

//
// supported image types
//
enum imageType {ppm, bmp};

//
// Detect the image format based on the extension of 'filename'.
// Default to .ppm if using 'stdout'
//
proc extToFmt(filename) {
  if filename == "stdout" then {
    stderr.writeln("Using .ppm format for 'stdout' (override with --format)");
    return imageType.ppm;
  } else {
    for ext in imageType do
      if filename.toLower().endsWith(ext:string) then
        return ext;
  }

  halt("Unsupported image format for output file '", filename, "'");
}

//
// how far to shift a color component when packing into a pixelType
//
inline proc colorOffset(param color) param {
  return (color - 1) * bitsPerColor;
}

//
// write the image to the output file
//
proc writeImage(outfile, format, pixels: [] pixelType) {
  select format {
    when imageType.ppm do
      writeImagePPM(outfile, pixels);
    when imageType.bmp do
      writeImageBMP(outfile, pixels);
    otherwise
      halt("Don't know how to write images of type: ", format);
  }
}

//
// write the image as a PPM file (a simple file format, but not always
// the best supported)
//
proc writeImagePPM(outfile, pixels) {
  outfile.writeln("P6");
  outfile.writeln(pixels.domain.dim(2).size, " ", pixels.domain.dim(1).size);
  outfile.writeln(255);
  for p in pixels do
    for param c in 1..numColors do
      outfile.writef("%|1i", ((p >> colorOffset(c)) & colorMask));
}

//
// write the image as a BMP file (a more complex file format, but much
// more portable)
//
proc writeImageBMP(outfile, pixels) {
  const rows = pixels.domain.dim(1).length,
        cols = pixels.domain.dim(2).length,

        headerSize = 14,
        dibHeaderSize = 40,  // always use old BITMAPINFOHEADER
        bitsPerPixel = numColors*bitsPerColor,

        // row size in bytes. Pad each row out to 4 bytes.
        rowQuads = divceil(bitsPerPixel * cols, 32),
        rowSize = 4 * rowQuads,
        rowSizeBits = 8 * rowSize,

        pixelsSize = rowSize * rows,
        size = headerSize + dibHeaderSize + pixelsSize,

        offsetToPixelData = headerSize + dibHeaderSize;

  // Write the BMP image header
  outfile.writef("BM%<4u %<2u %<2u %<4u",
                 size,
                 0 /* reserved1 */,
                 0 /* reserved2 */,
                 offsetToPixelData);

  // Write the DIB header BITMAPINFOHEADER
  outfile.writef("%<4u %<4i %<4i %<2u %<2u %<4u %<4u %<4u %<4u %<4u %<4u",
                 dibHeaderSize, cols, -rows /*neg for swap*/,
                 1 /* 1 color plane */, bitsPerPixel,
                 0 /* no compression */,
                 pixelsSize,
                 2835, 2835 /*pixels/meter print resolution=72dpi*/,
                 0 /* colors in palette */,
                 0 /* "important" colors */);

  for i in pixels.domain.dim(1) {
    var nbits = 0;
    for j in pixels.domain.dim(2) {
      var p = pixels[i,j];
      var redv = (p >> colorOffset(red)) & colorMask;
      var greenv = (p >> colorOffset(green)) & colorMask;
      var bluev = (p >> colorOffset(blue)) & colorMask;

      // write 24-bit color value
      outfile.writebits(bluev, bitsPerColor);
      outfile.writebits(greenv, bitsPerColor);
      outfile.writebits(redv, bitsPerColor);
      nbits += numColors * bitsPerColor;
    }
    // write the padding.
    // The padding is only rounding up to 4 bytes so
    // can be written in a single writebits call.
    outfile.writebits(0:uint, (rowSizeBits-nbits):int(8));
  }
}
