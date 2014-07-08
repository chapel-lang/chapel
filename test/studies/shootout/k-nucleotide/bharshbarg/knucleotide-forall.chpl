use IO;
use AdvancedIters;

extern proc memcpy(a:[], b, len);

proc channel.bulkget(len:int(64) = max(int(32)), ref str_out:string):bool {
  var ret:string;
  var err:syserr = ENOERR;
  var lenRead:int(64);
  var temp : c_string;

  err = qio_channel_read_string(false, this._style().byteorder, len,
      this._channel_internal, temp, lenRead, -1);
  str_out = toString(temp);
  __primitive("string_normalize", str_out, 1+lenRead);
  return err != EEOF;
}

var datalen = 1;

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

proc decode(data : uint, size : int) {
  var ret : string;
  var d = data;
  for i in 1..size {
    ret = tochar[(d & 3) : uint(8)] + ret;
    d >>= 2;
  }
  return ret;
}

proc hash(ref str : [] uint(8), beg, end : int ) {
  var size = (end - beg) : uint(8);
  var data = 0;
  for i in beg..end-1 {
    data <<= 2;
    data |= tonum(str[i]);
  }
  return data:uint;
}

proc calculate(ref data : [] uint(8), size : int) {
  // create a new dictionary informed of the expected size requirements
  var freqDom : domain(uint, false);
  var freqs : [freqDom] int;

  const mask = ~(3 << ((size - 1) * 2)); // will remove the leftmost encoded character
  var d = hash(data, 1, 1+size);
  for i in 1..datalen - size {
    if i > 1 {
      d = mask & d;
      d <<= 2;
      d |= tonum[data[i+size-1]];
    }
    if !freqDom.member(d) then freqDom += d;
    freqs[d] += 1;
  }

  return freqs;
}

proc write_frequencies(ref data : [] uint(8), size : int) {
  var sum = datalen  - size;
  var freqs = calculate(data, size);

  // sort by frequencies
  var arr : [1..freqs.size] (int, uint);
  for (a, k, v) in zip(arr, freqs.domain, freqs) do
    a = (-v,k); // '-' for descending order
  QuickSort(arr);

  for (c, e) in arr {
    writef("%s %.3dr\n",decode(e, size), (100.0 * -c) / sum);
  }
}

proc string.toBytes() var {
   var b : [1..this.length] uint(8);
   memcpy(b, this, this.length);
   return b;
}

proc write_count(ref data : [] uint(8), str : string) {
  var freqs = calculate(data, str.length);
  var d = hash(str.toBytes(), 1, str.length+1);
  return freqs[d] + "\t" + decode(d, str.length);
}

proc main() {
  var st = stdin._style();
  st.string_format = QIO_STRING_FORMAT_TOEND;
  st.string_end = 0x3E; // '>'

  var chunk : string;
  var res:syserr;
  stdin.read(chunk, st, res); // >
  stdin.read(chunk, st, res); // ONE ... >
  stdin.read(chunk, st, res); // TWO ... >
  stdin.readline(chunk);      // THREE ... \n

  // read in everything
  var info : string;
  stdin.bulkget(max(int(32)), info);

  var data = info.toBytes();

  // make everything uppercase and overwrite newlines
  for i in 1..data.size {
    if data[i] != 0xA {
      data[datalen] = data[i] ^ 0x20;
      datalen += 1;
    }
  }

  write_frequencies(data, 1);
  writeln();
  write_frequencies(data, 2);
  writeln();

  var interests = ["GGT", "GGTA", "GGTATT", "GGTATTTTAATT", "GGTATTTTAATTTATAGT"];
  var results : [1..interests.size] string;

  forall (idx, s) in zip(dynamic(1..interests.size, 1), interests) do results[idx] = write_count(data, s);

  for r in results do writeln(r);
}
