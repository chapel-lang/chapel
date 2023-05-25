module M {

  private use IO;

  /* Can you see me? */
  proc helloWorld() {
    writeln("Hello, world!");
  }

  /* This is definitely visible */
  proc fantasticFunction() {
    return 42;
  }

}
