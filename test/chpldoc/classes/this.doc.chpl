// The reason for a special test for this() is that
// this() methods used to be handled in an odd manner.

// testing a class

class ThisTestClass {
  /* Primary 'ThisTestClass.this' method. */
  proc this(yesChpldoc1) {}

  proc this(noChpldoc1) {}
}

/* Secondary 'ThisTestClass.this' method. */
proc ThisTestClass.this(yesChpldoc2) {}

proc ThisTestClass.this(noChpldoc2) {}

// testing a record

record ThisTestRecord {
  /* Primary 'ThisTestRecord.this' method. */
  proc this(yesChpldoc1) {}

  proc this(noChpldoc1) {}
}

/* Secondary 'ThisTestRecord.this' method. */
proc ThisTestRecord.this(yesChpldoc2) {}

proc ThisTestRecord.this(noChpldoc2) {}
