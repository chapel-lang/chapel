// deprecated by Vass in 1.31

var brt: BoundedRangeType;              // d: BoundedRangeType

brt = BoundedRangeType.bounded;         // d: BoundedRangeType, bounded
writeln(brt);
brt = BoundedRangeType.boundedLow;      // d: BoundedRangeType, boundedLow
writeln(brt);
brt = BoundedRangeType.boundedHigh;     // d: BoundedRangeType, boundedHigh
writeln(brt);
brt = BoundedRangeType.boundedNone;     // d: BoundedRangeType, boundedNone
writeln(brt);

// NB deprecating `range(boundedType=...)` required changing AggregateType.cpp
var r: range(boundedType =              // d: boundedType
  BoundedRangeType.boundedHigh);        // d: BoundedRangeType, boundedHigh

compilerWarning(r.boundedType:string);  // d: boundedType

// NB deprecating `range(boundedType=?B)` required changing preFold.cpp
proc query(arg: range(?,boundedType=?B,strides=?S)) {     // d: boundedType
  compilerWarning(B:string, S:string, 0);
  writeln(arg);
}
query(r);

{
// NB deprecating `use BoundedRangeType` required
// changing UseStmt.cpp and scope-queries.cpp
  use BoundedRangeType;                 // d: BoundedRangeType
  writeln(bounded);                     // d: bounded
  writeln(boundedLow);                  // d: boundedLow
  writeln(boundedHigh);                 // d: boundedHigh
  writeln(boundedNone);                 // d: boundedNone
}
