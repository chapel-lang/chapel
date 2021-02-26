config const niters = 1000;
config const debug = false;

proc dobegin() {

  if debug then writeln("dobegin");

  // bug goes away if order$ is declared here.

  sync {
    for i in 1..niters {
      if debug then writeln("iter ", i);

      var order$: sync bool; // can't be inside loop?

      begin {
        if debug then writeln("task 1 before wait");
        order$; // wait for order to be set below
        if debug then writeln("task 1 after wait");
      }
      
      begin {
        if debug then writeln("task 2 before wait");
        order$; // wait for order to be set below
        if debug then writeln("task 2 after wait");
      }

      if debug then writeln("first write");
      // Run a begin
      order$ = true;
      if debug then writeln("second write");
      // Run another begin
      order$ = true;
      if debug then writeln("after writes");

      // wait for order$ to be empty again
      order$ = true;
    }
    if debug then writeln("end of loop");
  }
}

dobegin();


writeln("OK");
