/* The Computer Language Benchmarks Game
   https://salsa.debian.org/benchmarksgame-team/benchmarksgame/

   contributed by Brad Chamberlain
   based on the C gcc #5 version by Mr Ledrug
*/

/*
#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>

void process(char *from, char *to) {
   while (*from++ != '\n');

   size_t len = to - from;
   size_t off = 60 - (len % 61);

   if (off) {
      char *m;
      for (m = from + 60 - off; m < to; m += 61) {
         memmove(m + 1, m, off);
         *m = '\n';
      }
   }

   char c;
   for (to--; from <= to; from++, to--)
      c = tbl[(int)*from], *from = tbl[(int)*to], *to = c;
}

int main() {
   char *s;
   for (s = pairs; *s; s += 2) {
      tbl[toupper(s[0])] = s[1];
      tbl[tolower(s[0])] = s[1];
   }


   const size_t _1M = 1024 * 1024;
   size_t buflen = 8 * 1024, len, end = 0;
   char *buf = malloc(buflen);

   int in = fileno(stdin);
   while ((len = read(in, buf + end, buflen - 256 - end))) {
      if (len < 0) break;
      end += len;
      if (end >= buflen - 256) {
         buflen = (buflen >= _1M) ? buflen + _1M : buflen * 2;
         buf = realloc(buf, buflen);
      }
   }
   buf[end] = '>';

   char *from, *to = buf + end - 1;
   while (1) {
      for (from = to; *from != '>'; from--);

      process(from, to);

      to = from - 1;
      if (to < buf) break;
   }

   write(fileno(stdout), buf, end);
   free(buf);

   return 0;
}
*/


param eol = "\n".toByte();      // end-of-line, as an integer

const table = createTable();    // create the table of code complements

// move to local var
  use IO;
  var stdoutBin = openfd(1).writer(iokind.native, locking=false,
                                   hints=QIO_CH_ALWAYS_UNBUFFERED);

proc main(args: [] string) {
  
  param _1M = 1024**2;
  var bufLen = 8 * 1024;
  var bufDom = {0..<bufLen};
  var buf: [bufDom] uint(8);
  var end = 0;

  const stdin = openfd(0),
        input = stdin.reader(iokind.native, locking=false,
                             hints = QIO_CH_ALWAYS_UNBUFFERED);
  do {
    const success = input.read(buf[end..]);
    if success {
      end = bufLen;
      bufLen += if bufLen >= _1M then _1M else bufLen;;
      bufDom = {0..<bufLen};
    }
  } while (success);

  //  stdoutBin.writeln(buf);
  
  for i in buf.indices by -1 {
    if buf[i] != 0 {
      end = i+1;
      break;
    }
  }

  // TODO: Is this conditional necessary?
  if end {  
    var from, to = end-1;
    
    do {
      from = to;
      while buf[from] != '>'.toByte() do
        from -= 1;

      process(buf, from, to);

      to = from - 1;
    } while (to >= 0);

    stdoutBin.writeln(buf[..<end]);
  }
}

