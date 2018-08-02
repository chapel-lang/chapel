record C {
  var x: int;
}

class D {
  var cs: [0..numLocales-1] C;

  proc init() {
    this.complete();
    coforall i in 0..numLocales-1 do
      on Locales(i) do cs[i] = new C(i);
  }
}

var d = new owned D();

writeln(d);
writeln(d.cs);

for i in 0..numLocales-1 do
  writeln(d.cs[i].locale.id);
