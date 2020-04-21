use Time;

proc serialWithReturn() {
    serial do return 1;
}

proc test() {

  serialWithReturn();

  begin{
    sleep(1);
    writeln("Begin 1");
  }

  begin{
    writeln("Begin 2");  
  }
}

test();
