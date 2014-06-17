use Time;

var i$: sync int;

proc foo() {
  var x: int = 2;
  on Locales(1) {
    begin {
      writeln(here.id, " x=", x);
      i$ = 1;
    }
  }
  writeln(i$);
  writeln(here.id, " x=", x);
}

foo();
