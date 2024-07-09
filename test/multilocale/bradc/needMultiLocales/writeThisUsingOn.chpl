use Time;

class LocC : writeSerializable {
  var id: int;
  
  override proc serialize(writer, ref serializer) throws {
    on this {
      writer.write(id);
    }
  }
}

class C : writeSerializable {
  var locCs: [LocaleSpace] unmanaged LocC?;

  proc postinit() {
    for loc in LocaleSpace {
      on Locales(loc) {
        locCs(loc) = new unmanaged LocC(loc);
      }
    }
  }

  override proc serialize(writer, ref serializer) throws {
    for loc in LocaleSpace {
      on Locales(loc) {
        if loc != 0 then
          writer.write(" ");
        writer.write(locCs(loc));
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
