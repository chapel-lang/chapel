/* The Computer Language Benchmarks Game
   http://benchmarksgame.alioth.debian.org/

   contributed by Ben Harshbarger
   derived from the GNU C version by Jeremy Zerfas
*/

/*
   This is very ugly because we don't have good IO support for
   reading/writing buffers. The 'data' buffer is included in the
   'buf' record so that we know exactly how many bytes to read
   with 'readBytes'. This is so that we don't accidentally read
   past the end of the file and get an error. This would be bad
   because 'readBytes' doesn't tell us how many bytes it actually
   read...
*/

config param columns = 61;

const table = initTable("ATCGGCTAUAMKRYWWSSYRKMVBHDDHBVNN\n\n");

record buf {
  var chan : channel(writing=false, kind=iokind.native, locking=false);
  var cap : int;
  var dom : domain(1);
  var data : [dom] uint(8);
  var chunkSize : int;
  var cursor : int;
  var numRead : int;

  proc init(fi, chunk:int) {
    chan = fi.reader(locking=false);
    this.cap = fi.length();
    dom = {1..cap};
    cursor = 1;
    chunkSize = chunk;
  }

  proc size return if numRead == 0 then 0 else cursor;

  proc readUntil(term : uint(8)) {
    assert(size <= cap);
    if size == cap then return;
    var found = false;
    proc advance() {
      if numRead == 0 then return false;
      extern proc memchr(s:c_void_ptr, c : c_int, n : size_t) : c_void_ptr;
      const dptr = c_pointer_return(data[cursor]);
      const ret = memchr(dptr, term:c_int, (numRead-cursor+1):size_t);
      if ret != c_nil {
        const idx = cursor + (ret:int - dptr:int);
        cursor = idx;
        return true;
      }
      cursor = numRead;
      return false;
    }
    while advance() == false && numRead < cap {
      const len = min(chunkSize, cap - numRead);
      chan.readBytes(c_ptrTo(data[numRead+1..]), len);
      numRead += len;
    }
  }
}

config const readSize = 16 * 1024;

proc main(args: [] string) {
  var b = new buf(openfd(0), readSize);

  sync {     // wait for all process() tasks to complete before continuing
    var numRead: int;
    while true {
      var err : syserr;
      b.readUntil(ascii("\n"));
      const start = b.size+1;
      b.readUntil(ascii(">"));
      const last = b.size;
      ref data = b.data;

      if data[last] == ascii(">") {
        begin process(data, start, last-2);
      } else {
        begin process(data, start, last-1);
        break;
      }
    }
  }

  const stdoutBin = openfd(1).writer(iokind.native, locking=false, 
                                     hints=QIO_CH_ALWAYS_UNBUFFERED);
  stdoutBin.write(b.data);
}

proc process(data, in start, in end) {
  proc advance(ref cursor, dir) {
    do { cursor += dir; } while data[cursor] == ascii("\n");
  }
  while start <= end {
    ref d1 = data[start], d2 = data[end];
    (d1, d2) = (table[d2], table[d1]);
    advance(start, 1);
    advance(end, -1);
  }
}

proc initTable(pairs) {
  var table: [1..128] uint(8);

  for i in 1..pairs.length by 2 {
    table[ascii(pairs[i])] = ascii(pairs[i+1]);
    if pairs[i] != "\n" then
      table[ascii(pairs[i].toLower())] = ascii(pairs[i+1]);
  }

  return table;
}
