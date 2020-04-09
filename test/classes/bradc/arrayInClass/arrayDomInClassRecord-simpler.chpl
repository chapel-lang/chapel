record R {
  var D: domain(1);
  var A: [D] real = [i in D] i;
}


var myR = new R({1..10});

writeln("myR is: ", myR);


//
// This is a test that doesn't work at present if the:
//
//   assert(here.id >= 0) 
//
// in ChapelDynDispatch.chpl is removed.  The best explanation
// for what's going wrong that I can find is that the only calls to
// chpl_id()/here.id that are resolved in functionResolution.cpp occur
// after the user code has been called and in resolveAutoCopies().
// For some reason when the resolution occurs this late in the game,
// it doesn't get the dynamic dispatch correct and dispatches to the
// pure virtual function in the root base class.  I tried to reproduce
// this in a standalone test with user-defined records and classes,
// but had difficulty creating a similar type hierarchy that would
// resolve a virtual function in resolveAutoCopies() but not
// before.  I don't know whether this is because I didn't get the
// pattern right or whether the compiler inserts special code for
// arrays/domains late in the compilation and therefore I can't
// reproduce this myself.  My hope is that it's the latter and that
// once arrays/domains are less special, this will fix itself and
// be removable.  In either case, I'm 95% positive that this
// is a pre-existing bug/condition that I've only now uncovered by
// virtue of inserting fewer temps and therefore creating fewer array
// copies/assignments, which cause there to be no other resolved
// references to here.id in user code.
