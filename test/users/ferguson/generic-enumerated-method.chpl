enum MyEnum {
  x = 1,
  y = 2
}

proc enumerated.blarg() {
  return 1;
}

var x = MyEnum.x.blarg();
writeln(x);

record R {
  var bz:uint(8) = MyEnum.x.blarg();
}



var r = new R();
writeln(r);
