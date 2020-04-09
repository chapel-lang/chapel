record R {
  type t = int;
  var x:t;
}

proc gr() {
  return new R(real, 7.9);
}

proc gi() :R {
  return new R(x=22);
}

type XYZ = R;
proc gii() : XYZ {
  return new R(x=23);
}

var a = gr();
writeln(a);
var b = gi();
writeln(b);
var c = gii();
writeln(c);
