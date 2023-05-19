use IO;
use List;

proc testData(param testNum) {
  var f = openTempFile();
  var w = f.writer();
  var data: list(string);
  if testNum == 1 {
    data.append("hello from line 1\n");
    data.append("hello from line 2\n");
    data.append("hello from line 3 which is the last line\n");
  }
  else if testNum == 2 {
    // empty file
  }
  else if testNum == 3 {
    // one line with a newline
    data.append("hello world\n");
  }
  else if testNum == 4 {
    // file ending without a newline
    data.append("hello world");
  }
  for l in data do w.write(l);
  w.close();
  return (f, data);
}

proc test(param testNum, param stripNewline) {
  var (f, data) = testData(testNum);

  var r = f.reader();
  var lines = r.lines(stripNewline);
  assert(lines.size == data.size);
  for (lineFromFile, originalLine) in zip(lines, data) {
    var compareLine = if stripNewline then originalLine.strip() else originalLine;
    assert(lineFromFile == compareLine);
  }
  r.close();
  
  f.close();
}


for param i in 1..4 {
  test(i, true);
  test(i, false);
}
