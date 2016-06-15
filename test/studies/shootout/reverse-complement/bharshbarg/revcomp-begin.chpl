param newLineChar = 0x0A;
param greaterThan = 0x3E;
const pairs = "ATCGGCTAUAMKRYWWSSYRKMVBHDDHBVNN\n\n";
var table : [1..128] uint(8);

proc main() {
  // initialize complement table
  for i in 0..#pairs.length by 2 {
    table[pairs.buff[i]] = pairs.buff[i+1];        // uppercase
    if pairs.buff[i] != newLineChar then
      table[pairs.buff[i] + 32] = pairs.buff[i+1]; // lowercase
  }
  
  var data : string; 
  var idx = 1;
  var start = 0;

  // sync statements wait for all tasks inside them to complete
  sync {
    var data_in : string;
    while readln(data_in) {
      data += data_in;

      // Look for the start of a section, and if possible 
      // spawn a task to start work on the previous section.
      if data.buff[idx] == greaterThan {
        if start != 0 then 
          begin process(data, start, idx-2);
        start = idx;
      }

      idx += data_in.len; 
    }

    // work on the last section
    process(data, start, idx-2);
  }
  
  write(data);
}

proc process(data : string, in from : int, end : int) {

  // Skip the header information
  while data.buff[from] != 0xa do
    from += 1;
  from += 1;

  const len = end - from;
  const adj = len % 61; 
  const off = 60 - adj;

  if off > 0 {
    var m = from + adj;
    while m < end {
      for i in 0..off-1 by -1 do
        data.buff[m+1+i] = data.buff[m+i];
      data.buff[m+1] = 0xa;
      m += 61;
    }
  }

  const middle = len / 2;
  for i in 0..middle {
    const c = table[data.buff[from+i]];
    data.buff[from+i] = table[data.buff[end-i]];
    data.buff[end-i] = c;
  }
}
