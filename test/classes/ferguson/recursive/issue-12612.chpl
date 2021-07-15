record R {
  param size: uint;

  proc type t type
    return uint(size);

  type this_type = R(size); // Recursive? This line causes a segfault.

  var x: this_type.t;
}

proc main() {
  var r: R(64);
}
