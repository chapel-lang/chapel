
private use List;
use MasonBuild;
use IO;

proc main() {

  
  const toml = open("Mason.toml", iomode.cw);
  var s      = toml.writer();

  s.writeln();
  s.writeln("[brick]");
  s.writeln('name = "foobar"');
  s.writeln('version = "0.1.0"');
  s.writeln('chplVersion = "1.0.0..1.0.0"');
  s.writeln();
  s.close();

  toml.close();

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

  var compopts: list(string);
  compopts.append("");
  buildProgram(false, false, false, compopts, "Mason.toml", "Mason.lock");
}
