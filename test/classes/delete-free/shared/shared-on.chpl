class MyClass {
  var x:int;
}

var a:shared MyClass?;

on Locales[numLocales-1] {
  a = new shared MyClass(1);
  writeln(a.locale.id);
  
  var b:shared MyClass?;
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
