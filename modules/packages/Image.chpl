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


A common convention taken by this module is to distinguish between arrays of
colors and arrays of pixels. An array of colors is a 2D array where each
element is a tuple of color values. An array of pixels is a 2D array where each
element is a single color value. The module provides functions
(:proc:`colorToPixel` and :proc:`pixelToColor`) to convert between these two
representations. Many functions, like :proc:`concatImage`, can work with either
representation.

*/
@unstable("Image is unstable")
module Image {

  private use IO, Math;


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
    bmp
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

  private inline proc checkFormat(format: 3*rgbColor) do
    if !(format[0] != format[1] &&
          format[1] != format[2] &&
          format[0] != format[2]) then
      halt("format must be a permutation of (red, green, blue)");

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

    if boundsChecking then checkFormat(format);

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
  */
  proc shuffleColors(colors: [] 3*pixelType, inFormat: 3*rgbColor, outFormat: 3*rgbColor = (rgbColor.red, rgbColor.green, rgbColor.blue)): colors.type
    where colors.isRectangular() && colors.rank == 2 {

    if boundsChecking {
      checkFormat(inFormat);
      checkFormat(outFormat);
    }

    var outColors: colors.type;
    forall (c, outC) in zip(colors, outColors) {
      const c0 = c[inFormat[0]:int];
      const c1 = c[inFormat[1]:int];
      const c2 = c[inFormat[2]:int];
      outC[outFormat[0]:int] = c0;
      outC[outFormat[1]:int] = c1;
      outC[outFormat[2]:int] = c2;
    }

    return outColors;
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


  // TODO: this is a simple nearest neighbor scaling. It would be nice to have
  // an enum to control the scaling algorithm. It would also be nice to be able
  // to scale down an image and use a non-integer factor.
  /*
    Scale a 2D array of pixels by a given factor

    :arg arr: the 2D array of pixels
    :arg factor: the factor to scale by
    :returns: a new array of pixels scaled by the factor
  */
  proc scale(arr: [?d], factor: int): []
    where d.isRectangular() && d.rank == 2 {

    if boundsChecking then
      if factor <= 0 then
        halt("factor must be greater than 0");

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

  /**/
  enum direction {
    /**/
    horizontal,
    /**/
    vertical
  }
  proc concatImage(pixels1: [] ?et,
                   pixels2: [] et,
                   dir: direction = direction.horizontal): [] et
    where pixels1.isRectangular() && pixels1.rank == 2 &&
          pixels2.isRectangular() && pixels2.rank == 2
    {
    const d1 = pixels1.domain, d2 = pixels2.domain;
    const newDom, d2Offset;
    select dir {
      when direction.vertical {
        if boundsChecking then
          if d1.dim(1).size != d2.dim(1).size then
            halt("pixels1 and pixels2 must have the same number of columns");

        newDom = {
          d1.dim(0).lowBound..#(d1.dim(0).size + d2.dim(0).size),
          d1.dim(1)
          };
        d2Offset = {
          (d1.dim(0).highBound+1)..#(d2.dim(0).size),
          d1.dim(1)
        };
      }
      when direction.horizontal {
        if boundsChecking then
          if d1.dim(0).size != d2.dim(0).size then
            halt("pixels1 and pixels2 must have the same number of rows");

        newDom = {
          d1.dim(0),
          d1.dim(1).lowBound..#(d1.dim(1).size + d2.dim(1).size)
        };
        d2Offset = {
          d1.dim(0),
          (d1.dim(1).highBound+1)..#(d2.dim(1).size)
        };
      }
      otherwise do halt("unknown direction");
    }

    var pixels: [newDom] et;
    pixels[d1] = pixels1;
    pixels[d2Offset] = pixels2;
    return pixels;
  }

  /**/
  enum cornerType {
    /**/
    topLeft,
    /**/
    topRight,
    /**/
    bottomLeft,
    /**/
    bottomRight,
    /**/
    center
  }
  /**/
  enum mergeMethod {
    /**/
    overwrite,
    /**/
    add
  }
  /*paste into base with left hand corner*/
  proc pasteInto(ref base: [?d1] ?et,
                 other: [?d2] et,
                 idx: 2*integral,
                 corner = cornerType.topLeft,
                 merge = mergeMethod.overwrite): [] et
    where base.isRectangular() && base.rank == 2 &&
          other.isRectangular() && other.rank == 2 {

    var pasteDom: domain(2);
    select corner {
      when cornerType.topLeft {
        pasteDom = {
          idx[0]..#(d2.dim(0).size),
          idx[1]..#(d2.dim(1).size)
        };
      }
      when cornerType.center {
        // TODO: this can result in incorrect bounds. why?
        private use Math;
        const radius0 = divFloor(d2.dim(0).size, 2);
        const radius1 = divFloor(d2.dim(1).size, 2);
        pasteDom = {
          (idx[0]-radius0)..#(d1.dim(0).size),
          (idx[1]-radius1)..#(d1.dim(1).size)
        };
      }
      otherwise do halt("unsupported cornerType");
    }

    select merge {
      when mergeMethod.overwrite {
        base[pasteDom] = other;
      }
      when mergeMethod.add {
        base[pasteDom] = other | base[pasteDom];
      }
      otherwise do halt("unknown mergeMethod");
    }

  }


  private proc concatArrays(arrays:[?dom]?elmType...?nArrays): [] elmType
    where dom.isRectangular() && dom.rank == 1 {
    var n: int;
    for param i in 0..#nArrays do
      n += arrays[i].domain.size;

    var outArr: [0..#n] elmType;

    var idx = 0;
    for param i in 0..#nArrays {
      outArr[idx..#arrays[i].domain.size] = arrays[i];
      idx += arrays[i].domain.size;
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

    proc init(image: string,
              imgType: imageType,
              framerate: int = 30,
              rateFactor: int = 1,
              verbose: bool = false) throws {
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
      const verboseArgs = if verbose
                            then ["-hide_banner"]
                            else ["-hide_banner", "-loglevel", "error"];
      var args = concatArrays(ffmpegStart, pixelFmtArgs, ffmpegEnd, verboseArgs);

      try {
        this.process = sp.spawn(args, stdin=sp.pipeStyle.pipe);
      } catch e: FileNotFoundError {
        halt("ffmpeg not found. Please install ffmpeg to use the mediaPipe");
      } catch e {
        throw e;
      }
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

}
