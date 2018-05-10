class ClassA {
  var a:int;
}

class ClassB {
  var b:real;
}

class GenericClassUnmanaged {
  var f:unmanaged;
  proc init(arg:unmanaged) {
    this.f = arg;
  }
}

class GenericClassBorrowed {
  var f:borrowed;
  proc init(arg:borrowed) {
    this.f = arg;
  }
}

proc test1() {
  var a = new unmanaged ClassA(1);
  var b = new unmanaged ClassA(2);
  var x = new GenericClassUnmanaged(a);
  var y:GenericClassUnmanaged = new GenericClassUnmanaged(b);

  writeln(x.type:string, " ", x);
  writeln(y.type:string, " ", y);

  delete a;
  delete b;
}

test1();

proc test2() {
  var a = (new owned ClassA(1)).borrow();
  var b = (new owned ClassA(2)).borrow();
  var x = new GenericClassBorrowed(a);
  var y:GenericClassBorrowed = new GenericClassBorrowed(b);

  writeln(x.type:string, " ", x);
  writeln(y.type:string, " ", y);
}

test2();
