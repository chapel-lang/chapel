module m {
  require "cf2.h";

  use IO;
  use CTypes;

  // If this test starts randomly segfaulting due to touching bad memory, it's
  // okay to remove it
  extern proc openTestFile(): c_FILE;

  try! {
    var f = new file(openTestFile(), own=true);
    var r = f.reader();
    write(r.readLine());
  } catch e: SystemError {
    // Note: the error message itself is not particularly clear
    writeln("Caught system error");
  }
}
