
private use List;
use MasonBuild;
use IO;

proc main() {

  
  const toml = open("Mason.toml", ioMode.cw);
  var s      = toml.writer();

  s.writeln();
  s.writeln("[brick]");
  s.writeln('name = "foobar"');
  s.writeln('version = "0.1.0"');
  s.writeln('chplVersion = "1.0.0..1.0.0"');
  s.writeln();
  s.close();

  toml.close();

  const lock = open("Mason.lock", ioMode.cw);
  var w      = lock.writer();

  w.writeln();
  w.writeln("[root]");
  w.writeln('name = "foobar"');
  w.writeln('version = "0.1.0"');
  w.writeln('chplVersion = "1.0.0..1.0.0"');
  w.writeln();
  w.close();

  lock.close();

  var compopts: list(string);
  compopts.pushBack("");
  buildProgram(false, false, false, false, compopts, "Mason.toml", "Mason.lock");
}
