class C {
  var x: int;
}

var myC = new C(42);

foo(myC);

proc foo(const ref myC: C) {
  coforall i in 1..10 do
    on Locales[i%numLocales] do
      writeln(i, " has ", myC);
}
