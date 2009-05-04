class C { var x: int; }

class C1 {
  var c = new C();
  var y: int;
}

class C2: C1 { var z: int; }

var c2: C2 = new C2();
