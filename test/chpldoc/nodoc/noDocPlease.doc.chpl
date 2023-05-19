module Foo {
  @chpldoc.nodoc
  config var blah: int; // No comment, should have nothing printed out

  /* This config var has a comment before it. */
  @chpldoc.nodoc
  config var blah2: int;

  @chpldoc.nodoc
    /* This config var has a comment after the pragma */
  config var blah3: int;

  config var blah4: int; // Should be printed, but without comment

  /* This type should not be doc'd. */
  @chpldoc.nodoc
  type myint = int(64);

  /* This external type should not be doc'd. */
  @chpldoc.nodoc
  extern type exT = 2*real(64);

  /* This opaque external type should not be doc'd. */
  @chpldoc.nodoc
  extern type MyExT;

  @chpldoc.nodoc
  proc a () {  }

  /* This function has a comment before it. */
  @chpldoc.nodoc
  proc b () {  }

  @chpldoc.nodoc
  /* This function has a comment after the pragma. */
  proc c () {  }

  proc d() {
    // Should be printed, but without comment
  }

  @chpldoc.nodoc
  class alsoIgnoreMe {
    type t;
    var b: t;
  }

  @chpldoc.nodoc
    /* This class has a comment after the pragma */
  class alsoIgnoreMe2 {
    type t;
    var b: t;
  }

  /* This class has a comment before the pragma */
  @chpldoc.nodoc
  class alsoIgnoreMe3 {
    type t;
    var b: t;
  }

  class bar {
    // No class comment.  Should not grab previous class comment.

    @chpldoc.nodoc
    var ignoreMe: bool;

    /* This field has a comment before it */
    @chpldoc.nodoc
    var ignoreMe2: bool;

    @chpldoc.nodoc
    /* This field has a comment after the pragma */
    var ignoreMe3: bool;

    var showMe: bool;
    // This is the only field that should be printed
  }

  /* This is an undocumented module. */
  @chpldoc.nodoc
  module invisible {

    /* This class is undocumented, since its module is undocumented. */
    class MyC {}
  }
}

@chpldoc.nodoc
module toBeIgnored {

  class otherwiseSeen {
    var andAField: string;
  }
}
