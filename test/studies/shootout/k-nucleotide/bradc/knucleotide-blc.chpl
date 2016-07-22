config param tableSize = 1 << 16,
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


proc writeFreqs(data, size) {
  var sum = data.size  - size;
  var freqs = calculate(data, size);

  // sort by frequencies
  var arr: [1..freqs.size] (int, uint);
  for (a, k, v) in zip(arr, freqs.domain, freqs) do
    a = (v,k);
  QuickSort(arr, reverse=true);

  for (f, s) in arr do
    writef("%s %.3dr\n", decode(s, size), (100.0 * f) / sum);
  writeln();
}


//
// TODO: make these static to decode and hash respectively
//
const toChar: [0..3] string = ["A", "C", "T", "G"];
var toNum: [0..127] int;

toNum[ascii(toChar)] = toChar.domain;

inline proc hash(str, beg, sizeRange) {
  var data: uint = 0;
  for i in sizeRange {
    data <<= 2;
    data |= toNum[str[beg+i]];
  }
  return data;
}

proc decode(data, size) {
  var ret : string;
  var d = data;
  for i in 1..size {
    ret = toChar[(d & 3) : uint(8)] + ret;
    d >>= 2;
  }
  return ret;
}

proc calculate(data, size) {
  var freqDom: domain(uint);
  var freqs: [freqDom] int;

  var lock: sync bool;
  lock = true;
  const sizeRange = 0..size-1;
  coforall tid in 1..here.maxTaskPar {
    var curDom: domain(uint);
    var curArr: [curDom] int;
    for i in tid .. data.size-size by here.maxTaskPar {
      curArr[hash(data, i, sizeRange)] += 1;
    }
    lock; // acquire lock
    for (k,v) in zip(curDom, curArr) do freqs[k] += v;
    lock = true; // free lock
  }

  return freqs;
}

proc writeCount(data, str) {
  var freqs = calculate(data, str.length);
  var d = hash(str.toBytes(), 1, 0..str.length-1);
  writeln(freqs[d], "\t", decode(d, str.length));
}

proc string.toBytes() ref {
  extern proc memcpy(x: [], b, len);

  var b : [1..this.length] uint(8);
  memcpy(b, this.c_str(), this.length:size_t);
  return b;
}

inline proc startsWithThree(data) {
  return data[1] == ascii(">") && 
         data[2] == ascii("T") && 
         data[3] == ascii("H");
}

