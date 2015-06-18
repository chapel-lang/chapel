use VisualDebug;


startVdebug("CM");

coforall ix in 1 .. numLocales-1 do
   on Locales[ix] {
      var r : real;
      for t in 1 .. 1000 do
        for i in 1 .. 1000000*(ix+1) do
          r = 1.0 / i;
      writeln(r);
   }

stopVdebug();
