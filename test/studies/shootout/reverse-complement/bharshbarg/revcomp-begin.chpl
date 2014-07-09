
extern proc memcpy(a:[], b, len);

var barrier : atomic int;

proc string.toBytes() var {
   var b : [1..this.length] uint(8);
   memcpy(b, this, this.length);
   return b;
}

const pairs = "ATCGGCTAUAMKRYWWSSYRKMVBHDDHBVNN\n\n".toBytes();
var table : [1..128] uint(8);

proc main() {
  var info : string;
  var temp : string;

  // Major leaks
  while stdin.readline(temp) do info += temp;

  // temporary while we wait for string features that 
  // would allow us to modify the string in place.
  var data = info.toBytes();

  forall i in 1..pairs.size by 2 {
    table[pairs[i]] = pairs[i+1];      // uppercase
    if pairs[i] != ascii("\n") then
      table[pairs[i] + 32] = pairs[i+1]; // lowercase
  }

  var end = data.size - 1;

  while true {
    var from = end;
    while data[from] != ascii(">") do from -= 1;

    begin process(data, from, end);

    end = from - 2;
    if end <= 0 then break;
  }

  barrier.waitFor(3);

  var f = openfd(1);
  var binout = f.writer(iokind.native, locking=false);
  binout.write(data);
}

proc process(ref data : [],in from : int, end : int) {
  while data[from] != 0xa do from += 1;
  from += 1;

  const len = end - from;
  const off = 60 - (len % 61);

  if off > 0 {
    var m = from + 60 - off;
    while m < end {
      for i in 0..off-1 by -1 do data[m+1+i] = data[m+i];
      data[m+1] = 0xa;
      m += 61;
    }
  }

  const middle = (end-from)/2;
  for i in 0 .. middle do {
    const c = table[data[from+i]];
    data[from+i] = table[data[end-i]];
    data[end-i] = c;
  }

  barrier.add(1);
}
