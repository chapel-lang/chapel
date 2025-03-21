

var x = 0;
var y = 0.0;
var z = false;

proc foo(args...?n) {
  if n >= 1 then var x = args[0];
  if n >= 2 then var x = args[1];
  if n >= 3 then var x = args[2];
}

foo(x, y, z);
