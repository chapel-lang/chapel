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
// Maximum color depth for image file
//
config const maxColor = 15;

//
// This routine will plot a rectangular array with any coordinate
// system to the output file specified by the filename config const.
//
proc plot(NumSteps:[]) where NumSteps.rank == 2 {
  //
  // An associative domain+array mapping from the image type enum to
  // file extensions.  Note that associative domains over enumerated
  // types are fully-populated by default.
  //
  const extensionSpace: domain(imageType);
  const extensions: [extensionSpace] string = (".pbm", ".pgm", ".ppm");

  //
  // Compute the full output filename and open the file and a writer
  // to it.
  //
  const outfilename = filename + extensions(imgType);
  const outfile = open(outfilename, iomode.cw).writer();

  //
  // Plot the image to the file (could also pass stdout in as the file...)
  //
  plotToFile(NumSteps, outfile);

  //
  // Close file and tell user what it was called
  //
  outfile.close();
  writeln("Wrote output to ", outfilename);
}


//
// This is a helper routine that plots to an outfile 'channel'; this
// channel could simply be stdout or some other channel instead of an
// actual file channel.
//
proc plotToFile(NumSteps: [?Dom], outfile) {
  //
  // Capture the number of rows and columns in the array to be plotted
  //
  const rows = Dom.dim(1).length,
        cols = Dom.dim(2).length;

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

  //
  // Write the output data.  Though verbose, we use three loop nests
  // here to avoid extra conditionals in the inner loop.
  //
  select (imgType) {
    when imageType.bw {
      for i in Dom.dim(1) {
        for j in Dom.dim(2) {
          outfile.write(if NumSteps[i,j] then 0 else 1, " ");
        }
        outfile.writeln();
      }
    }

    when imageType.grey {
      for i in Dom.dim(1) {
        for j in Dom.dim(2) {
          outfile.write((maxColor*NumSteps[i,j])/maxSteps, " ");
        }
        outfile.writeln();
      }
    }

    when imageType.color {
      for i in Dom.dim(1) {
        for j in Dom.dim(2) {
          outfile.write((maxColor*NumSteps[i,j])/maxSteps, " ", 0, " ", 0, " ");
        }
        outfile.writeln();
      }
    }
  }
}
