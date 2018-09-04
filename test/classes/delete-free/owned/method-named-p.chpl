class C {
 proc init() {}
 proc p(x) { }
}

class D : C {
 proc init() {
   super.init();
 }
 override proc p(x) {
   writeln("In D.p(", x, ")");
 }
}

var d = new owned D();
compilerWarning(d.type : string);
d.p(1);

var e = new shared D();
compilerWarning(e.type : string);
e.p(1);
