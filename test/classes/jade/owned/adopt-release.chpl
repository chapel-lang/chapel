

class A {
  var x: int;
  var y: int;
  proc init() {x=0;y=0;}
  proc init(x:int,y:int) {this.x=x;this.y=y;}
}

{
  writeln("test adopt of unmanaged");
  var aHeapObj = new unmanaged A();
  var a = owned.adopt(aHeapObj);
}

{
  writeln("test release of owned");
  var a = new A();
  var aHeapObj = owned.release(a);
  delete aHeapObj;
}


{
  writeln("test adopt/release ownership transfer");
  var un = new unmanaged A(16, 17);

  var own: owned A = owned.adopt(un);
  writeln("unmanaged obj: ", un, " and managed obj: ", own);
  own.x += own.y;
  writeln("unmanaged obj: ", un, " and managed obj: ", own);

  var un2: unmanaged A = owned.release(own);
  writeln("unmanaged obj: ", un2);
  delete un2;

}


{
  writeln("test adopt ownership transfer to owned object");
  var own1 = new A(2, 3);
  var own2 = owned.adopt(own1);
  // own1 should now be invalid
  writeln(own2);
}
