
use MasonBuild;

proc main() {
  const lock = open("Mason.lock", iomode.cw);
  var w      = lock.writer();

  w.writeln();
  w.writeln("[root]");
  w.writeln('name = "foobar"');
  w.writeln('version = "0.1.0"');
  w.writeln('chplVersion = "1.0.0..1.0.0"');
  w.writeln();
  w.close();

  lock.close();

  var compopts = ["",];
  buildProgram(false, false, compopts);
}
