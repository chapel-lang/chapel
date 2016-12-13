
class C { proc ~C {} }
class D { }  proc D.~D {}

var c = new C();
delete c;
var d = new D();
delete d;
