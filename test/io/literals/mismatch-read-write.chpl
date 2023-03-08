
use IO;

//
// Testing errors like 'write on read-only channel'
//

enum action {
  read,
  match,
  write,
}

config param useStrings = true;
config param useLiteral = true;
config param useAction = action.read;

proc main() {
  var f = openMemFile();
  param lit = if useStrings then "x" else b"x";
  var ch = if useAction == action.write then f.reader() else f.writer();

  if useLiteral {
    select useAction {
      when action.read do ch.readLiteral(lit);
      when action.match do ch.matchLiteral(lit);
      when action.write do ch.writeLiteral(lit);
    }
  } else {
    select useAction {
      when action.read do ch.readNewline();
      when action.match do ch.matchNewline();
      when action.write do ch.writeNewline();
    }
  }
}
