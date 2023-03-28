

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
  writeln(a, " | ", a.type:string);
}

{
  writeln("test adopt of owned");
  var own: A? = new A();
  var a = shared.adopt(own);
  writeln(own, " | ", own.type:string);
  writeln(a, " | ", a.type:string);
}
