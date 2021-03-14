proc foo() {
  var a = 10;
  on Locales[0] {
    writeln(a>10);
    writeln(a>=10);
    writeln(a<10);
    writeln(a<=10);
  }
}

foo();
