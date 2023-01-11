record R {
  var myfield: integral;
}

var rr: R(int);
writeln(rr);

class MyClass { }
record A {
  var myfield: MyClass?;
}

var x = new A(new MyClass());
writeln(x, " : ", x.type:string);

record B {
  var myfield: MyClass?;
}

var y: B(owned MyClass?);
writeln(y, " : ", y.type:string);

record C {
  var myfield: MyClass;
  proc init() {
    this.myfield = new MyClass();
  }
}

var z = new C();
