// Test iterator without using explicitly _to_seq(iterator)
def simple() : int {
  var i : int;
  while i < 10 {
    yield i * 4;
    i += 1;
  }
}
writeln( "test1: ", simple());

// Test with argument to iterator
def simple2( max: int) : int {
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

def simple3( r: R) : int {
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

def simple4( c: C) : int {
  var i : int;
  while i < c.max {
    yield i * 4;
    i += 1;
  }
}

var c: C = C();
c.max = 10;
writeln( "test4: ", simple4( c));
