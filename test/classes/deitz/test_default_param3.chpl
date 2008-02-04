enum Colors { Red, Green, Blue };

class C {
  param color: Colors = Red;
  var x: int;
}

var c = new C();
writeln(c);

var cc = new C(Blue);
writeln(cc);
  
