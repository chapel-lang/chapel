var global:[1..10] int;
var a = global[1..3];
var b = global[2..4];

proc f() {
  return ( a, b );
}

proc g() {
  return ( global[1..3], global[2..4] );
}

proc setEm( x ) {
  x(0)[1] = 1;
  x(1)[2] = 2;
}

global = 0;
setEm(f());
writeln(global);

global = 0;
setEm(g());
writeln(global);

