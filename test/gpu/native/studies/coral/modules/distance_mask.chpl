module distance_mask {

proc create_distance_mask(radius : real, dx : real, nx : int(16)) {

  const D : domain(2, int(16)) = {-nx..nx, -nx..nx};
  var D_center : sparse subdomain(D);
  var D_left : sparse subdomain(D);
  var D_right : sparse subdomain(D);

  var dist : [D] real;

  var center_mask : [D_center] bool;
  var left_mask : [D_left] bool;
  var right_mask : [D_right] bool;

  // Set default value of sparse arrays to be true.
  center_mask.IRV = true;
  left_mask.IRV = true;
  right_mask.IRV = true;

  // Define center mask. Using < here instead of <= because <= leaves only one point at the edge of the
  // domain, and it becomes difficult to define the left and right masks in a sensible way.
  for (i,j) in dist.domain do {
    dist[i,j] = dx * sqrt(i**2 + j**2);
    if (dist[i,j] < radius) {
      D_center += (i,j);
    }
  }

  // Cefine left and right masks.
  // "left" will represent the left edge of the circle,
  // and "right" will represent the right edge

  // left
  for (i,j) in dist.domain do {
    if ( D_center.contains((i,j)) && (j == -nx || !D_center.contains((i,j-1))) ) {
      D_left += (i,j);
    }
  }

  // right
  for (i,j) in dist.domain do {
    if ( D_center.contains((i,j)) && (j == nx || !D_center.contains((i,j+1))) ) {
      D_right += (i,j);
    }
  }

  // Remove points from the center mask that overlap with the left and right masks
  for (i,j) in D_left do {
    if D_center.contains((i,j)) {
      D_center -= (i,j);
    }
  }
  for (i,j) in D_right do {
    if D_center.contains((i,j)) {
      D_center -= (i,j);
    }
  }

  var mask_size = (+ reduce left_mask) + (+ reduce center_mask) + (+ reduce right_mask);

  return (left_mask, center_mask, right_mask, mask_size);

}

} // module distance_mask
