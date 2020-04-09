config const half_width = 39;
config const half_height = 11;
config const iterations = 50;

proc main() {
  var grid_space: domain(2) =
    {-half_height..half_height, -half_width..half_width};

  var grid: [grid_space] bool;

  forall (i, j) in grid.domain do
    grid(i, j) = in_mandelbrot_set((j:real/half_width)*2.0,
                                   (i:real/half_height)*1.0);

  // Print the set
  for (i, j) in grid.domain {
    if (grid(i,j)) then
      write('#');
    else
      write(' ');
    if (j == half_width) then
      write('\n');
  }
}

proc in_mandelbrot_set(real_part, imag_part) {
  // Returns true if the complex number real_part + i*imag_part
  // is in the Mandelbrot set.  Returns false otherwise.
  var start, z: complex;
  start.re = real_part;
  start.im = imag_part;
  z = start;

  for i in 1..iterations {
    z = z * z + start;
    if (z.re**2 + z.im**2 > 4) then
      break;
  }
  if (z.re**2 + z.im**2 > 4) then
    return false;
  else
    return true;
}

