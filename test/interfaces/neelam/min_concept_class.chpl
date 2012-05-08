class C {
	var data:int;
}
class D : C {}

interface LessThan {
  proc LT(x:self, y:self):bool;
}

C implements LessThan;

proc LT(x:C, y:C) : bool {
  return x.data < y.data;
}

proc minFn(x:?T, y:T):T where T implements LessThan {
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

