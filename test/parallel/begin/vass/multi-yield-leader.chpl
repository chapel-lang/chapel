/*
This test is a trimmed-down version of
  multilocale/diten/oneOrMoreLocales/assocArrayOfRemoteArithArray.chpl

The point is to have a 'begin' within a 'forall', whose leader iterator
has multiple yields.
The array is here to test correctness of autocopies/destroys.
Just two yields within the leader would be sufficient, but
it seems an 'if' is also needed. A couple of extra yields are just frills.
*/

config const x = 0;  // ensure it's not known to the compiler

// viter (Vass's iter): a leader with multiple yields
iter viter() {
  yield 0;
}
iter viter(param tag: iterKind) where tag == iterKind.leader {
  if x != 0 then
    yield 111;
  else {
    yield x;
    yield x+1;
    yield x+2;
  }
}
iter viter(param tag: iterKind, followThis) where tag == iterKind.follower {
  yield followThis;
}

// 'forall' and the array are within a function
proc test() {
  var AA7: [0..2] real;
  sync {
    forall idx7 in viter() do begin {
        AA7(idx7) = 77;
      }
  }
  writeln(AA7);
}
test();

// now the same at the top level
var AA9: [0..2] real;
sync {
  forall idx9 in viter() do begin {
      AA9(idx9) = 99;
    }
}
writeln(AA9);
