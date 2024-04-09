use IO;

// read the first line just for comparison
{
  var f = openReader("./snowmen.txt", locking=false);
  var s = f.readLine(stripNewline=true);
  writeln(s);
}

// the first line has 9 codepoints, so this should match
{
  var f = openReader("./snowmen.txt", locking=false);
  var s = f.readLine(maxSize=9, stripNewline=true);
  writeln(s);
}

// this is the main point of the test: readString(9) should
// also read the same 9 codepoints
{
  var f = openReader("./snowmen.txt", locking=false);
  var got = f.readString(9);
  writeln(got);
}
