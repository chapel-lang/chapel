/* The Computer Language Benchmarks Game
   http://benchmarksgame.alioth.debian.org/

   contributed by Ben Harshbarger
   derived from the GNU C++ version by Branimir Maksimovic
*/
use Map, Sort, IO;

// Used to encode a string into a uint
var tonum : [0..127] int;
tonum["A".toByte()] = 0;
tonum["C".toByte()] = 1;
tonum["T".toByte()] = 2;
tonum["G".toByte()] = 3;

// Used to decode a uint back into a string
var tochar : [0..3] string;
tochar[0] = "A";
tochar[1] = "C";
tochar[2] = "T";
tochar[3] = "G";

inline proc hash(data : string) {
  var e : uint = 0;
  for d in data {
    e <<= 2;
    e |= tonum[d.toByte()];
  }
  return e;
}

proc decode(in data : uint, size : int) {
  var ret : string;
  for 1..size {
    ret = tochar[(data & 3):int] + ret;
    data >>= 2;
  }
  return ret;
}

proc calculate(data : string, size : int) {
  //
  // Create an associative array to store the results of each task.
  // Disables the parallel safety feature to achieve better performance. We
  // will avoid races through the use of locks.
  //
  var freqs = new map(uint, int);

  // Create a lock and release it.
  var lock$ : sync bool = true;

  const high = data.size - size + 1;

  coforall tid in 1..here.maxTaskPar with (ref freqs) {
    var privMap = new map(uint, int);

    for i in tid..high by here.maxTaskPar {
      //
      // Assigning to an index in an associative array will create an
      // index/element pair if one does not already exist.
      //
      privMap[hash(data[i:byteIndex..#size])] += 1;
    }

    lock$;                                  // read to acquire lock
    for (k,v) in privMap.items() do
      freqs[k] += v;                        // accumulate into returned array
    lock$ = true;                           // write to release lock
  }

  return freqs;
}

proc write_frequencies(data : string, size : int) {
  var freqs = calculate(data, size);

  //
  // To sort by frequencies, create a temporary array of tuples, where the
  // first element is the frequency and the second element is the encoded
  // string.
  //
  var sorted : [1..freqs.size] (int, uint);
  for (s, (e, f)) in zip(sorted, freqs.items()) do
    s = (f, e);

  // sort will sort starting at the tuple's first element.
  sort(sorted, comparator=reverseComparator);

  const sum = data.size - size;
  for (f, e) in sorted do
    writef("%s %.3dr\n", decode(e, size), (100.0 * f) / sum);
}

proc write_count(data : string, pattern : string) {
  const size = pattern.numBytes;
  var freqs = calculate(data, size);
  const d = hash(pattern[1:byteIndex..#size]);
  writeln(freqs[d], "\t", decode(d, size));
}

proc main(args: [] string) {
  var data, buf : string;

  // Read each line until the desired section
  while readln(buf) && !buf.startsWith(">THREE") do {}
  
  // Append all remaining lines (excluding \n)
  while readln(buf) do data += buf;

  data = data.toUpper();

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
