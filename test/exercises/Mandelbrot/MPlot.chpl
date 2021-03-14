// MPlot.chpl
//
// Plotting routines for the Mandelbrot exercise.
//

//
// Types of image files: Black & White, Greyscale, or Color.  The integer
// values chosen here represent the header encoding for PBM, PGM, and
// PPM files, respectively.
//
enum imageType { bw=1, grey, color};

//
// User-specifiable image type to produce; defaults to color
//
config const imgType = imageType.color;

//
// Base filename for output file
//
config const filename = "mandelbrot";

//
// Format for the output file
//
config var format = "bmp";

//
// Maximum color depth for image file
//
config const maxColor = 15;

//
// This routine will plot a rectangular array with any coordinate
// system to the output file specified by the filename config const.
//
proc plot(NumSteps:[]) where NumSteps.rank == 2 {
  use IO;

  //
  // An array mapping from the image type enum to file extensions.
  //
  const extensions: [imageType.bw..imageType.color] string = ("pbm", "pgm", "ppm");

  if format == "pbm" || format == "pgm" || format == "ppm" {
    format = extensions(imgType);
  }

  //
  // Compute the full output filename and open the file and a writer
  // to it.
  //
  const outfilename = filename + "." + format;
  const outfile = open(outfilename, iomode.cw).writer();

  //
  // Plot the image to the file (could also pass stdout in as the file...)
  //
  if format == "bmp" then plotToFileBMP(NumSteps, outfile);
  else plotToFilePPM(NumSteps, outfile);

  //
  // Close file and tell user what it was called
  //
  outfile.close();
  writeln("Wrote ", imgType, " output to ", outfilename);
}


//
// This is a helper routine that plots to an outfile 'channel'; this
// channel could simply be stdout or some other channel instead of an
// actual file channel.
//
proc plotToFilePPM(NumSteps: [?Dom], outfile) {
  //
  // Capture the number of rows and columns in the array to be plotted
  //
  const rows = Dom.dim(0).size,
        cols = Dom.dim(1).size;

  //
  // Write the image header corresponding to the file type
  //
  outfile.writeln("P", imgType:int);
  outfile.writeln(rows, " ", cols);

  //
  // For file types other than greyscale, we have to write the number
  // of colors we're using
  //
  if (imgType != imageType.bw) then
    outfile.writeln(maxColor);

  //
  // compute the maximum number of steps that were taken, just in case
  // it wasn't the user-supplied cutoff.
  //
  const maxSteps = max reduce NumSteps;
  assert(maxSteps != 0, "NumSteps contains no positive values");

  //
  // Write the output data.  Though verbose, we use three loop nests
  // here to avoid extra conditionals in the inner loop.
  //
  select (imgType) {
    when imageType.bw {
      for i in Dom.dim(0) {
        for j in Dom.dim(1) {
          outfile.write(if NumSteps[i,j] then 0 else 1, " ");
        }
        outfile.writeln();
      }
    }

    when imageType.grey {
      for i in Dom.dim(0) {
        for j in Dom.dim(1) {
          outfile.write((maxColor*NumSteps[i,j])/maxSteps, " ");
        }
        outfile.writeln();
      }
    }

    when imageType.color {
      for i in Dom.dim(0) {
        for j in Dom.dim(1) {
          outfile.write((maxColor*NumSteps[i,j])/maxSteps, " ", 0, " ", 0, " ");
        }
        outfile.writeln();
      }
    }
  }
}

//
// This is a helper routine that plots a BMP to an outfile 'channel'; this
// channel could simply be stdout or some other channel instead of an
// actual file channel.
//
proc plotToFileBMP(NumSteps: [?Dom], outfile) {
  //
  // Capture the number of rows and columns in the array to be plotted
  //
  const rows = Dom.dim(0).size,
        cols = Dom.dim(1).size;

const header_size = 14;
  const dib_header_size = 40;  // always use old BITMAPINFOHEADER
  const   bits_per_pixel = 24;

  // row size in bytes. Pad each row out to 4 bytes.
  const row_quads = (bits_per_pixel * cols + 31) / 32;
  const row_size = 4 * row_quads;
  const row_size_bits = 8 * row_size;

  const pixels_size = row_size * rows;
  const size = header_size + dib_header_size + pixels_size;

  const offset_to_pixel_data = header_size + dib_header_size;

  // Write the BMP image header
  outfile.writef("BM%<4u %<2u %<2u %<4u",
                 size,
                 0 /* reserved1 */,
                 0 /* reserved2 */,
                 offset_to_pixel_data);
  
  // Write the DIB header BITMAPINFOHEADER
  outfile.writef("%<4u %<4i %<4i %<2u %<2u %<4u %<4u %<4u %<4u %<4u %<4u",
                 dib_header_size, cols, -rows /*neg for swap*/,
                 1 /* 1 color plane */, bits_per_pixel,
                 0 /* no compression */,
                 pixels_size,
                 2835, 2835 /*pixels/meter print resolution=72dpi*/,
                 0 /* colors in palette */,
                 0 /* "important" colors */);

  //
  // compute the maximum number of steps that were taken, just in case
  // it wasn't the user-supplied cutoff.
  //
  const maxSteps = max reduce NumSteps;
  assert(maxSteps != 0, "NumSteps contains no positive values");

  //
  // Write the output data.  Though verbose, we use three loop nests
  // here to avoid extra conditionals in the inner loop.
  //
  select (imgType) {
    when imageType.bw {
      for i in Dom.dim(0) {
        var nbits = 0;
        for j in Dom.dim(1) {
          var bit = (if NumSteps[i,j] then 255 else 0):uint;
          outfile.writebits(bit, 8);
          outfile.writebits(bit, 8);
          outfile.writebits(bit, 8);
          nbits += 24;
        }
        // write the padding.
        // The padding is only rounding up to 4 bytes so
        // can be written in a single writebits call.
        outfile.writebits(0:uint, (row_size_bits-nbits):int(8));
      }
    }

    when imageType.grey {
      for i in Dom.dim(0) {
        var nbits = 0;
        for j in Dom.dim(1) {
          var grey = ((255*NumSteps[i,j])/maxSteps):uint;
          // write 24-bit color value by repeating grey
          outfile.writebits(grey, 8);
          outfile.writebits(grey, 8);
          outfile.writebits(grey, 8);
          nbits += 24;
        }
        // write the padding.
        // The padding is only rounding up to 4 bytes so
        // can be written in a single writebits call.
        outfile.writebits(0:uint, (row_size_bits-nbits):int(8));
      }
    }

    when imageType.color {
      for i in Dom.dim(0) {
        var nbits = 0;
        for j in Dom.dim(1) {
          var green:uint = 0;
          var blue:uint = 0;
          var red = ((255*NumSteps[i,j])/maxSteps):uint;
          // write 24-bit color value
          outfile.writebits(blue, 8);
          outfile.writebits(green, 8);
          outfile.writebits(red, 8);
          nbits += 24;
        }
        // write the padding.
        // The padding is only rounding up to 4 bytes so
        // can be written in a single writebits call.
        outfile.writebits(0:uint, (row_size_bits-nbits):int(8));
      }
    }
  }
}
