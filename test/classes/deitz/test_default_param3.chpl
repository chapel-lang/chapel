enum Colors { Red, Green, Blue };

class C {
  param color: Colors = Red;
  var x: int;
}

var c = C();
writeln(c);

var cc = C(Blue);
writeln(cc);
  
