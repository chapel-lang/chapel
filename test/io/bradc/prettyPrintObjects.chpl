record R {
  var a: int;
  var b: int;
}

class C {
  var x: int;
  var y: real;
  var z: R;
  var id: string;
}

var myC = new C(x=1, y=1.2, z=new R(3,4), id = "hello");
writeln("myC is:\n", myC);
