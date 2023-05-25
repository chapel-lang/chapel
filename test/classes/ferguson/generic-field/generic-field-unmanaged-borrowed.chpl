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

proc test1a() {
  var a = new unmanaged ClassA(1);
  var b = new unmanaged ClassA(2);
  var c = new unmanaged ClassA(3);
  var x = (new owned GenericClassUnmanaged(a)).borrow();
  var y:borrowed GenericClassUnmanaged = (new owned GenericClassUnmanaged(b)).borrow();
  var z:borrowed GenericClassUnmanaged(unmanaged ClassA) = (new owned GenericClassUnmanaged(c)).borrow();

  writeln(x.type:string, " ", x);
  writeln(y.type:string, " ", y);
  writeln(z.type:string, " ", z);

  delete a;
  delete b;
  delete c;
}

test1a();

proc test1b() {
  var a = new unmanaged ClassB(1);
  var b = new unmanaged ClassB(2);
  var c = new unmanaged ClassB(3);
  var x = new owned GenericClassUnmanaged(a);
  var myOwnedY = new owned GenericClassUnmanaged(b);
  var y:borrowed GenericClassUnmanaged = myOwnedY;
  var myOwnedZ = new owned GenericClassUnmanaged(c);
  var z:borrowed GenericClassUnmanaged(unmanaged ClassB) = myOwnedZ;

  writeln(x.type:string, " ", x);
  writeln(y.type:string, " ", y);
  writeln(z.type:string, " ", z);

  delete a;
  delete b;
  delete c;
}

test1b();


proc test2a() {
  var a = (new owned ClassA(1)).borrow();
  var b = (new owned ClassA(2)).borrow();
  var c = (new owned ClassA(3)).borrow();
  var x = new unmanaged GenericClassBorrowed(a);
  var y:borrowed GenericClassBorrowed = (new owned GenericClassBorrowed(b)).borrow();
  var z:borrowed GenericClassBorrowed(borrowed ClassA) = (new owned GenericClassBorrowed(c)).borrow();

  writeln(x.type:string, " ", x);
  writeln(y.type:string, " ", y);
  writeln(z.type:string, " ", z);

  delete x;
}

test2a();

proc test2b() {
  var a = (new owned ClassB(1)).borrow();
  var b = (new owned ClassB(2)).borrow();
  var c = (new owned ClassB(3)).borrow();

  var x = (new owned GenericClassBorrowed(a)).borrow();
  var myOwnedY = new owned GenericClassBorrowed(b);
  var y:borrowed GenericClassBorrowed = myOwnedY;
  var myOwnedZ = new owned GenericClassBorrowed(c);
  var z:borrowed GenericClassBorrowed(borrowed ClassB) = myOwnedZ;

  writeln(x.type:string, " ", x);
  writeln(y.type:string, " ", y);
  writeln(z.type:string, " ", z);
}

test2b();
