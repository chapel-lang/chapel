class C {
  var myid: int;
}

var x: [LocaleSpace] C;

for i in LocaleSpace {
  on Locales(i) do x(i) = new C(i);
}

writeln("x is: ", x);

for i in LocaleSpace {
  writeln("x[", i, "] is on locale: ", x(i).locale.id);
  writeln("x[", i, "].myid is on locale: ", x(i).myid.locale.id);
}

for xx in x do on xx do delete xx;
