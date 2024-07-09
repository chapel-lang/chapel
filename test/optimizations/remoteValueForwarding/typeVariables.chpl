proc foo() {
  type t = [1..10] int;
  on Locales[numLocales-1] {
    var x: t;
    writeln(x, ": ", t:string);
  }
}

foo();
