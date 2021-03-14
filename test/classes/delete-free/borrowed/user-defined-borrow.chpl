class MyClass {

  var x:int;

  override proc borrow() {
    return "You just borrowed " + x:string;
  }
}

var a = new owned MyClass(1);
var b = a.borrow(); // owned.borrow()

var c = b.borrow(); // MyClass.borrow()
writeln(c.type:string, " ", c);
