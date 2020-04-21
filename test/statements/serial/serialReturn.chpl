use Time;

proc serialWithReturn() {
    serial do return 1;
}

proc test() {
  var x$: sync int;
  
  serialWithReturn();

  begin{
    x$; 
    writeln("Begin 1");
  }

  begin{
    writeln("Begin 2");
    x$ = 1;
  }
}

test();
