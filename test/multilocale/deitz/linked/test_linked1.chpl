class C {
  var x: int;
}

class D {
  var cs: [0..2] C;

  def initialize() {
    coforall i in 0..2 do 
      on Locales(i) do cs[i] = new C(i);
  }
}

var d = new D();
writeln(d);
writeln(d.cs);
for i in 0..2 do
  writeln(d.cs[i].locale);
