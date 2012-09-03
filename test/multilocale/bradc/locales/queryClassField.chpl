class C {
  var myid: int;
}

var x: [rootLocale.getLocaleSpace()] C;

for i in rootLocale.getLocaleSpace() {
  on rootLocale.getLocales()(i) do x(i) = new C(i);
}

writeln("x is: ", x);

for i in rootLocale.getLocaleSpace() {
  writeln("x[", i, "] is on locale: ", x(i).locale.id);
  writeln("x[", i, "].myid is on locale: ", x(i).myid.locale.id);
}

for xx in x do on xx do delete xx;
