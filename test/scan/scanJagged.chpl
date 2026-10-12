record R {
  var size: int;
  var inner: [0..<size] real;
}

var outer = [i in 0..<10] new R(size=i);
//writeln(outer);

{ // Original version
  var offsets = + scan [i in 0..#outer.size] outer[i].inner.size;
  writeln(offsets);
}

{ // Index-neutral rewrite
  var offsets = + scan [i in outer.domain] outer[i].inner.size;
  writeln(offsets);
}

{ // Whole-array rewrite
  var offsets = + scan outer.inner.size;
  writeln(offsets);
}

{ // Loop-over-array rewrite
  var offsets = + scan [o in outer] o.inner.size;
  writeln(offsets);
}
