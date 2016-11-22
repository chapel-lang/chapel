use samples;

proc xform(ref arg) ref {
  arg[1].x += 1;
  return arg;
}

var A:[1..2] R;
var B = xform(xform(xform(A)));

// How many copies of the array A's elements should be made
// by this program?
