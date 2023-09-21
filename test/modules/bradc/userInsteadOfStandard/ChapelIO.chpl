// some actual ChapelIO code copied to make standard library compile,
// since we have replaced ChapelIO with this file

use IO;

  proc chpl_stringify_wrapper(const args ...):string {
    use IO only chpl_stringify;
    return chpl_stringify((...args));
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

  proc writef(fmt:?t, const args ...?k)
    where isStringType(t) || isBytesType(t)
  {
    try! { stdout.writef(fmt, (...args)); }
  }

  //
  // Used by the compiler to support the compiler-generated initializers that
  // accept a 'fileReader'. The type 'fileReader' may not be readily
  // available, but the ChapelIO module generally is available and so
  // we place the check here. For example:
  //
  //   proc R.init(r) where chpl__isFileReader(r.type) { ... }
  //
  proc chpl__isFileReader(type T) param : bool {
    return isSubtype(T, fileReader(?));
  }

// function to test if this was the ChapelIO that got used
proc testchapelio() {
  writeln("In my ChapelIO!");
}
