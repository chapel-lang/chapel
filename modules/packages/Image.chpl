/*
 * Copyright 2024-2024 Hewlett Packard Enterprise Development LP
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
The Image module provides a way to write arrays of pixels to an output image format.

For example, the following code creates a 3x3 array of pixels and writes it to
a BMP file. The array is then scaled by a factor of 2 (creating a 9x9 image)
and written to a second BMP file.

.. code-block:: chapel

   use Image;

   var color: [1..3, 1..3] 3*int;

   colors[1, ..] = [(0xFF,0,0), (0,0xFF,0), (0,0,0xFF)];
   colors[2, ..] = [(0,0xFF,0), (0,0,0xFF), (0xFF,0,0)];
   colors[3, ..] = [(0,0,0xFF), (0xFF,0,0), (0,0xFF,0)];

   var format = (rgbColor.blue, rgbColor.green, rgbColor.red);
   var arr = colorToPixel(color, format=format);

   writeImage("pixels.bmp", imageType.bmp, arr);
   writeImage("pixels2.bmp", imageType.bmp, scale(arr, 2));


*/
@unstable("Image is unstable")
module Image {

  private use IO, Math;

  private import this.PNGHelper;
  private import this.JPGHelper;


  //
  // Configuration params/types
  // (Override defaults on compiler line using -s<cfg>=<val>)
  //
  @chpldoc.nodoc
  config type pixelType = int;
  @chpldoc.nodoc
  config param bitsPerColor = 8;

  //
  // Define config-dependent types and params.
  //
  @chpldoc.nodoc
  param colorMask = (0x1 << bitsPerColor) - 1;
  @chpldoc.nodoc
  type colorType = uint(bitsPerColor);


  //
  // set helper params for colors
  //
  @chpldoc.nodoc
  enum rgbColor {
    red=0,
    green=1,
    blue=2,
  }

  //
  // Verify that config-specified pixelType is appropriate for storing colors
  //
  if isIntegral(pixelType) {
    if (rgbColor.size)*bitsPerColor > numBits(pixelType) then
      compilerError("pixelType '" + pixelType:string +
                    "' isn't big enough to store " +
                    bitsPerColor:string + " bits per color");
  } else {
    compilerError("pixelType must be an integral type");
  }

  //
  // how far to shift a color component when packing into a pixelType
  //
  private inline proc colorOffset(param color) param do
    return color:int * bitsPerColor;
  private inline proc colorOffset(color) do
    return color:int * bitsPerColor;

  /* Defines what kind of image to output */
  enum imageType {
    /* A BMP image is written from a 2D array of pixels */
    bmp,
    /* a PNG image is written from a 2D array of pixels */
    png,
    /* a JPG image is written from a 2D array of pixels */
    jpg,
  }

  /*
    Write an array of pixels as an image

    :arg image: the output filename or an open fileWriter
    :arg format: the image format
    :arg pixels: an array of pixel values
  */
  proc writeImage(image, format: imageType, pixels: [] pixelType) throws {
    // the output file channel
    const outfile = if isSubtype(image.type, fileWriter)
                      then image
                      else open(image, ioMode.cw).writer(locking=true);
    select format {
      when imageType.bmp do writeImageBMP(outfile, pixels);
      when imageType.png do PNGHelper.writePng(outfile, pixels);
      when imageType.jpg do JPGHelper.writeJpg(outfile, pixels);
      otherwise
        halt("Don't know how to write images of type: ", format);
    }
  }

  pragma "last resort"
  private proc writeImageBMP(outfile, pixels) do
    compilerError("pixels must be a 2D array of colors");

