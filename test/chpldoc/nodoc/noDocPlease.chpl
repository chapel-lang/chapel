module Foo {
  pragma "no doc"
  config var blah: int; // No comment, should have nothing printed out

  /* This config var has a comment before it. */
  pragma "no doc"
  config var blah2: int;

  pragma "no doc"
    /* This config var has a comment after the pragma */
  config var blah3: int;

  config var blah4: int; // Should be printed, but without comment

  pragma "no doc"
  proc a () {  }

  /* This function has a comment before it. */
  pragma "no doc"
  proc b () {  }

  pragma "no doc"
  /* This function has a comment after the pragma. */
  proc c () {  }

  proc d() {
    // Should be printed, but without comment
  }

  pragma "no doc"
  class alsoIgnoreMe {
    type t;
    var b: t;
  }

  pragma "no doc"
    /* This class has a comment after the pragma */
  class alsoIgnoreMe2 {
    type t;
    var b: t;
  }

  /* This class has a comment before the pragma */
  pragma "no doc"
  class alsoIgnoreMe3 {
    type t;
    var b: t;
  }

  class bar {
    // No class comment.  Should not grab previous class comment.

    pragma "no doc"
    var ignoreMe: bool;

    /* This field has a comment before it */
    pragma "no doc"
    var ignoreMe2: bool;

    pragma "no doc"
    /* This field has a comment after the pragma */
    var ignoreMe3: bool;

    var showMe: bool;
    // This is the only field that should be printed
  }

  /* This is an undocumented module. */
  pragma "no doc"
  module invisible {

    /* This class is undocumented, since its module is undocumented. */
    class MyC {}
  }
}

pragma "no doc"
module toBeIgnored {

  class otherwiseSeen {
    var andAField: string;
  }
}
