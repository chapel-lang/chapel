
// From user issue #12462

record Concrete {
  param size: uint;
}

record SingleData {
  type t;
  var data: t;
}

record SingleDataArray {
  type t;
  var data: [1..4] SingleData(t);
}

record TupleData {
  param p: int;
  
  type t;
  var data: p*t;
}

record TupleDataArray {
  param p: int;
  
  type t;
  var data: [1..4] TupleData(p, t);
}

record R {
  type ut;
  param u: ut;
}

proc main() {
  var r: Concrete(64) = new Concrete(64);
  writeln(r);

  {
    var A : SingleDataArray(R(int, 42));
    var B : SingleDataArray(R(uint, 42:uint));
    var C : SingleDataArray(R(uint, 42));

    writeln(A);
    writeln(B);
    writeln(C);
  }
  
  
  {
    var A : TupleDataArray(2, R(int, 42));
    var B : TupleDataArray(2, R(uint, 42:uint));
    var C : TupleDataArray(2, R(uint,42));

    writeln(A);
    writeln(B);
    writeln(C);
  }
}
