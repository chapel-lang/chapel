// mandelbrot3-taskpar.chpl
//
// Solution for Exercise 3: Mandelbrot Set -- Task parallel implementation.
//
// compile command: chpl mandelbrot3-taskpar.chpl MPlot.chpl -o mand
// execute command: mand
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
config const numTasks = here.maxTaskPar; // tasking layer max parallelism


proc main() {
  // The set of indices over which the image is defined.
  var ImgSpace = {0..#rows, 0..#cols};

  //
  // An array representing the number of iteration steps taken in the
  // calculation (effectively, the image)
  //
  var NumSteps: [ImgSpace] int;

  //
  // Compute the image
  //

  // We explicitly partition the work and send it to separate tasks.
  coforall tid in 0..#numTasks do
    // This performs a portion of the computation, based on the task ID.
    blockCompute(tid, numTasks, NumSteps);

  // Plot the image
  plot(NumSteps);
}


// Perform the computation a portion of the image plane 
// assigned to the given task/taskID.
proc blockCompute(taskId, taskCount, NumSteps: [?D] int)
{
  // Let's choose a simple partitioning: horizontal stripes == sets of rows.
  proc blockbound(id, count) {
    // This function returns a partition boundary in the first dimension of the domain
    // used to define the array NumSteps.
    // The return value is the first index in the given partition.
    // The end of a given partition is the start of the next partition minus one.
    var xlow = D.dim(1).low;
    var xlim = D.dim(1).high + 1;
    var xspan = xlim - xlow;

    // Special case: Task IDs are number from 0 to taskCount-1, 
    // so if taskCount is passed in, return one more than the high index in dim(1).
    // The ensures that we visit every index, even if xspan is not evenly divisible by count.
    if id == count then return xlim;

    return xlow + id * xspan / count;
  }

  // Get the range of rows assigned to this task.
  var myRowRange = blockbound(taskId, taskCount)..(blockbound(taskId+1, taskCount) - 1);

  // Iterate over my subset of rows and all columns.
  for i in myRowRange do
    for j in D.dim(2) do
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

