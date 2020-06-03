class MyClass {
  var x:int;
}

var a:owned MyClass?;

on Locales[numLocales-1] {
  a = new owned MyClass(1);
  writeln(a.locale.id);
  
  var b:owned MyClass?;
  writeln(b.locale.id);
  on b {
    writeln(here.id);
  }
  on Locales[0] {
    writeln(b.locale.id);
  }
}

writeln(a.locale.id);
on a {
  writeln(here.id);
}
