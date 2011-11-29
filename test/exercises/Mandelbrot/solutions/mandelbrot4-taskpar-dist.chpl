// mandelbrot4-taskpar-dist.chpl: Distributed task-parallel implementation
//

use MPlot;

//
// Dimensions of image file
//
config const rows = 201,
             cols = rows;

//
// Maximum number of steps to iterate
//
config const maxSteps = 50;

// 
// The number of tasks we should spawn.
//
config const tasksPerLocale = 1;


proc main() {
  // The set of indices over which the image is defined.
  var ImgSpace = [0..#rows, 0..#cols];

  //
  // An array representing the number of iteration steps taken in the
  // calculation (effectively, the image)
  //
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


//
// Compute the pixel value as described in the handout
//
proc compute((x, y)) {
  const c = mapImg2CPlane((x, y));
  
  var z: complex;
  for i in 1..maxSteps {
    z = z*z + c;
    if (abs(z) > 2.0) then
      return i;
  }
  return 0;			
}


// Map an image coordinate to a point in the complex plane.
// Image coordinates are (row, col), with row 0 at the top.
proc mapImg2CPlane((row, col)) {
  const (rmin, rmax) = (-1.5, .5);
  const (imin, imax) = (-1i, 1i);

  return ((rmax - rmin) * col / cols + rmin) +
         ((imin - imax) * row / rows + imax);
}

