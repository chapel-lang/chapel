interface I {
	proc self.f() { }
}
class C { }
C implements I;

var x : any I = new C();
x.f()
