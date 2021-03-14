// This is motivated by "record body {...}" in shootout/nbody.chpl.
//
// If we have an array of records and the record has const field(s),
// our UMM should provide a way to initialize that array,

record RECTYPE {
  var VARFIELD: int;
  const CONSTFIELD: int;
}

const D = {1..2};

var   A:      [D] RECTYPE = [ new RECTYPE(1,1), new RECTYPE(2,2) ];
const Aconst: [D] RECTYPE = [ new RECTYPE(3,3), new RECTYPE(4,4) ];
writeln(A);
writeln(Aconst);

var   B      = [ new RECTYPE(5,5), new RECTYPE(6,6) ];
const Bconst = [ new RECTYPE(7,7), new RECTYPE(8,8) ];
writeln(B);
writeln(Bconst);

var   C      = [i in D] new RECTYPE(100+i, 200+i);
const Cconst = [i in D] new RECTYPE(300+i, 400+i);
writeln(C);
writeln(Cconst);

// TODO: would also like to be able to factor out array initialization
// into a function. Cf. the following is currently illegal.
proc illegal() {
  var ARRvar: [D] RECTYPE;
  initArr(ARRvar);    //currently this call is legal - this is desired
  writeln(ARRvar);

  const ARRconst: [D] RECTYPE;
  initArr(ARRconst); //currently illegal: cannot pass a 'const' by 'ref' intent
  writeln(ARRconst);

  proc initArr(ref ARR) {
    // Currently these assignments are illegal because they modify
    // records with 'const' fields.
    ARR[1] = new RECTYPE(11,12);
    ARR[2] = new RECTYPE(13,14);
  }

  // In particular, allow array initialization by reading from a channel.
  // Motivated by studies/parsec/blackscholes.chpl.
  use IO;
  const ch: channel; // initialization is not shown - todo
  var Arr3: [D] RECTYPE;
  ch.read(Arr3); //wholesale
  writeln(Arr3);
  var Arr4: [D] RECTYPE;
  for a4 in Arr4 do ch.read(a4); // element by element
  writeln(Arr4);
  ch.close();

  // ditto for const arrays
}
