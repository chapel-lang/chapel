
use IO;

proc main() {
  var f = openmem();
  var w = f.writer();
  for i in 1..10 do w.writeln(i);
  w.close();

  {
    var ch = f.reader();
    var ir = ch.itemReader(int);
    for i in ir do writeln(i);
  }
  {
    var ch = f.reader();
    var ir = new ItemReader(int, ch.kind, ch.locking, ch);
    for i in ir do writeln(i);
  }
  {
    // Make sure we don't throw any warnings for this case, which used to use
    // the deprecated method/type 
    var ch = f.reader();
    for i in ch.lines() do writeln(i.strip());
  }
}
