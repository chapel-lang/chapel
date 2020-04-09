proc paramprint(param arg) {
  writeln(arg.type:string, " ", arg:string);
}

proc check(type t, param ini) {
  writeln(t:string, " initialized by ", ini.type:string, " ", ini);
  param x:t = ini:t;
  write("x=", x, " ");
  paramprint(x);
  param y:t = ini; // relies on conversion
  write("y=", y, " ");
  paramprint(y);
  param z = ini:t;
  write("z=", z, " ");
  paramprint(z);
  // Check that the types match
  assert(x.type == y.type && y.type == z.type);
  // Check that the values match
  assert(x == y && y == z);
  // Now try casting one of them back to ini.type
  param a = z:ini.type;
  write("a=", a, " ");
  paramprint(a);
  writeln();
}
proc checkNoCoerce(type t, param ini) {
  writeln(t:string, " initialized by ", ini.type:string, " ", ini);
  param x:t = ini:t;
  write("x=", x, " ");
  paramprint(x);
  param y:t = ini:t; // does not rely on coercion
  write("y=", y, " ");
  paramprint(y);
  param z = ini:t;
  write("z=", z, " ");
  paramprint(z);
  // Check that the types match
  assert(x.type == y.type && y.type == z.type);
  // Check that the values match
  assert(x == y && y == z);
  // Now try casting one of them back to ini.type
  param a = z:ini.type;
  write("a=", a, " ");
  paramprint(a);
  writeln();
}


// initializing from real of different sizes
check(complex(128), 1.0);
check(complex(128), 1.0:real(64));
check(complex(128), 1.0:real(32));
check(complex(64), 1.0);
check(complex(64), 1.0:real(64));
check(complex(64), 1.0:real(32));
writeln();

// initializing from imag of different sizes
check(complex(128), 1.0i);
check(complex(128), 1.0i:imag(64));
check(complex(128), 1.0i:imag(32));
check(complex(64), 1.0i);
check(complex(64), 1.0i:imag(64));
check(complex(64), 1.0i:imag(32));
writeln();

// initializing from complex of different sizes 
check(complex(128), 2.0+1.0i);
check(complex(128), (2.0+1.0i):complex(128));
check(complex(128), (2.0+1.0i):complex(64));
check(complex(64), 2.0+1.0i);
check(complex(64), (2.0+1.0i):complex(128));
check(complex(64), (2.0+1.0i):complex(64));
writeln();

// again with a negative value
check(complex(128), -2.0-1.0i);
check(complex(128), (-2.0-1.0i):complex(128));
check(complex(128), (-2.0-1.0i):complex(64));
check(complex(64), -2.0-1.0i);
check(complex(64), (-2.0-1.0i):complex(128));
check(complex(64), (-2.0-1.0i):complex(64));
writeln();


// initializing from int of different sizes
check(complex(128), -1);
check(complex(128), -1:int(64));
check(complex(128), -1:int(32));
check(complex(128), -1:int(16));
check(complex(128), -1:int(8));
check(complex(64), -1);
check(complex(64), -1:int(64));
check(complex(64), -1:int(32));
check(complex(64), -1:int(16));
check(complex(64), -1:int(8));
writeln();

// initializing from uint of different sizes
check(complex(128), 1:uint);
check(complex(128), 1:uint(64));
check(complex(128), 1:uint(32));
check(complex(128), 1:uint(16));
check(complex(128), 1:uint(8));
check(complex(64), 1);
check(complex(64), 1:uint(64));
check(complex(64), 1:uint(32));
check(complex(64), 1:uint(16));
check(complex(64), 1:uint(8));
writeln();

// initializing from bool of different sizes
checkNoCoerce(complex(128), true);
checkNoCoerce(complex(128), true:bool(64));
checkNoCoerce(complex(128), true:bool(32));
checkNoCoerce(complex(128), true:bool(16));
checkNoCoerce(complex(128), true:bool(8));
checkNoCoerce(complex(64), true);
checkNoCoerce(complex(64), true:bool(64));
checkNoCoerce(complex(64), true:bool(32));
checkNoCoerce(complex(64), true:bool(16));
checkNoCoerce(complex(64), true:bool(8));
writeln();

// Note, initializing a numeric param from a string param
// is not currently supported, but if it becomes supported,
// this test could include initialization from various strings.
