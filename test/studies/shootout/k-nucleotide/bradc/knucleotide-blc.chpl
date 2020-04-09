/* The Computer Language Benchmarks Game
   http://benchmarksgame.alioth.debian.org/

   contributed by Ben Harshbarger and Brad Chamberlain
   derived from the GNU C++ version by Branimir Maksimovic
*/

use Map, Sort, IO;

config param tableSize = 2**16,
             columns = 61;


proc main(args: [] string) {
  // Open stdin and a binary reader channel
  const consoleIn = openfd(0),
        fileLen = consoleIn.size,
        stdinNoLock = consoleIn.reader(kind=ionative, locking=false);

  // Read line-by-line until we see a line beginning with '>TH'
  var buff: [1..columns] uint(8),
      lineSize = 0,
      numRead = 0;

  while stdinNoLock.readline(buff, lineSize) && !startsWithThree(buff) do
    numRead += lineSize;

  // Read in the rest of the file
  var dataDom = {1..fileLen-numRead},
      data: [dataDom] uint(8),
      idx = 1;

  while stdinNoLock.readline(data, lineSize, idx) do
    idx += lineSize - 1;
  
  // Resize our array to the amount actually read
  dataDom = {1..idx};

  // Make everything uppercase
  forall d in data do
    d -= ("a".toByte() - "A".toByte());

  writeFreqs(data, 1);
  writeFreqs(data, 2);
  writeCount(data, "GGT");
  writeCount(data, "GGTA");
  writeCount(data, "GGTATT");
  writeCount(data, "GGTATTTTAATT");
  writeCount(data, "GGTATTTTAATTTATAGT");
}


proc writeFreqs(data, param nclSize) {
  const freqs = calculate(data, nclSize);

  // sort by frequencies
  //
  // TODO: Shouldn't this work?
  //
  //  var arr = [(k,v) in zip(freqs.domain, freqs)] (v,k);
  var arr = for (k,v) in freqs.items() do (v,k);

  //  var arr: [1..freqs.size] 2*int;
  //  for (a, k, v) in zip(arr, freqs.domain, freqs) do
  //    a = (v, k);

  // arr.sorted() creates another (temporary) array
  // ideally, would like "for (f,s) in
  //   ( for (s,f) in zip(freqs.domain, freqs) do (f,s) ).sorted(...)"

  for (f, s) in arr.sorted(comparator=reverseComparator) do
   writef("%s %.3dr\n", decode(s, nclSize), 
           (100.0 * f) / (data.size - nclSize));
  writeln();
}


proc writeCount(data, param str) {
  const freqs = calculate(data, str.numBytes),
        d = hash(str.toBytes(), 0, str.numBytes);

  writeln(freqs[d], "\t", decode(d, str.numBytes));
}


proc calculate(data, param nclSize) {
  var freqs = new map(int, int);

  //
  // TODO: Could we combine these local hash tables with a reduce
  // intent and use a forall intent?
  //

  var lock$: sync bool = true;
  const numTasks = here.maxTaskPar;
  coforall tid in 1..numTasks with (ref freqs) {
    var myMap = new map(int, int);

    for i in tid..(data.size-nclSize) by numTasks do
      myMap[hash(data, i, nclSize)] += 1;

    lock$;        // acquire lock
    for (k,v) in myMap.items() do
      freqs[k] += v;
    lock$ = true; // release lock
  }

  return freqs;
}


//
// TODO: make these static to decode and hash respectively
//
const toChar: [0..3] string = ["A", "C", "T", "G"];
var toNum: [0..127] int;

forall i in toChar.domain do
  toNum[toChar[i].toByte()] = i;
//
// Too terse (?): toNum[toChar.toByte()] = toChar.domain;


inline proc decode(in data, param nclSize) {
  var ret: string;

  for i in 1..nclSize {
    ret = toChar[(data & 3)] + ret;
    data >>= 2;
  }

  return ret;
}


inline proc hash(str, beg, param size) {
  var data = 0;

  for i in 0..size-1 {
    data <<= 2;
    data |= toNum[str[beg+i]];
  }

  return data;
}


proc string.toBytes() {
  var byteArr: [0..#this.numBytes] uint(8);
  for (b, i) in zip(byteArr, 0..) do
    b = this.byte(i);
  return byteArr;
}


inline proc startsWithThree(data) {
  return data[1] == ">".toByte() &&
         data[2] == "T".toByte() &&
         data[3] == "H".toByte();
}

