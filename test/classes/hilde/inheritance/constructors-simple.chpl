class C{ proc init(){} }

class D:C {}

var d = new unmanaged D();

writeln(d);

delete d;

