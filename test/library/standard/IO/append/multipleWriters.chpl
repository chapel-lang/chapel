use IO;

config var filename = "test.txt";

config var mode = ioMode.a;
config var nWriters = 5;
config param locking = true;

{
  var w = openWriter(filename);
  for i in 1..100 {
      w.writeln("original ", i);
      w.flush();
  }
  w.close();
}
{
  proc writeStrings(w: fileWriter, id: int) {
    // w.lock();
    for i in 1..100 {
      // w.lock();
      w.writeln("task ", id, " ", i);
      w.flush();
      // w.unlock();
    }
    // w.unlock();

  }
  var f = open(filename, mode);

  var writers: [0..<nWriters] fileWriter(iokind.dynamic,locking,nothing);
  for idx in 0..<nWriters {
    writers[idx] = f.writer(locking=locking);
  }
  coforall idx in 0..<nWriters {
    writeStrings(writers[idx], idx);
  }

  for idx in 0..<nWriters {
    writers[idx].close();
  }
  f.close();
}
{
  var r = openReader(filename);
  var line: string;
  while r.readLine(line) {
    write(line);
  }
  r.close();
}