proc process(buf, in from, in to) {
  writeln((from,to));
  /*
  for i in from..to do
    writeln(buf[i]);
  exit(0);
  */
  do {
    from += 1;
  } while buf[from-1] != eol;
  writeln((from,to));

  const len = to - from,
        off = 60 - (len % 61);

  writeln("Before:");
  stdout.flush();
  stdoutBin.writeln(buf[from..#3]);
  stdoutBin.writeln(buf[..to#-3]);
  stdoutBin.writeln(buf[from..to]);
  
  if off {
    for m in from+60-off..<to by 61 {
      writeln((m,off));
      for i in m..#off by -1 do
        buf[i+1] = buf[i];
      buf[m] = eol;
      //      writeln("During:");
      //      stdout.flush();
      //      stdoutBin.writeln(buf[from..to]);
    }
  }

  writeln("After:");
  stdout.flush();
  stdoutBin.writeln(buf[from..to]);

  to -= 1;
  do {
    ref d1 = buf[from],
        d2 = buf[to];

    (d1, d2) = (table[d2], table[d1]);

    from += 1;
    to -= 1;
  } while (from <= to);
}
/*
void process(char *from, char *to) {
   while (*from++ != '\n');

   size_t len = to - from;
   size_t off = 60 - (len % 61);

   if (off) {
      char *m;
      for (m = from + 60 - off; m < to; m += 61) {
         memmove(m + 1, m, off);
         *m = '\n';
      }
   }

   char c;
   for (to--; from <= to; from++, to--)
      c = tbl[(int)*from], *from = tbl[(int)*to], *to = c;
}
*/
/*

config const readSize = 16384;  // the chunk size to read at a time

// a channel and coordination variable for writing data to stdout
var stdoutBin = openfd(1).writer(iokind.native, locking=false,
                                 hints=QIO_CH_ALWAYS_UNBUFFERED),
    seqToWrite: atomic int = 1;


proc main(args: [] string) {
  const stdin = openfd(0),
        input = stdin.reader(iokind.native, locking=false,
                             hints = QIO_CH_ALWAYS_UNBUFFERED);

  var curSeq = new Seq?(id=1);

  do {
    const eof = curSeq!.readChunk(input);

    // look for one or more sequences in the chunk we just read
    do {
      const (foundSeq, moreSeqs) = curSeq!.processChunk();

      if foundSeq {
        // if the chunk completed a sequence, save it away and copy the
        // remainder into the next one
        var lastSeq = curSeq;
        curSeq = lastSeq!.copyTail();

        // then fire off an asynchronous task to process the last sequence
        begin with (in lastSeq) {
          lastSeq!.revcomp();
        }
      }
    } while moreSeqs;
  } while !eof;
}

// a class representing a sequence
class Seq {
  const id = 0;              // the sequence ID
  var cursor = 0,            // a cursor for reading the next chunk
      last = 0,              // the index of the last byte in the chunk
      start = -1,            // the start of the sequence
      end = -1,              // the end of the sequence
      dataLen = readSize,    // the size of the data buffer
      inds = {0..#dataLen},  // the domain describing the data buffer
      data: [inds] uint(8);  // the data buffer

  // read a chunk of bytes into the data buffer from the input channel;
  // returns whether we've seen EOF (true) or not (false)
  proc readChunk(input) {
    last = cursor + readSize - 1;
    // if we don't have enough space for the last byte, double the data buffer
    if last >= dataLen {
      dataLen *= 2;
      inds = {0..#dataLen};
    }
    return !input.read(data[cursor..last]);
  }

  // process the chunk from 'cursor' through 'last' looking for '>'s or '\0's;
  // returns a pair of bools where the first indicates whether or not we found
  // a sequence and the second indicates whether there may yet be more.
  proc processChunk() {
    for i in cursor..last {
      if data[i] == ">".toByte() {  // if we find '>'
        if start == -1 {            // and haven't started a sequence yet
          start = i;                // this is the start
        } else {
          end = i-1;                // otherwise, it starts the next one
          return (true, true);      // we found a sequence and there's more
        }
      } else if data[i] == 0 {      // if we find '\0', the file has ended
        end = i-1;
        if start != -1 then
          return (true, false);     // we found a sequence and there's no more
      }
    }

    // we did not find the end of a sequence, so advance the cursor to
    // prepare for the next read
    cursor = last+1;
    return (false, false);          // we didn't find a sequence
  }

  // copy the unused tail of our data buffer into a new sequence
  proc copyTail() {
    const tailLen = last - end;
    var newSeq = new Seq(id = id+1, cursor = 0, last = tailLen-1);
    newSeq.data[0..#tailLen] = data[end+1..#tailLen];

    return newSeq;
  }

  // reverse and complement the sequence, then write out the result
  proc revcomp() {
    last = end;                  // snapshot the end of the sequence

    // advance 'start' to the first end-of-line
    do {
      start += 1;
    } while data[start] != eol;

    while start < end {
      // swap the end values, replacing them with their complements
      ref d1 = data[start],
          d2 = data[end];

      (d1, d2) = (table[d2], table[d1]);

      // advance the cursors, skipping over end-of-line characters
      advance(start, 1);
      advance(end, -1);

      proc advance(ref idx, dir) {
        do {
          idx += dir;
        } while data[idx] == eol;
      }
    }

    // write the sequence to stdout
    seqToWrite.waitFor(id);         // wait for our ID's turn to write
    stdoutBin.write(data[..last]);  // write the transformed data
    seqToWrite.write(id+1);         // make it the next sequence's turn
  }
}
*/

proc createTable() {
  // `pairs` compactly represents the table we're creating, where the
  // first byte of each pair (in either case) maps to the second:
  //   A|a -> T, C|c -> G, G|g -> C, T|t -> A, etc.
  param pairs = b"ATCGGCTAUAMKRYWWSSYRKMVBHDDHBVNN\n\n",
        upperToLower = "a".toByte() - "A".toByte();

  var table: [0..127] uint(8);

  table[eol] = eol;
  for i in pairs.indices by 2 {
    const src = pairs[i],
          dst = pairs[i+1];

    table[src] = dst;
    if src != eol then
      table[src+upperToLower] = dst;
  }

  return table;
}
