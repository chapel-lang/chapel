use samples;

proc xform(ref arg) {
  arg[1].x += 1;
  return arg;
}

var A:[1..2] R;
var t1 = xform(A);
var t2 = xform(t1);
var B = xform(t2);

// How many copies of the array A's elements should be made
// by this program?
