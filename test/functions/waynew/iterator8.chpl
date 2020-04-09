// Test iterator without using explicitly _to_seq(iterator)
iter simple() : int {
  var i : int;
  while i < 10 {
    yield i * 4;
    i += 1;
  }
}
writeln( "test1: ", simple());

// Test with argument to iterator
iter simple2( max: int) : int {
  var i : int;
  while i < max {
    yield i * 4;
    i += 1;
  }
}
writeln( "test2: ", simple2( 10));


// Test with record argument to iterator
record R {
  var max: int;
}

iter simple3( r: R) : int {
  var i : int;
  while i < r.max {
    yield i * 4;
    i += 1;
  }
}

var r: R;
r.max = 10;
writeln( "test3: ", simple3( r));


// Test with class argument to iterator
class C {
  var max: int;
}

iter simple4( c: borrowed C) : int {
  var i : int;
  while i < c.max {
    yield i * 4;
    i += 1;
  }
}

var c: borrowed C = new borrowed C();
c.max = 10;
writeln( "test4: ", simple4( c));
