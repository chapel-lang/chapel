/*

  Tests of argument bundle handling for various Chapel constructs.

  Since the runtime layers might have different logic to handle
  different sizes of task/on bundles, this test ensures that they
  work as expected with a variety of sizes.
*/


config const niters = if CHPL_COMM == "none" then 1000 else 10;
config const ncoforall = 100;
config const debug = false;


// This is just a way to get an assert
// explicitly allowed for fast-on.
// It could be removed if assert is at some point
// already fast-on OK.
require "myassert.h", "myassert.c";

pragma "insert line file info"
pragma "fast-on safe extern function"
extern proc assert_match(got:int, expected:int);


proc dobegin(param n) {

  var x: int;
  var tup: n * int;
  var order$: sync bool; // can't be inside loop?

  if debug then writeln("dobegin");

  sync {
    for i in 1..niters {
      if debug then writeln("iter ", i);

      const capture_i = i;
      x = i;
      tup[0] = i;
      tup[n-1] = i;

      begin {
        if debug then writeln("task 1 before wait");
        order$; // wait for order to be set below
        if debug then writeln("task 1 after wait");
        assert(x == capture_i);
        assert(tup[0] == capture_i);
        assert(tup[n-1] == capture_i);
      }
      
      x += 1;
      tup[0] += 1;
      tup[n-1] = x;

      begin {
        if debug then writeln("task 2 before wait");
        order$; // wait for order to be set below
        if debug then writeln("task 2 after wait");
        assert(x == capture_i+1);
        assert(tup[0] == capture_i+1);
        assert(tup[n-1] == capture_i+1);
      }

      // Modifications to x and tup at this point
      // should not affect the tasks.
      x = 0;
      tup[0] = 0;
      tup[n-1] = 0;

      if debug then writeln("first write");
      // Run a begin
      order$ = true;
      if debug then writeln("second write");
      // Run another begin
      order$ = true;
      if debug then writeln("after writes");
    }
    if debug then writeln("end of loop");
  }
}

proc dobeginon(param n) {

  var x: int;
  var tup: n * int;
  var order$: sync bool; // can't be inside loop?

  if debug then writeln("dobeginon");

  sync {
    for i in 1..niters {
      if debug then writeln("iter ", i);

      const capture_i = i;
      x = i;
      tup[0] = i;
      tup[n-1] = i;

      begin on Locales[numLocales-1] {
        if debug then writeln("task 1 before wait");
        order$; // wait for order to be set below
        if debug then writeln("task 1 after wait");
        assert(x == capture_i);
        assert(tup[0] == capture_i);
        assert(tup[n-1] == capture_i);
      }
      
      x += 1;
      tup[0] += 1;
      tup[n-1] = x;

      begin on Locales[numLocales-1] {
        if debug then writeln("task 2 before wait");
        order$; // wait for order to be set below
        if debug then writeln("task 2 after wait");
        assert(x == capture_i+1);
        assert(tup[0] == capture_i+1);
        assert(tup[n-1] == capture_i+1);
      }

      // Modifications to x and tup at this point
      // should not affect the tasks.
      x = 0;
      tup[0] = 0;
      tup[n-1] = 0;

      if debug then writeln("first write");
      // Run a begin
      order$ = true;
      if debug then writeln("second write");
      // Run another begin
      order$ = true;
      if debug then writeln("after writes");
    }
    if debug then writeln("end of loop");
  }
}

proc doon(param n) {

  var x: int;
  var tup: n * int;

  if debug then writeln("doon");

  for i in 1..niters {
    if debug then writeln("iter ", i);

    const capture_i = i;
    x = i;
    tup[0] = i;
    tup[n-1] = i;
    const tup_copy = tup; 

    on Locales[numLocales-1] {
      assert(x == capture_i);
      assert(tup[0] == capture_i);
      assert(tup[n-1] == capture_i);
      assert(tup_copy[0] == capture_i);
      assert(tup_copy[n-1] == capture_i);
    }
    
    // Modifications to x and tup at this point
    // should not affect the tasks.
    x = 0;
    tup[0] = 0;
    tup[n-1] = 0;
  }
  if debug then writeln("end of loop");
}


proc dofaston(param n) {

  var x: int;
  var tup: n * int;

  if debug then writeln("dofaston");

  for i in 1..niters {
    if debug then writeln("iter ", i);

    const capture_i = i;
    x = i;
    tup[0] = i;
    tup[n-1] = i;
    const tup_copy = tup; 

    on Locales[numLocales-1] {
      assert_match(x, capture_i);
      assert_match(tup_copy[0], capture_i);
      assert_match(tup_copy[n-1], capture_i);
    }
    
    // Modifications to x and tup at this point
    // should not affect the tasks.
    x = 0;
    tup[0] = 0;
    tup[n-1] = 0;
  }
  if debug then writeln("end of loop");
}

proc defastbeginon(param n) {

  var x: int;
  var tup: n * int;

  if debug then writeln("defastbeginon");

  for i in 1..niters {
    if debug then writeln("iter ", i);

    const capture_i = i;
    x = i;
    tup[0] = i;
    tup[n-1] = i;
    const tup_copy = tup; 

    begin on Locales[numLocales-1] {
      assert_match(x, capture_i);
      assert_match(tup_copy[0], capture_i);
      assert_match(tup_copy[n-1], capture_i);
    }
    
    // Modifications to x and tup at this point
    // should not affect the tasks.
    x = 0;
    tup[0] = 0;
    tup[n-1] = 0;
  }
  if debug then writeln("end of loop");
}




/* This could be commented out once bug.chpl is fixed
iter modifyAndYield(num:int, i: int, ref x: int, ref tup: n*int)
{
  for j in 1..num {
    const tmp = 1000*i + j;
    x = tmp;
    tup[0] = tmp;
    tup[n-1] = tmp;
    if debug then writeln("Yielding ", j, " x=", x);
    yield j;
  }
}

proc docoforall(param n) 

  var x: int;
  var tup: n * int;
  var order$: sync bool; // can't be inside loop?

  if debug then writeln("docoforall");

  sync {
    for i in 1..niters {
      if debug then writeln("iter ", i);
      const capture_i = i;

      coforall j in modifyAndYield(ncoforall, i, x, tup) {
        var tmp = 1000*capture_i + j;
        if x != tmp {
          writeln("Task i=", i, " j=", j, " got tmp ", tmp, " x=", x);
        }
        assert(x == tmp);
        assert(tup[0] == tmp);
        assert(tup[n-1] == tmp);
      }
    }
    if debug then writeln("end of loop");
  }
}
*/

proc dotests(param nbytes) {
  param nwords = (7+nbytes) / 8; 
  dobegin(nwords);
  dobeginon(nwords);
  doon(nwords);
  dofaston(nwords);
  defastbeginon(nwords);
  //docoforall(nwords);
}

// interesting sizes:
//  GASNet "small" messages are < 128 bytes
//  GASNet UDP gasnet_AMMaxMedium() == 512 bytes (the minimum allowed in Gasnet)
//  GASNet IBV gasnet_AMMaxMedium() == 4024 in one configuration
//  
dotests(1);
dotests(5);
dotests(10);
dotests(50);
dotests(100);
dotests(250);
dotests(500);
dotests(1000);
dotests(2500);
dotests(5000);
dotests(10000);

writeln("OK");
