module SomeModule {
  /*
   Putting everything outside a module compiles fine
   */

  config const shouldThrow = false;
  proc foo(i, shouldThrow=false) throws {
    if shouldThrow && i == 0 then throw new Error();
    else return i*2;
  }

  proc main() {
    try {
      /*
       Below version compiles fine:

        var junk1 = forall i in -5..5 do foo(i, shouldThrow=shouldThrow);
        writeln(junk1);
      */

      var junk2: [-5..5] int;
      junk2 = [i in -5..5] foo(i, shouldThrow=shouldThrow);

      /*
       Turning this into:
      
        var junk2 = [i in -5..5] foo(i, shouldThrow=shouldThrow);

       doesn't change the behavior.
      */

      writeln(junk2);
    }
    catch e {
      writeln("Error caught");
    }
  }
}
