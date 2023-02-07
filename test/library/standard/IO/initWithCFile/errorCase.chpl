module m {
  require "cf2.h";

  use IO;
  use CTypes;

  extern proc openTestFile(): c_FILE;

  try! {
    var f = new file(openTestFile(), hints = ioHintSet.fromFlag(QIO_HINT_OWNED));
    var r = f.reader();
    write(r.readLine());
  } catch e: SystemError {
    // Note: the error message itself is not particularly clear
    writeln("Caught system error");
  }
}
