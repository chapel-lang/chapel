module Lib {
  var globalLib = 10;
  proc updateLib() { globalLib += 1; }
}

module Main {
  use Lib;
  var s$: sync int;
  var globalMain = 20;
  proc updateMain() { globalMain += 1; }

  proc main {
    coforall 1..2 {
      s$ = 1; // grab the lock
      writeln("globaLib   = ", globalLib);
      writeln("globalMain = ", globalMain);
      updateLib();
      updateMain();
      s$;
    }
  }
}
