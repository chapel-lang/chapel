/* The Computer Language Benchmarks Game
   http://benchmarksgame.alioth.debian.org/

   contributed by Ben Harshbarger
   derived from the Rust #2 version by Matt Brubeck
*/
private use IO, SysCTypes;

/*
   This is very ugly because we don't have good IO support for
   reading/writing buffers. The 'data' buffer is included in the
   'buf' record so that we know exactly how many bytes to read
   with 'readBytes'. This is so that we don't accidentally read
   past the end of the file and get an error. This would be bad
   because 'readBytes' doesn't tell us how many bytes it actually
   read...
*/

private use List;

config param columns = 61;

const table = initTable("ATCGGCTAUAMKRYWWSSYRKMVBHDDHBVNN\n\n");

record buf {
  const bufSize : int;
  var buf : [0..#bufSize] uint(8);
  var cur, cap, numLeft : int;
  var chan : channel(writing=false, kind=iokind.native, locking=false);

  proc init(fi:file, bs:int) {
    this.bufSize = bs;

    this.complete();

    chan = fi.reader(locking=false);
    numLeft = fi.size;
  }

  // Returns (by ref-ish) a slice of the buffer starting at 'low'
  pragma "no copy return"
  proc fill() {
    if cur >= cap {
      if numLeft > 0 {
        cap = min(bufSize, numLeft);
        chan.readBytes(c_ptrTo(buf), cap:ssize_t);
        numLeft -= cap;

        // ensure we return an empty slice if we run out of bytes
        cur = 0;
      } else {
        cur = 0;
        cap = 0;
      }
    }
    const low = if cap == 0 then 1 else cur;
    return buf[low..max(0, cap-1)];
  }

  proc consume(n : int) {
    cur = min(cur + n, cap);
  }

  proc _memchr(c : uint(8), arr : []) {
    extern proc memchr(s:c_void_ptr, c : c_int, n : size_t) : c_void_ptr;
    const ptr = c_ptrTo(arr);
    const ret = memchr(ptr, c:c_int, arr.size:size_t);
    if ret != c_nil {
      const idx = arr.domain.first + ret:c_intptr - ptr:c_intptr;
      return idx;
    }
    return -1;
  }

  proc readUntil(term : uint(8), ref data : list(uint(8))) : int {
    var read = 0;
    while true {
      var done = false, used = 0;
      ref avail = fill();
      if avail.size > 0 {
        const idx = _memchr(term, avail);
        if idx >= 0 {
          // Character found, bulk-append characters up to and including 'idx'
          // to the 'data' array.
          data.extend(avail[..idx]);
          (done, used) = (true, avail[..idx].size);
        } else {
          data.extend(avail);
          (done, used) = (false, avail.size);
        }
      } else return 0;
      consume(used);
      read += used;
      if done || used == 0 then return read;
    }
    return 0;
  }
}

config const readSize = 16 * 1024;

proc main(args: [] string) {
  const stdin = openfd(0);
  var input = new buf(stdin, readSize);
  var data: list(uint(8));
  
  // Use undocumented internals to fake a request for capacity.
  // Sets up 'data' to have an underlying capacity equal to the size of the
  // input file.
  // NOTE: We can't do this with lists yet.
  /*
  {
    const r = 1..stdin.size;
    data._value.dataAllocRange = r;
    data._value.dsiReallocate((r,));
    data._value.dsiPostReallocate();
  }
  */

  sync {     // wait for all process() tasks to complete before continuing
    while true {
      input.readUntil("\n".toByte(), data);
      const start = data.size + 1;
      input.readUntil(">".toByte(), data);
      const last = data.size;

      if data[last] == ">".toByte() {
        // '-2' to skip over '\n>'
        begin with (ref data) process(data, start, last-2);
      } else {
        // Final section
        // '-1' to skip over '\n'
        begin with (ref data) process(data, start, last-1);
        break;
      }
    }
  }

  const stdoutBin = openfd(1).writer(iokind.native, locking=false, 
                                     hints=QIO_CH_ALWAYS_UNBUFFERED);
  //
  // This conversion wastes memory, but correct output requires array stdout
  // specifically at the moment.
  //
  stdoutBin.write(data.toArray());
}

proc process(ref data, in start, in end) {
  proc advance(ref cursor, dir) {
    do { cursor += dir; } while data[cursor] == "\n".toByte();
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

  for i in 1..pairs.numBytes by 2 {
    table[pairs.byte(i)] = pairs.byte(i+1);
    if pairs.byte(i) != "\n".toByte() then
      table[pairs[i:byteIndex].toLower().toByte()] = pairs.byte(i+1);
  }

  return table;
}
