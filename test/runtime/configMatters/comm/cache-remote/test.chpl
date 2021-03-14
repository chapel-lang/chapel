extern proc chpl_cache_print();

config const verbose = false;

class C {
  var x = 17;
  var y = 24;
  var z = 77;
}

proc test_puts() {
  var x = 17;
  var y = 24;
  writeln("On locale ", here);
  on Locales[1] {
    writeln("PERFORMING 2 PUTS ON ", here);
    x = 22;
    if verbose then chpl_cache_print();
    y = 19;
    if verbose then chpl_cache_print();
  }
  writeln("RESULT x=",x," y=",y," expect x=22 y=19");
  on Locales[1] {
    writeln("PERFORMING 2 PUTS ON ", here);
    y = 12;
    if verbose then chpl_cache_print();
    x = 64;
    if verbose then chpl_cache_print();
  }
  writeln("RESULT x=",x," y=",y," expect x=64 y=12");
}
proc test_gets() {
  var c = new C();
  writeln("On locale ", here);
  on Locales[1] {
    writeln("PERFORMING 3 GETS ON ", here);
    var myx = c.x;
    var myy = c.y;
    var myz = c.z;
    if verbose then chpl_cache_print();
    writeln("GOT x=",myx," y=", myy, " z=", myz);
    assert( myx == 17 );
    assert( myy == 24 ); 
    assert( myz == 77 ); 
  }
  on Locales[1] {
    writeln("PERFORMING 3 GETS ON ", here);
    var myy = c.y;
    var myx = c.x;
    var myz = c.z;
    if verbose then chpl_cache_print();
    writeln("GOT x=",myx," y=", myy, " z=", myz);
    assert( myx == 17 );
    assert( myy == 24 ); 
    assert( myz == 77 ); 
  }
}


proc main() {
  test_puts();
  test_gets();
}

