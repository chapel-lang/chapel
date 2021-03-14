class C {
  var x: int;
};

var c: unmanaged C?;
on Locales[numLocales-1] do c = new unmanaged C();
writeln(c.locale);
delete c;

c = new unmanaged C();
writeln(c.locale);
on Locales[numLocales-1] do delete c;

