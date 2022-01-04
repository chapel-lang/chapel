use Time;

proc serialWithReturn() {
    serial do return 1;
}

proc test() {
  var x$: sync int;
  
  serialWithReturn();

  sync{
    begin{
      x$.readFE(); 
      writeln("Begin 1");
    }

    begin{
      writeln("Begin 2");
      x$.writeEF(1);
    }
  }
}

test();
