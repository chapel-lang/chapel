// test #1
var s:int;

proc function1( a:int, b:int, c:int) {
  var d:int;

  proc nested_function( a:int, b:int, c:int) {
    writeln( "test1:", a, b, c, d, s);
  }

  d = 2*a;
  cobegin {
    nested_function( a, b, c);
    nested_function( b, c, a);
    nested_function( c, a, b);
  }
}

var x:int = 1;
var y:int = 2;
var z:int = 3;

s = 5;
function1( x, y, z);


// test #2
proc function2( a:int) {
  var d:int;

  d = a;
  cobegin with (ref d) {
    d = 2*a;
    d = 4*a;
  }
  write( "test2: ");
  if (d==4 || d==8) {
    writeln( "good");
  } else {
    writeln( "bad");
  }
}

function2( 2);


// test #3
proc function3() {
  class C {
    var a:int;
  }

  class D {
    var c:unmanaged C?;
  }

  var c:unmanaged C = new unmanaged C();
  c.a = 1;

  var d:unmanaged D = new unmanaged D();
  d.c = new unmanaged C();
  d.c!.a = 2;

  cobegin {
    {
      d.c!.a = c.a * 6;
      c.a = 5;
    }
  }

  writeln( "test3:", c, d);

  delete d.c;
  delete d;
  delete c;
}

function3();
