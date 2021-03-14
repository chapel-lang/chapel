config const n: int = 8;
config const verbose: bool = true;

if n < numLocales || n % numLocales != 0 then
  halt("the number of locales, ", numLocales, ", does not evenly divide n,", n);

class DistributedArray {
  var ndata: range(int);
  var data: [ndata] int;
  var others: [0..numLocales-1] unmanaged DistributedArray?;
  var otherBases: [0..numLocales-1] _ddata(int);
}

proc DistributedArray.this(i: int) ref {
  if ndata.contains(i) {
    return data[i];
  } else {
    const loc = (i-1)*numLocales/n;
    return otherBases[loc](i-n*loc/numLocales-1);
  }
}

proc DistributedArray.writeThis(W) throws {
  for loc in Locales {
    W.write(if loc == here then data else others[loc.id]!.data);
    if loc.id != numLocales-1 then
      W.write(" ");
  }
}

pragma "locale private" var A: unmanaged DistributedArray?;

//
// set up DistributedArray
//
{
  var AS: [0..numLocales-1] unmanaged DistributedArray?;
  for loc in Locales do on loc {
    A = new unmanaged DistributedArray(n*here.id/numLocales+1..n*(here.id+1)/numLocales);
    AS[here.id] = A;
    if verbose then
      writeln(here.id, ": data[", A!.ndata, "] = ", A!.data);
  }

  for loc in Locales do on loc {
    A!.others = AS;
    for i in 0..numLocales-1 {
      A!.otherBases[i] = AS[i]!.data._value.data;
    }
  }
}

if verbose then
  writeln(A);

for i in 1..n do
  A!(i) = i;

if verbose then
  writeln(A);

for loc in Locales do on loc {
  delete A;
}
