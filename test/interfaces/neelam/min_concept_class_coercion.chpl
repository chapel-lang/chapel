class C {
	var data:int;
	proc display() {
		writeln("C:"+data);
	}
}

class D : C {
	proc display() {
		writeln("D:"+data);
	}
}

interface LessThan(type T) {
  proc LT(x:T, y:T):bool;
}

implements LessThan(D);

proc LT(x:C, y:C) : bool {
  x.display();
  y.display();
  return x.data < y.data;
}

proc minFn(x:?T, y:T):T where implements LessThan(T) {
  if (LT(y, x)) {
    return y;
  }
  else {
    return x;
  }
}

var c1 = new D(1);
var c2 = new D(2);
writeln(minFn(c1,c2));

