class C {
  var myid: int;
}

var x: [LocaleSpace] unmanaged C?;
var y: [LocaleSpace] borrowed C?;

for i in LocaleSpace {
  on Locales(i) do x(i) = new unmanaged C(i);
}
y = x;

writeln("x is: ", x);

for i in LocaleSpace {
  writeln("x[", i, "] is on locale: ", x(i).locale.id);
  writeln("x[", i, "].myid is on locale: ", x(i)!.myid.locale.id);
  writeln("y[", i, "] is on locale: ", y(i).locale.id);
  writeln("y[", i, "].myid is on locale: ", y(i)!.myid.locale.id);
}

for xx in x do on xx do delete xx;
