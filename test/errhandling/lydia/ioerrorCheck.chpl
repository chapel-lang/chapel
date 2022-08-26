use OS;

proc main() {
  try! {
    ioerror("blah blah", "blah blah blah", "unknown", -1);
  } catch e: IoError {
    writeln("caught the right subclass");
  } catch e : SystemError {
    writeln("Hey, wait a minute!  That didn't throw an IoError!");
    writeln(e.details);
  }
}
