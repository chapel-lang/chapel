use IO;
use Map;
use Sort;

extern proc memcpy(x : [], b:c_string , len:int);

config const tableSize = 1 << 16;
config const lineSize = 61;

var tonum : [1..128] int;
tonum[0x41] = 0; // A
tonum[0x43] = 1; // C
tonum[0x54] = 2; // T
tonum[0x47] = 3; // G

var tochar : [0..3] string;
tochar[0] = "A";
tochar[1] = "C";
tochar[2] = "T";
tochar[3] = "G";

inline proc hash(str : [] uint(8), beg:int, sizeRange:range(?) ) {
  var data : uint = 0;
  for i in sizeRange {
    data <<= 2;
    data |= tonum[str[beg+i]];
  }
  return data;
}

proc decode(data : uint, size : int) {
  var ret : string;
  var d = data;
  for i in 1..size {
    ret = tochar[(d & 3) : uint(8)] + ret;
    d >>= 2;
  }
  return ret;
}

proc calculate(data : [] uint(8), size : int) {
  //var freqDom : domain(uint);
  var freqs = new map(uint, int);

  var lock : sync bool;
  lock = true;
  const sizeRange = 0..size-1;
  coforall tid in 1..here.maxTaskPar with (ref freqs) {
    //var curDom : domain(uint);
    var curMap = new map(uint, int);
    for i in tid .. data.size-size by here.maxTaskPar {
      curMap[hash(data, i, sizeRange)] += 1;
    }
    lock; // acquire lock
    for (k,v) in curMap.items() do freqs[k] += v;
    lock = true; // free lock
  }

  return freqs;
}

proc write_frequencies(data : [] uint(8), size : int) {
  var sum = data.size  - size;
  var freqs = calculate(data, size);

  // sort by frequencies
  var arr : [1..freqs.size] (int, uint);
  for (a, (k, v)) in zip(arr, freqs.items()) do
    a = (v,k);
  sort(arr, comparator=reverseComparator);

  for (f, s) in arr do
    writef("%s %.3dr\n", decode(s, size), (100.0 * f) / sum);
}

proc write_count(data : [] uint(8), str : string) {
  var freqs = calculate(data, str.numBytes);
  var d = hash(str.toBytes(), 1, 0..str.numBytes-1);
  writeln(freqs[d], "\t", decode(d, str.numBytes));
}

proc string.toBytes() {
   var b : [1..this.numBytes] uint(8);
   memcpy(b, this.c_str(), this.numBytes);
   return b;
}

inline proc startsWithThree(data : []) {
  return data[1] == 0x3E && data[2] == 0x54 && data[3] == 0x48;
}

proc main(args: [] string) {
  // Open stdin and a binary reader channel
  const inFile = openfd(0);
  const fileLen = inFile.size;
  var myin = inFile.reader(kind=ionative,locking=false);

  // Read line-by-line until we see a line beginning with '>TH'
  var tempdata : [1..lineSize] uint(8);
  var numRead = 0;
  var total = 0;
  while myin.readline(tempdata, numRead) && !startsWithThree(tempdata) { total += numRead; }

  // Read in the rest of the file
  var dataDom = {1..fileLen-total};
  var data : [dataDom] uint(8);
  var idx = 1;
  while myin.readline(data, numRead, idx) { idx += numRead - 1; }
  
  // Resize our array to the amount actually read
  dataDom = {1..idx};

  // Make everything uppercase
  forall d in data do d ^= 0x20;

  write_frequencies(data, 1);
  writeln();
  write_frequencies(data, 2);
  writeln();
  write_count(data, "GGT");
  write_count(data, "GGTA");
  write_count(data, "GGTATT");
  write_count(data, "GGTATTTTAATT");
  write_count(data, "GGTATTTTAATTTATAGT");
}
