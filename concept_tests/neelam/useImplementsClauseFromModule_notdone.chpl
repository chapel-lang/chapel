interface I { }
proc f(x : ?T) where T implements I { }

module A {
	class C { }
	C implements I;
}

module B {
	from A use C implements I;

	proc g(y : A.C) {
		f(y);
	}
}
