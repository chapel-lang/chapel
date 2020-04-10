proc string.splitStringToTuple(param numChunks: int) {
  var tup: numChunks*string;
  var count = 0;

  // fill in the initial tuple elements defined by split()                  
  for s in this.split(numChunks-1) {
    tup(count) = s;
    count += 1;
  }
  // if split() had fewer items than the tuple, fill in the rest            
  if (count < numChunks) {
    for i in count..numChunks-1 {
      tup(i) = "";
    }
  }
  return tup;
}

var (i, r, s) = "1 2.3 'hi'".splitStringToTuple(3): (int,real,string);
writeln(i, ":", i.type:string);
writeln(r, ":", r.type:string);
writeln(s, ":", s.type:string);
