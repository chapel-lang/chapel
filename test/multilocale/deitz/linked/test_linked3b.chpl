record C {
  var i : int;
  var D: domain(1) = {1..numLocales};
  var A: [D] int = i..i+numLocales-1;
}

var cs: [0..numLocales-1] C;

coforall loc in Locales do
  on loc do
    cs[loc.id] = new C(loc.id);

writeln(cs);
