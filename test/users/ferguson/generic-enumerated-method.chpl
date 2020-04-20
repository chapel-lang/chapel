enum MyEnum {
  x = 1,
  y = 2
}

proc enum.blarg() {
  return 1;
}

var x = MyEnum.x.blarg();
writeln(x);

record R {
  var bz:int = MyEnum.x.blarg();
}



var r = new R();
writeln(r);
