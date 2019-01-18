config const seed: uint = 113579,
             arraySize = 101;

proc main () {
  writeln ("Before shadowing, arraySize = ", arraySize);
  use RandomNumber;
  //var arraySize = 32;
  if arraySize % 2 != 0 then
    halt ("arraySize must be even!");
  // Each point consists of two coordinates.
  const numberOfPoints = arraySize/2;
  var r = Random (),
      rArray: [1..arraySize] real;
  ref x = rArray[1..numberOfPoints],
      y = rArray[numberOfPoints+1 ..];
  // Fill rArray with random numbers between 0 and 1.
  rArray = r.RandomNumber(arraySize);
  // Count how many points fall within the unit circle.
  var count = + reduce x**2 + y**2 <= 1.0;
  if arraySize <= 30 then
    writeln ("Generated points:\n", rArray);
  writeln ("Number of points: ", numberOfPoints);
  // The area of a circle is pi * r**2.  In this case, r = 1
  // because it's the unit circle, and all generated points
  // are in a quadrant of that circle, so the ratio of points
  // inside the circle versus all generated points should be
  // close to one-fourth pi.
  writeln ("Estimate of pi: ", count*4 / numberOfPoints : real);
}
