// When a record variable declaration has an explicit type and an initializer,
// and the record has 'const' field(s) - like r2 below, it should be legal.

record RECTYPE {
  var VARFIELD: int;
  const CONSTFIELD: int;
}

var r1 = new RECTYPE(1,2);           // legal
writeln(r1);
var r2: RECTYPE = new RECTYPE(3,4);  // also legal
writeln(r2);
