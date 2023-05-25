use IO, Map, Sort;

config param tableSize = 2**16,
             columns = 61;

record intWrapper {
  var val: int;
  proc init() {
    val = 0;
  }
  proc init(i: int) {
    val = i;
  }
  proc hash() {
    return val;
  }
}

proc main(args: [] string) {
  // Open stdin and a binary reader channel
  const consoleIn = new file(0),
        fileLen = consoleIn.size,
        stdinNoLock = consoleIn.reader(kind=ionative, locking=false);

  // Read line-by-line until we see a line beginning with '>TH'
  var buff: [1..columns] uint(8),
      lineSize = 0,
      numRead = 0;

  lineSize = stdinNoLock.readLine(buff);
  while lineSize>0 && !startsWithThree(buff) {
    numRead += lineSize;
    lineSize = stdinNoLock.readLine(buff);
  }
  // Read in the rest of the file
  var dataDom = {1..fileLen-numRead},
      data: [dataDom] uint(8),
      idx = 1;

  lineSize = stdinNoLock.readLine(data[idx..]);
  while lineSize>0 {
    idx += lineSize - 1;
    lineSize = stdinNoLock.readLine(data[idx..]);
  }
  
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

  var arr = for (s,f) in zip(freqs.keys(), freqs.values()) do (f,s.val);

  // sort by frequencies

  for (f, s) in sorted(arr, comparator=reverseComparator) do
   writef("%s %.3dr\n", decode(s, nclSize), 
           (100.0 * f) / (data.size - nclSize));
  writeln();
}


proc writeCount(data, param str) {
  const freqs = calculate(data, str.numBytes),
        d = hash(str.toBytes(), 0, str.numBytes);

  var a = new intWrapper(d);
  writeln(freqs[a], "\t", decode(d, str.numBytes));
}


proc calculate(data, param nclSize) {
  var freqs = new map(intWrapper, int);

  var lock$: sync bool = true;
  const numTasks = here.maxTaskPar;
  coforall tid in 1..numTasks with (ref freqs) {
    var myArr = new map(intWrapper, int);

    for i in tid..(data.size-nclSize) by numTasks {
      var a = new intWrapper(hash(data, i, nclSize));
      myArr[a] += 1;
    }

    lock$.readFE();        // acquire lock
    for (k,v) in zip(myArr.keys(), myArr.values()) {
      freqs[k] += v;
    }
    lock$.writeEF(true); // release lock
  }

  return freqs;
}


const toChar: [0..3] string = ["A", "C", "T", "G"];
var toNum: [0..127] int;

forall i in toChar.domain do
  toNum[toChar[i].toByte()] = i;


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
