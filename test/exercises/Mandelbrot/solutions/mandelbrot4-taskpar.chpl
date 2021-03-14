// mandelbrot4-taskpar.chpl: Task parallel implementation
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
config const numTasks = here.maxTaskPar;


proc main() {
  //
  // The set of indices over which the image is defined.  Note that
  // 0..#n means "a range with n indices starting at 0", i.e., 0..n-1
  //
  var ImgSpace = {0..#rows, 0..#cols};

  //
  // An array to store the resulting Image.
  //
  var Image: [ImgSpace] int;

  //
  // Compute the image, using task parallelism
  //
  coforall tid in 0..#numTasks do  // Create a number of sibling tasks
    //
    // Each will perform a portion of the computation, based on the task ID.
    //
    blockCompute(tid, numTasks, Image);

  //
  // Plot the image
  //
  plot(Image);
}

//
// Perform the computation of a portion of the image plane
// assigned to the given task/taskID.  We'll use a simple partitioning:
// decompose rows to tasks by blocks.
//
proc blockCompute(taskId, taskCount, Image: [?D] int) {
  //
  // Get the range of rows assigned to this task.
  //
  var myRowRange = blockbound(taskId,   taskCount, D.dim(0))..
                  (blockbound(taskId+1, taskCount, D.dim(0)) - 1);

  //
  // Iterate over my subset of rows and all columns.
  //
  for i in myRowRange do
    for j in D.dim(1) do
      Image[i, j] = compute(i, j);
}

//
// This function helps block a range 'r' between 'count' tasks.
// It returns the first index for the given task in the partition.
// The end of a given partition is the start of the next partition minus one.
//
proc blockbound(id, count, r) {
  var xlow = r.low;
  var xlim = r.high + 1;
  var xspan = xlim - xlow;

  //
  // Special case: Task IDs are numbered from 0 to taskCount-1, 
  // so if 'taskCount' is passed in, return one more than the high index in dim(0).
  // This ensures that we visit every index, even if xspan is not
  // evenly divisible by count.
  //
  if id == count then return xlim;

  return xlow + id * xspan / count;
}

//
// Compute the pixel value as described in the handout
//
proc compute(x, y) {
  const c = mapImg2CPlane(x, y);  // convert the pixel coordinates to a complex value

  var z: complex;
  for i in 1..maxSteps {
    z = z*z + c;
    if (abs(z) > 2.0) then
      return i;
  }

  return 0;			
}

//
// Map an image coordinate to a point in the complex plane.
// Image coordinates are (row, col), with row 0 at the top.
//
proc mapImg2CPlane(row, col) {
  const (rmin, rmax) = (-1.5, .5);
  const (imin, imax) = (-1i, 1i);

  return ((rmax - rmin) * col / cols + rmin) +
         ((imin - imax) * row / rows + imax);
}
