use IO;

if unicodeSupported() {
  var f = opentmp();

  var euro = new ioChar(0x20ac); // euro sign "?";
  var got = new ioChar(0);

  writeln("Writing ", euro);
  var writer = f.writer();
  writer.write(euro);
  writer.close();

  var reader = f.reader();
  reader.read(got);
  reader.close();

  writeln("Read  ", got);

  assert( got == euro );

} else {
  writeln("Test omitted because unicode is not supported (set e.g. LC_CTYPE=\"en_US.UTF-8\")");
}
