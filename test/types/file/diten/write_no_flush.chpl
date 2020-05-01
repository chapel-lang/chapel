use IO;

proc write32u(w, u: uint(32)) {
  var tmp: uint(8);
  tmp =         (u & 0xff): uint(8); w.write(tmp);
  tmp = ((u >> 8)  & 0xff): uint(8); w.write(tmp);
  tmp = ((u >> 16) & 0xff): uint(8); w.write(tmp);
  tmp = ((u >> 24) & 0xff): uint(8); w.write(tmp);
}

config param filename = "write_no_flush.tmp";

proc main {
  var f = open(filename, iomode.cw);
  var w = f.writer(kind=ionative);
  for i in 1..5000 {
    write32u(w, i:uint(32));
  }
  //w.flush();
  f.close();
  w; // mention w to prevent it from being deinited (and flushed) earlier
}
