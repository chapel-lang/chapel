

class A {
  var x: int;
  var y: int;
  proc init() {x=0;y=0;}
  proc init(x:int,y:int) {this.x=x;this.y=y;}
}

{
  writeln("test adopt of unmanaged");
  var aHeapObj = new unmanaged A();
  var a = shared.adopt(aHeapObj);
}

{
  writeln("test adopt of owned");
  var own: A? = new A();
  var a = shared.adopt(own);
  writeln(own);
}


{
  writeln("test adopt ownership transfer to shared object");
  var shr1: A = new shared A(2, 3);
  var shr2 = shared.adopt(shr1);
  // both should refer to the same object
  writeln(shr1);
  writeln(shr2);
  shr2.x += shr1.y;
  writeln(shr1);
  writeln(shr2);
}
