class C {
  var i : int;
  var D: domain(1) = {1..numLocales};
  var A: [D] int = i..i+numLocales-1;
}

class D {
  var cs: [0..numLocales-1] unmanaged C;

  proc init() {
    this.complete();
    coforall i in 0..numLocales-1 do 
      on Locales(i) do cs[i] = new unmanaged C(i);
  }

  proc deinit() {
    coforall i in 0..numLocales-1 do
      on Locales(i) do delete cs[i];
  }

}

var d = new unmanaged D();
writeln(d);
writeln(d.cs);
for i in 0..numLocales-1 do
  writeln(d.cs[i].locale.id);
delete d;
