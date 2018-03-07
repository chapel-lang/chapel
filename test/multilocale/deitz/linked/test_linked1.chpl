class C {
  var x: int;
}

class D {
  var cs: [0..numLocales-1] C;

  proc init() {
    this.initDone();
    coforall i in 0..numLocales-1 do 
      on Locales(i) do cs[i] = new C(i);
  }

  proc deinit() {
    coforall i in 0..numLocales-1 do 
      on Locales(i) do delete cs[i];
  }
}

var d = new D();
writeln(d);
writeln(d.cs);
for i in 0..numLocales-1 do
  writeln(d.cs[i].locale.id);
delete d;
