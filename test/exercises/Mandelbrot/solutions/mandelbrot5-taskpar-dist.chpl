use BlockDist;

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
// Dimensions of image file
//
config const xsize = 200,
             ysize = xsize;

//
// Maximum color depth for image file
//
config const maxColor = 15;

//
// Maximum number of steps to iterate
//
config const maxSteps = 50;

// 
// The number of tasks we should spawn.
//
config const tasksPerLocale = 1;


proc main() {
  //
  // domains and arrays representing the number of steps taken in the
  // calculation (effectively, the image).  Note that Block is
  // actually not a very good choice for Mandelbrot; Cyclic should be
  // better.
  //
  var ImgSpace = [0..#xsize, 0..#ysize];
  var NumSteps: [ImgSpace] int;

  //
  // Compute the image
  //

  // We partition the work among locales (in vertical stripes)
  // and tasks (in horizontal stripes).
  coforall tid in 0..#tasksPerLocale do
    coforall loc in Locales do
      on loc do
        // This performs a portion of the computation, based on the locale and task IDs.
        blockCompute(tid, tasksPerLocale, NumSteps);

  // Plot the image
  plot(NumSteps);
}


// Perform the computation a portion of the image plane 
// assigned to the given task/taskID.
proc blockCompute(taskId, taskCount, NumSteps: [?D] int)
{
  // We can reuse blockbound for both horizontal and vertical partitioning
  // We pass in a range instead of a domain.
  proc blockbound(id, count, r) {
    // This function returns a partition boundary in the first dimension of the domain
    // used to define the array NumSteps.
    // The return value is the first index in the given partition.
    // The end of a given partition is the start of the next partition minus one.
    var xlow = r.low;
    var xlim = r.high + 1;
    var xspan = xlim - xlow;

    // Special case: Task IDs are number from 0 to taskCount-1, 
    // so if taskCount is passed in, return one more than the high index in dim(1).
    // The ensures that we visit every index, even if xspan is not evenly divisible by count.
    if id == count then return xlim;

    return xlow + id * xspan / count;
  }

  // Get the range of rows assigned to this task.
  var myRowRange = blockbound(taskId, taskCount, D.dim(1))..
    (blockbound(taskId+1, taskCount, D.dim(1)) - 1);
  var myColRange = blockbound(here.id, numLocales, D.dim(2))..
    (blockbound(here.id + 1, numLocales, D.dim(2)) - 1);

  // Iterate over my subset of rows and columns.
  for i in myRowRange do
    for j in myColRange do
      NumSteps(i, j) = compute((i, j));
}


proc compute((x, y)) {
  const c = ((2.0 * y) / xsize - 1.5) + ((2i * x) / ysize - 1i);
  
  var z: complex = 0i;
  for i in 1..maxSteps {
    z = z*z + c;
    if (abs(z) >= 2.0) then
      return i;
  }
  return 0;			
}


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
  const outfile = open(outfilename, mode.w).writer();

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
