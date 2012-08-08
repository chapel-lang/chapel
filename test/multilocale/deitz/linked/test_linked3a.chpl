record C {
  var i : int;
  var D: domain(1) = {1..numLocales};
  var A: [D] int = i..i+numLocales-1;
}

var cs: [0..numLocales-1] C;

coforall i in 0..numLocales-1 do 
  on Locales(i) do
    cs[i] = new C(i);

writeln(cs);
