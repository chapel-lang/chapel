
//
// This test exists to lock in behavior regarding how global variables are
// captured by task-intent, and how that capture can hide modifications of that
// global elsewhere. See issue #21952 for discussion on this behavior.
//
module M {
  use Other;
  use AllLocalesBarriers;

  proc main() {
    // 'myGlobal' captured by 'const in', so the change isn't observed.
    coforall loc in Locales do on loc {
      setup();
      allLocalesBarrier.barrier();
      writeln(myGlobal); // 0
    }

    myGlobal = 0; // reset

    coforall loc in Locales with (ref myGlobal) do on loc {
      setup();
      allLocalesBarrier.barrier();
      writeln(myGlobal); // 42
    }
  }
}

//
// Note: this test also serves to test against an old bug in ``convert-uast``
// involving the conversion order of modules. In particular, there was an
// issue finding 'myGlobal' while converting the task-intent.
//
module Other {
  config var myGlobal = 0;
  proc setup() {
    if here.id == 0 then
      myGlobal = 42;
  }
}
