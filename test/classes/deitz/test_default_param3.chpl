enum Colors { Red, Green, Blue };

class C {
  param color: Colors = Colors.Red;
  var x: int;
}

var c = new C();
writeln("{color = ", c.color, ", x = ", c.x, "}");

var cc = new C(Colors.Blue);
writeln("{color = ", cc.color, ", x = ", cc.x, "}");

delete c;
delete cc;
