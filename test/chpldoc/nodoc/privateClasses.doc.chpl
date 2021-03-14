private class alsoIgnoreMe {
  type t;
  var b: t;
}

/* This class has a comment before the pragma */
private class alsoIgnoreMe2 {
  type t;
  var b: t;
}

class bar {
  // No class comment.  Should not grab previous class comment.

  private var ignoreMe: bool;

  /* This field has a comment before it */
  private var ignoreMe2: bool;

  var showMe: bool;
  // This is the only field that should be printed
}

