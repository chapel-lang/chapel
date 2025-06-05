use Map;

proc foo(const ref m: map(int, int)) {
  var newMap = m;
  writeln(newMap[10]);
}

var m = new map(int, int);
m[10] = 10;

foo(m);
