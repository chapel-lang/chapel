use samples;
proc f() {
  var A:[1..4] R;
  var B => A;
  return B;
}

var A_slice = f();
writeln(A_slice);

// Is returning a slice of a local array variable valid? Or an error?
