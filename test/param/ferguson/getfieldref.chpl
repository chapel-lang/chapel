use Reflection;

record R {
  var x:int;
}

var r:R;

// note: this returns a ref to a field in
// a record that was passed with 'const ref'.
// If const checking is improved, I expect it
// to become a compile error.
proc getFieldRef(x:?t, param i:int) ref {
  return __primitive("field by num", x, i+1);
}

getFieldRef(r, 0) = 10;
writeln(r.x);

var i = getFieldRef(r, 0);
writeln(i);


