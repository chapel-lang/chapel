module Bar {
  private config var blah: int; // No comment, should have nothing printed out

  /* This config var has a comment before it. */
  private config var blah2: int;

  config var blah3: int; // Should be printed, but without comment

  private proc a () {  }

  /* This function has a comment before it. */
  private proc b () {  }

  proc c() {
    // Should be printed, but without comment
  }

  /* This is an undocumented module. */
  private module invisible {

    /* This class is undocumented, since its module is undocumented. */
    class MyC {}
  }
}

private module toBeIgnored {

  class otherwiseSeen {
    var andAField: string;
  }
}
