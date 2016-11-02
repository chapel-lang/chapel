/* The Computer Language Benchmarks Game
   http://benchmarksgame.alioth.debian.org/

   contributed by Ben Harshbarger and Brad Chamberlain
   derived from the GNU C++ version by Branimir Maksimovic
*/

use Sort;

config param tableSize = 2**16,
             columns = 61;


proc main(args: [] string) {
  // Open stdin and a binary reader channel
  const consoleIn = openfd(0),
        fileLen = consoleIn.length(),
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
    d -= (ascii("a") - ascii("A"));

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
  var arr = for (k,v) in zip(freqs.domain, freqs) do (v,k);

  quickSort(arr, comparator=reverseComparator);

  for (f, s) in arr do
   writef("%s %.3dr\n", decode(s, nclSize), 
           (100.0 * f) / (data.size - nclSize));
  writeln();
}


proc writeCount(data, param str) {
  const freqs = calculate(data, str.length),
        d = hash(str.toBytes(), 1, str.length);

  writeln(freqs[d], "\t", decode(d, str.length));
}


proc calculate(data, param nclSize) {
  var freqDom: domain(int, parSafe=false),
      freqs: [freqDom] int;

  var lock$: sync bool = true;
  const numTasks = here.maxTaskPar;
  coforall tid in 1..numTasks {
    var myDom: domain(int, parSafe=false),
        myArr: [myDom] int;

    for i in tid..(data.size-nclSize) by numTasks do
      myArr[hash(data, i, nclSize)] += 1;

    lock$;        // acquire lock
    for (k,v) in zip(myDom, myArr) do
      freqs[k] += v;
    lock$ = true; // release lock
  }

  return freqs;
}


const toChar: [0..3] string = ["A", "C", "T", "G"];
var toNum: [0..127] int;

forall i in toChar.domain do
  toNum[ascii(toChar[i])] = i;


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
  var bytes: [1..this.length] uint(8);
  for (b, i) in zip(bytes, 1..) do
    b = ascii(this[i]);
  return bytes;
}


inline proc startsWithThree(data) {
  return data[1] == ascii(">") && 
         data[2] == ascii("T") && 
         data[3] == ascii("H");
}

