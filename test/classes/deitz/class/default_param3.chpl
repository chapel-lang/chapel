enum Colors { Red, Green, Blue };

class C {
  param color: Colors = Colors.Red;
  var x: int;
}

var cOwn = new owned C();
var c = cOwn.borrow();
writeln("{color = ", c.color, ", x = ", c.x, "}");

var ccOwn = new owned C(Colors.Blue);
var cc = ccOwn.borrow();
writeln("{color = ", cc.color, ", x = ", cc.x, "}");
