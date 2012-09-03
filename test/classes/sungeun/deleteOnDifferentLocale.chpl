class C {
  var x: int;
};

var c: C;
on rootLocale.getLocale(numLocales-1) do c = new C();
writeln(c.locale);
delete c;

c = new C();
writeln(c.locale);
on rootLocale.getLocale(numLocales-1) do delete c;

