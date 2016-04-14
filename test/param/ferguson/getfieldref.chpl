use Reflection;

record R {
  var x:int;
}

var r:R;

proc getFieldRef(x:?t, param i:int) ref {
  return __primitive("field by num", x, i);
}

getFieldRef(r, 1) = 10;
writeln(r.x);

var i = getFieldRef(r, 1);
writeln(i);


