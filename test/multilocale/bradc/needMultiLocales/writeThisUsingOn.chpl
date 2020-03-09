use Time;

class LocC {
  var id: int;
  
  proc writeThis(x) throws {
    on this {
      x.write(id);
    }
  }
}

class C {
  var locCs: [LocaleSpace] unmanaged LocC?;

  proc postinit() {
    for loc in LocaleSpace {
      on Locales(loc) {
        locCs(loc) = new unmanaged LocC(loc);
      }
    }
  }

  proc writeThis(x) throws {
    for loc in LocaleSpace {
      on Locales(loc) {
        if loc != 0 then
          write(" ");
        write(locCs(loc));
      }
    }
  }
}

// Once this test is working, only the first
// branch of this cobegin is necessary; the
// second branch is just to kill the test in
// a reasonable amount of time while it's not
// working.
cobegin {
  {
    var myC = new unmanaged C();
    writeln("C is: ", myC);
  }
  {
    sleep(10);
    halt("Timed out");
  }
}
