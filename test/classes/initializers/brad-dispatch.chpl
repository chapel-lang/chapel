// Modified from
// test/classes/constructors/brad-dispatch.chpl
class C {
   var x:int;
   var y:int;
   proc init(a:int,b:int) {
     x = a;
     y = b;
     super.init();
     writeln(this); // Only doable in Phase 2
   }
}

writeln("C(1,2);");
var c = new C(1,2);
writeln("C(x=3, y=4);");
delete c; c = new C(x=3, y=4);
writeln(c);
writeln("C(a=5,6);");
delete c; c = new C(a=5,6);
writeln("C(7,y=8);");
delete c; c = new C(7,y=8);
writeln(c);
delete c;
