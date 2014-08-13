use Time;
extern proc memcpy(a:[], b, len);

var barrier : atomic int;

proc string.toBytes() var {
   var b : [1..this.length] uint(8);
   memcpy(b, this, this.length);
   return b;
}

const pairs = "ATCGGCTAUAMKRYWWSSYRKMVBHDDHBVNN\n\n".toBytes();
var table : [1..128] uint(8);
var g : Timer;

proc main() {
  param newLineChar = 0x0A;
  param greaterThan = 0x3E;
  var inFile = openfd(0); // stdin
  const fileLen = inFile.length();
  var data : [1..fileLen] uint(8);
  var r = inFile.reader(kind=ionative, locking=false);

  for i in 1..pairs.size by 2 {
    table[pairs[i]] = pairs[i+1];      // uppercase
    if pairs[i] != newLineChar then
      table[pairs[i] + 32] = pairs[i+1]; // lowercase
  }

  var numRead = 1;
  const lineSize = 61;
  var idx = 1;
  var start = 0;
  var t : Timer;
  t.start();
  while r.readline(data, numRead, idx) {
    if data[idx] == greaterThan {
      if start == 0 then start = idx;
      else {
        stderr.writeln("starting another at ", t.elapsed());
        begin process(data, start, idx-2);
        start = idx;
      }
    }
    idx += numRead; 
  }
  stderr.writeln("read took ", t.elapsed());
  begin process(data,start, idx-2);

  var end = data.size - 1;

  barrier.waitFor(3);

  var f = openfd(1);
  var binout = f.writer(iokind.native, locking=false, hints=QIO_CH_ALWAYS_UNBUFFERED);
  binout.write(data);
}

proc process(ref data : [],in from : int, end : int) {
  var t : Timer;
  t.start();
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
  for i in 0 .. middle {
    const c = table[data[from+i]];
    data[from+i] = table[data[end-i]];
    data[end-i] = c;
  }
  stderr.writeln(from, " .. ", end, " took ", t.elapsed());

  barrier.add(1);
}
