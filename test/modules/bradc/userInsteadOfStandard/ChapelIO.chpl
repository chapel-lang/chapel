// some actual ChapelIO code copied to make standard library compile,
// since we have replaced ChapelIO with this file

use IO;

  proc chpl_stringify_wrapper(const args ...):string {
    use IO only stringify;
    return stringify((...args));
  }

  /* Equivalent to ``try! stdout.write``. See :proc:`IO.fileWriter.write` */
  proc write(const args ...?n) {
    try! stdout.write((...args));
  }
  /* Equivalent to ``try! stdout.writeln``. See :proc:`IO.fileWriter.writeln` */
  proc writeln(const args ...?n) {
    try! stdout.writeln((...args));
  }

  // documented in the arguments version.
  @chpldoc.nodoc
  proc writeln() {
    try! stdout.writeln();
  }

// function to test if this was the ChapelIO that got used
proc testchapelio() {
  writeln("In my ChapelIO!");
}

