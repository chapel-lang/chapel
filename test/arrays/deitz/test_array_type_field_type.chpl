class C {
  const D;
  type AT = [D] int;
  var A: AT;
}

var c = new C([1..3]);
writeln(c);
