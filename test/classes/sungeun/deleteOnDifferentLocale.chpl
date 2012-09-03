class C {
  var x: int;
};

var c: C;
on Locales[numLocales-1] do c = new C();
writeln(c.locale);
delete c;

c = new C();
writeln(c.locale);
on Locales[numLocales-1] do delete c;

