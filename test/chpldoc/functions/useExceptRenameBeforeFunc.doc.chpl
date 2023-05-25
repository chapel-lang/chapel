module M {

  use IO as OI except file;

  /* Can you see me? */
  proc helloWorld() {
    writeln("Hello, world!");
  }

  /* This is definitely visible */
  proc fantasticFunction() {
    return 42;
  }

}
