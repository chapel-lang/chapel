enum Colors { Red, Green, Blue };

class C {
  param color: Colors = Colors.Red;
  var x: int;
}

var c = (new owned C()).borrow();
writeln("{color = ", c.color, ", x = ", c.x, "}");

var cc = (new owned C(Colors.Blue)).borrow();
writeln("{color = ", cc.color, ", x = ", cc.x, "}");
