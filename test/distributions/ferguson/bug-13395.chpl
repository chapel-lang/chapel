use CyclicDist;

class C {
	var dom;
	var arr : [dom] int;
	proc init(in map : ?t = unmanaged DefaultDist()) {
		this.dom = {1..10} dmapped map;
	}
}

var c = new C(new cyclicDist(startIdx=1));
writeln("Hello World");