  private proc writeImageBMP(outfile, pixels: [?d]) throws
    where d.isRectangular() && d.rank == 2 {
    const rows = d.dim(0).size,
          cols = d.dim(1).size,
          headerSize = 14,
          dibHeaderSize = 40,  // always use old BITMAPINFOHEADER
          bitsPerPixel = (rgbColor.size)*bitsPerColor,
          // row size in bytes. Pad each row out to 4 bytes.
          rowQuads = divCeil(bitsPerPixel * cols, 32),
          rowSize = 4 * rowQuads,
          rowSizeBits = 8 * rowSize,
          pixelsSize = rowSize * rows,
          size = headerSize + dibHeaderSize + pixelsSize,
          offsetToPixelData = headerSize + dibHeaderSize;

    // Write the BMP image header
    outfile.writef("BM");
    outfile.writeBinary(size:uint(32), endianness.little);
    outfile.writeBinary(0:uint(16), endianness.little); /* reserved1 */
    outfile.writeBinary(0:uint(16), endianness.little); /* reserved2 */
    outfile.writeBinary(offsetToPixelData:uint(32), endianness.little);

    // Write the DIB header BITMAPINFOHEADER
    outfile.writeBinary(dibHeaderSize:uint(32), endianness.little);
    outfile.writeBinary(cols:int(32), endianness.little);
    outfile.writeBinary(-rows:int(32), endianness.little); /*neg for swap*/
    outfile.writeBinary(1:uint(16), endianness.little); /* 1 color plane */
    outfile.writeBinary(bitsPerPixel:uint(16), endianness.little);
    outfile.writeBinary(0:uint(32), endianness.little); /* no compression */
    outfile.writeBinary(pixelsSize:uint(32), endianness.little);
    outfile.writeBinary(2835:uint(32), endianness.little); /*pixels/meter print resolution=72dpi*/
    outfile.writeBinary(2835:uint(32), endianness.little); /*pixels/meter print resolution=72dpi*/
    outfile.writeBinary(0:uint(32), endianness.little); /* colors in palette */
    outfile.writeBinary(0:uint(32), endianness.little); /* "important" colors */

    for i in d.dim(0) {
      for j in d.dim(1) {
        var p = pixels[i,j];
        var redv = (p >> colorOffset(rgbColor.red)) & colorMask;
        var greenv = (p >> colorOffset(rgbColor.green)) & colorMask;
        var bluev = (p >> colorOffset(rgbColor.blue)) & colorMask;

        // write 24-bit color value
        outfile.writeBits(bluev, bitsPerColor);
        outfile.writeBits(greenv, bitsPerColor);
        outfile.writeBits(redv, bitsPerColor);
      }
      // write the padding.
      // The padding is only rounding up to 4 bytes so
      // can be written in a single writeBits call.
      const nbits = bitsPerColor * (rgbColor.size) * cols;
      outfile.writeBits(0:uint, (rowSizeBits-nbits):int(8));
    }
  }

  /*
    Takes a 2D array of color values and turns them into pixels. The order of
    the colors is determined by the ``format`` formal. The default format is
    ``(red, green, blue)``.

    :arg colors: the 2D array of colors
    :arg format: the order of the colors in the array. it must be a permutation
                 of ``(red, green, blue)``
    :returns: a new array of pixels with the same domain as ``colors``
  */
  proc colorToPixel(colors: [?d] 3*pixelType, format: 3*rgbColor = (rgbColor.red, rgbColor.green, rgbColor.blue)): [d] pixelType
    where d.isRectangular() && d.rank == 2 {

    // check format
    if boundsChecking {
      if !(format[0] != format[1] &&
           format[1] != format[2] &&
           format[0] != format[2]) then
        halt("colorToPixel: format must be a permutation of (red, green, blue)");
    }

    proc getColorAsPixel(color: pixelType, offset: rgbColor) {
      return (color & colorMask) << colorOffset(offset);
    }

    var outPixels: [d] pixelType;
    forall (c, outPixel) in zip(colors, outPixels) {
      outPixel = getColorAsPixel(c[0], format[0]) |
                 getColorAsPixel(c[1], format[1]) |
                 getColorAsPixel(c[2], format[2]);
    }

    return outPixels;
  }

  /*
    Takes a 2D array of some element type and computes an integer color
    gradient between the newMin/Max. This is a simple interpolation of the
    values in the array to the new range.

    :arg arr: the 2D array of values to sample
    :arg newMin: the new minimum value
    :arg newMax: the new maximum value
    :returns: a new array of pixels with the same domain as ``arr``
  */
  proc downSample(arr: [?d], newMin:int = 0, newMax:int = 0xFFFFFF): [d] int
    where d.isRectangular() && d.rank == 2 {
    const (oldMin, oldMax) = minmax reduce arr;

    var outArr: [d] int =
      ((newMax - newMin) * ((arr - oldMin) / (oldMax - oldMin)) + newMin):int;

    return outArr;
  }

