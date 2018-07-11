enum Colors { Red, Green, Blue };

class C {
  param color: Colors = Colors.Red;
  var x: int;
}

var c = new borrowed C();
writeln("{color = ", c.color, ", x = ", c.x, "}");

var cc = new borrowed C(Colors.Blue);
writeln("{color = ", cc.color, ", x = ", cc.x, "}");
