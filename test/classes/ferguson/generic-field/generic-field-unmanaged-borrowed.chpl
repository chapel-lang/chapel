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
  var ownX = new owned GenericClassUnmanaged(a);
  var x = ownX.borrow();
  var ownY = new owned GenericClassUnmanaged(b);
  var y:borrowed GenericClassUnmanaged = ownY.borrow();
  var ownZ = new owned GenericClassUnmanaged(c);
  var z:borrowed GenericClassUnmanaged(unmanaged ClassA) = ownZ.borrow();

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
  var ownA = new owned ClassA(1);
  var a = ownA.borrow();
  var ownB = new owned ClassA(2);
  var b = ownB.borrow();
  var ownC = new owned ClassA(3);
  var c = ownC.borrow();
  var x = new unmanaged GenericClassBorrowed(a);
  var ownY = new owned GenericClassBorrowed(b);
  var y:borrowed GenericClassBorrowed = ownY.borrow();
  var ownZ = new owned GenericClassBorrowed(c);
  var z:borrowed GenericClassBorrowed(borrowed ClassA) = ownZ.borrow();

  writeln(x.type:string, " ", x);
  writeln(y.type:string, " ", y);
  writeln(z.type:string, " ", z);

  delete x;
}

test2a();

proc test2b() {
  var ownA = new owned ClassB(1);
  var a = ownA.borrow();
  var ownB = new owned ClassB(2);
  var b = ownB.borrow();
  var ownC = new owned ClassB(3);
  var c = ownC.borrow();

  var ownX = new owned GenericClassBorrowed(a);
  var x = ownX.borrow();
  var myOwnedY = new owned GenericClassBorrowed(b);
  var y:borrowed GenericClassBorrowed = myOwnedY;
  var myOwnedZ = new owned GenericClassBorrowed(c);
  var z:borrowed GenericClassBorrowed(borrowed ClassB) = myOwnedZ;

  writeln(x.type:string, " ", x);
  writeln(y.type:string, " ", y);
  writeln(z.type:string, " ", z);
}

test2b();
