
use IO;

var f = openTempFile();

var smile1 = 0x1F601;
var smile2 = 0x1F602;


{
  var w = f.writer();

  w.writeCodepoint(smile1);
  w.writeln();

  w.writef("%c\n", smile2);

  write("Writing ");
  stdout.writeCodepoint(smile1);
  write(" ");
  writef("%c", smile2);
  writeln();
}

{
  var r = f.reader();
  var got1, got2: int;

  got1 = r.readCodepoint();
  r.readNewline();

  r.readf("%c\n", got2);

  write("Reading ");
  stdout.writeCodepoint(got1);
  write(" ");
  writef("%c", got2);
  writeln();


  assert(got1 == smile1);
  assert(got2 == smile2);
}