  // workaround https://github.com/chapel-lang/chapel/issues/25045
  private proc interpolateColorDefaultColorRange(arr: []) do return minmax reduce arr;

  /*
  Linearly interpolates between two colors to create an array of pixels.
  */
  proc interpolateColor(arr: [?d],
                        colorA: pixelType,
                        colorB: pixelType,
                        colorRange = interpolateColorDefaultColorRange(arr)): [d] pixelType {
    const (low, high) = colorRange;
    const spread = (high - low):real;

    proc colorComponent(color: pixelType, param offset: rgbColor) do
      return (color >> colorOffset(offset)) & colorMask;
    proc linInterp(t:real, l, h) do return (l * (1 - t) + h * t):int;

    proc computeColor(t:real, param rgb: rgbColor) {
      const v = linInterp(t,
                          colorComponent(colorA, rgb),
                          colorComponent(colorB, rgb)) & colorMask;
      return v << colorOffset(rgb);
    }

    var outPixels: [d] pixelType;
    forall (a, outPixel) in zip(arr, outPixels) {
      const t = (a - low):real / spread;
      outPixel = computeColor(t, rgbColor.red) |
                 computeColor(t, rgbColor.green) |
                 computeColor(t, rgbColor.blue);
    }

    return outPixels;
  }


  /*
    Scale a 2D array of pixels by a given factor

    :arg arr: the 2D array of pixels
    :arg factor: the factor to scale by
    :returns: a new array of pixels scaled by the factor
  */
  proc scale(arr: [?d], factor: int): []
    where d.isRectangular() && d.rank == 2 {
    var rows = d.dim(0).size,
        cols = d.dim(1).size;

    var outArr: [d.dim(0).lowBound..#rows*factor, d.dim(1).lowBound..#cols*factor] arr.eltType;

    const outArrSpace = {d.dim(0).lowBound..by factor #rows,
                         d.dim(1).lowBound..by factor #cols};
    forall (arrIdx, outArrIdx) in zip(d, outArrSpace) {
      outArr[outArrIdx[0]..#factor, outArrIdx[1]..#factor] = arr[(...arrIdx)];
    }

    return outArr;
  }


  private import Subprocess as sp;
  /*
  Represents a ffmpeg stream that frame data can be written to to produce a mp4.
  */
  record mediaPipe {
    @chpldoc.nodoc
    var imageName: string;
    @chpldoc.nodoc
    var imgType: imageType;
    @chpldoc.nodoc
    var finished = false;
    @chpldoc.nodoc
    var process: sp.subprocess(true);

    proc init(image: string, imgType: imageType, framerate: int = 30, rateFactor: int = 1) throws {
      this.imageName = image;
      this.imgType = imgType;
      init this;

      const ffmpegStart = ["ffmpeg", "-y",
                           "-f", "image2pipe",
                           "-r", framerate:string];
      const pixelFmtArgs;
      select imgType {
        when imageType.bmp do
          pixelFmtArgs = ["-pix_fmt", "bgr24", "-c:v", "bmp"];
        otherwise {
          halt("Don't know how to write images of type: ", imgType);
          pixelFmtArgs = [""]; // dummy value for split init
        }
      }
      const ffmpegEnd = ["-i", "-",
                         "-c:v", "libx264",
                         "-crf", rateFactor:string,
                         "-pix_fmt", "yuv420p",
                         this.imageName];
      const nArgs = ffmpegStart.domain.size + pixelFmtArgs.domain.size + ffmpegEnd.domain.size;
      var args: [0..#nArgs] string;
      args[0..#ffmpegStart.domain.size] = ffmpegStart;
      args[ffmpegStart.domain.size..#pixelFmtArgs.domain.size] = pixelFmtArgs;
      args[(ffmpegStart.domain.size + pixelFmtArgs.domain.size)..#ffmpegEnd.size] = ffmpegEnd;

      this.process = sp.spawn(args, stdin=sp.pipeStyle.pipe);
    }
    proc ref deinit() {
      try! finish();
    }

    /*
    Write a frame into the media pipe.
    */
    proc writeFrame(pixelData: [] pixelType) throws {
      writeImage(process.stdin, imageType.bmp, pixelData);
    }
    proc ref finish() throws {
      if !finished {
        process.wait();
        finished = true;
      }
    }
  }

  @chpldoc.nodoc
  module PNGHelper {
    enum PNGImpl {
      stb_image,
      libpng
    }
    config param pngImpl = PNGImpl.stb_image;
    proc writePng(outfile, pixels: [?d]) {
      if pngImpl == PNGImpl.stb_image {
        import super.STBImageHelper;
        STBImageHelper.writePng(outfile, pixels);
      } else if pngImpl == PNGImpl.libpng {
        compilerError("libpng is not yet supported");
      } else {
        compilerError("Unknown PNG implementation");
      }
    }
  }

  @chpldoc.nodoc
  module JPGHelper {
    enum JPGImpl {
      stb_image
    }
    config param jpgImpl = JPGImpl.stb_image;
    proc writeJpg(outfile, pixels: [?d]) {
      if jpgImpl == JPGImpl.stb_image {
        import super.STBImageHelper;
        STBImageHelper.writeJpg(outfile, pixels);
      } else {
        compilerError("Unknown JPG implementation");
      }
    }
  }

  @chpldoc.nodoc
  module STBImageHelper {
    require "ImageHelper/stb_image_write_helper.h";

    private use CTypes;
    private use IO;
    private use Image;

    /*private*/ type stbi_write_func = c_fn_ptr;
    private extern proc stbi_write_png_to_func(func: stbi_write_func,
                               context: c_ptr(void),
                               w: c_int,
                               h: c_int,
                               comp: c_int,
                               data: c_ptrConst(void),
                               stride_in_bytes: c_int): c_int;

    private extern proc stbi_write_jpg_to_func(func: stbi_write_func,
                               context: c_ptr(void),
                               w: c_int,
                               h: c_int,
                               comp: c_int,
                               data: c_ptrConst(void),
                               quality: c_int): c_int;

    private proc stbi_writeFuncLocking(context: c_ptr(void),
                             data: c_ptrConst(void),
                             size: c_int) {
      const outfile = (context:c_ptr(fileWriter(locking=true))).deref();
      try! outfile.writeBinary(data:c_ptrConst(uint(8)):c_ptr(uint(8)), size);
    }
    private proc stbi_writeFunc(context: c_ptr(void),
                             data: c_ptrConst(void),
                             size: c_int) {
      const outfile = context:c_ptr(fileWriter(locking=false));
      try! outfile.writeBinary(data:c_ptrConst(uint(8)), size);
    }

    // rewrite pixels into the right format for stb_image
    private proc writeCommon(outfile, pixels: [?dom]) {
      const rows = dom.dim(0).size, cols = dom.dim(1).size;

      // 1=Y, 2=YA, 3=RGB, 4=RGBA
      const mode: c_int = 3;
      const width = cols;
      const height = rows;
      const nBytes = width * height * mode;
      var data = allocate(uint(8), nBytes);
      forall idx in 0..<(rows*cols) {
        const (i,j) = dom.orderToIndex(idx);
        const offset = idx * mode;
        const p = pixels[i, j];
        data[offset] = ((p >> colorOffset(rgbColor.red)) & colorMask):uint(8);
        data[offset + 1] = ((p >> colorOffset(rgbColor.green)) & colorMask):uint(8);
        data[offset + 2] = ((p >> colorOffset(rgbColor.blue)) & colorMask):uint(8);
      }

      const writeFunc = if outfile.locking
                          then c_ptrTo(stbi_writeFuncLocking)
                          else c_ptrTo(stbi_writeFunc);

      const context = c_ptrToConst(outfile);
      return (writeFunc, context, width.safeCast(c_int), height.safeCast(c_int), mode, data);
    }

    proc writePng(outfile, pixels: [?dom]) {
      const (writeFunc, context, width, height, mode, data) = writeCommon(outfile, pixels);
      const stride = width * mode;
      stbi_write_png_to_func(writeFunc, context, width, height, mode, data, stride);
    }

    proc writeJpg(outfile, pixels: [?dom]) {
      const (writeFunc, context, width, height, mode, data) = writeCommon(outfile, pixels);
      const quality: c_int = 100;
      stbi_write_jpg_to_func(writeFunc, context, width, height, mode, data, quality);
    }

  }

}
