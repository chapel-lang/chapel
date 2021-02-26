use Time;

proc serialWithReturn() {
    serial do return 1;
}

proc test() {
  var x$: sync int;
  
  serialWithReturn();

  sync{
    begin{
      x$; 
      writeln("Begin 1");
    }

    begin{
      writeln("Begin 2");
      x$ = 1;
    }
  }
}

test();
