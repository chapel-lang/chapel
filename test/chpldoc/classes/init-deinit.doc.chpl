// testing a class

class ThisTestClass {
  /* initializer as a primary method */
  proc init(yesChpldoc1) {}

  proc init(noChpldoc1) {}

  /* deinitializer as a primary method */
  proc deinit() {}

  proc deinit() {}
}

/* initializer as a secondary method */
proc ThisTestClass.init(yesChpldoc2) {}

proc ThisTestClass.init(noChpldoc2) {}

/* deinitializer as a secondary method */
proc ThisTestClass.deinit() {}

proc ThisTestClass.deinit() {}

// testing a record

record ThisTestRecord {
  /* initializer as a primary method */
  proc init(yesChpldoc1) {}

  proc init(noChpldoc1) {}

  /* deinitializer as a primary method */
  proc deinit() {}

  proc deinit() {}
}

/* initializer as a secondary method */
proc ThisTestRecord.init(yesChpldoc2) {}

proc ThisTestRecord.init(noChpldoc2) {}

/* deinitializer as a secondary method */
proc ThisTestRecord.deinit() {}

proc ThisTestRecord.deinit() {}
