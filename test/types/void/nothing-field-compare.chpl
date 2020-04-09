record R {
  var a: int;
  var b: nothing;
}

var x = new R(a=1);
writeln(x);

var y = x; // copy-initialize
assert(y == x); // compare

var z = new R(a=2);
y = z; // assign

assert(y == z); // compare
assert(x != z);
