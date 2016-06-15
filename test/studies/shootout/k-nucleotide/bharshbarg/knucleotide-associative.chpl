/* The Computer Language Benchmarks Game
   http://benchmarksgame.alioth.debian.org/

   contributed by Ben Harshbarger
   derived from the GNU C++ version by Branimir Maksimovic
*/

// Used to encode a string into a uint
var tonum : [0..127] int;
tonum[ascii("A")] = 0;
tonum[ascii("C")] = 1;
tonum[ascii("T")] = 2;
tonum[ascii("G")] = 3;

// Used to decode a uint back into a string
var tochar : [0..3] string;
tochar[0] = "A";
tochar[1] = "C";
tochar[2] = "T";
tochar[3] = "G";

inline proc hash(data : string, sizeRange:range) {
  var d : uint = 0;
  for i in sizeRange {
    d <<= 2;
    d |= tonum[data.buff[i-1]];
  }
  return d;
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
  var freqDom : domain(uint, parSafe = false);
  var freqs : [freqDom] int;

  // Create and free a lock
  var lock$ : sync bool = true;
  const high = data.length - size + 1;

  coforall tid in 1..here.maxTaskPar {
    var privDom : domain(uint, parSafe = false);
    var privArr : [privDom] int;

    for i in tid..high by here.maxTaskPar {
      // Assignment into an associative array creates an index an element
      privArr[hash(data, i..#size)] += 1;
    }

    lock$;                                  // read to acquire lock
    for (k,v) in zip(privDom, privArr) do
      freqs[k] += v;                        // accumulate into returned array
    lock$ = true;                           // write to free lock
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
  for (s, e, f) in zip(sorted, freqs.domain, freqs) do
    s = (f, e);

  // QuickSort will sort starting at the tuple's first element.
  QuickSort(sorted, reverse=true);

  const sum = data.length - size;
  for (f, e) in sorted do
    writef("%s %.3dr\n", decode(e, size), (100.0 * f) / sum);
}

proc write_count(data : string, pattern : string) {
  const size = pattern.length;
  var freqs = calculate(data, size);
  const d = hash(pattern, 1..size);
  writeln(freqs[d], "\t", decode(d, size));
}

proc main() {
  var data, buf : string;

  // Read each line until the desired section
  while stdin.readline(buf) && !buf.startsWith(">THREE") do {}
  
  //
  // Accumulate each remaining line into 'data'.
  // 'readline' includes the newline character in 'buf', so only add a slice
  // of 'buf' that leaves off the last character.
  //
  while stdin.readline(buf) do data += buf[1..buf.length-1];

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
