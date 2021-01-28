class C {
	var data:int;
}

interface LessThan(type T) {
  proc LT(x:T, y:T):bool;
}

implements LessThan(C);

proc LT(x:C, y:C) : bool {
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

var c1 = new C(1);
var c2 = new C(2);
writeln(minFn(c1,c2));

