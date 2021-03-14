// No longer relies on the generation of a default initializer, as the
// presence of any initializer prevents the creation of a compiler generated
// one.
class C {
   var x:int;
   var y:int;
   proc init(a:int,b:int) {
     x = a;
     y = b;
     this.complete();
     writeln(this); // Only doable in Phase 2
   }
}

writeln("C(1,2);");
var c = new unmanaged C(1,2);
writeln("C(a=3, b=4);");
delete c; c = new unmanaged C(a=3, b=4);
writeln(c);
writeln("C(a=5,6);");
delete c; c = new unmanaged C(a=5,6);
writeln("C(7,b=8);");
delete c; c = new unmanaged C(7,b=8);
writeln(c);
delete c;
