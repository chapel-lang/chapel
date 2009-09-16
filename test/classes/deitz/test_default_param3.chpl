enum Colors { Red, Green, Blue };

class C {
  param color: Colors = Colors.Red;
  var x: int;
}

var c = new C();
writeln(c);

var cc = new C(Colors.Blue);
writeln(cc);

delete c;
delete cc;
