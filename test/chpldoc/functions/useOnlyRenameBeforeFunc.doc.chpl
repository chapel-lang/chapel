module M {

  use IO as OI only file;

  /* Can you see me? */
  proc helloWorld() {
    writeln("Hello, world!");
  }

  /* This is definitely visible */
  proc fantasticFunction() {
    return 42;
  }

}
