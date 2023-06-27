// various examples from #20462
// updated to the rule sign(result.stride) = sign(r1.stride) * sign(r2.stride)

// consistently verbose vs. the range-to-string cast
proc show(r):string {
  var ret: string;
  if r.hasLowBound() then ret += r.lowBound:string;
  ret += "..";
  if r.hasHighBound() then ret += r.highBound:string;
  if !r.hasUnitStride() {
    ret += " by " + r.stride:string;
    ret += " align " + if r.aligned then r.alignment:string else "?";
  }
  return ret;
}

proc test(r1: range(?), r2: range(?), expected:string) {
  proc checkType(r) { compilerError("slice type = ", r.type:string); }
  proc checkType(r: range(?)) { }
  const slice = r1[r2];
  checkType(slice);
  if show(slice) == expected then
    writeln("ok  ", r1, " , ", r2, " , ", expected);
  else
    writeln(r1, " , ", r2, " , ", expected, "  unexpected: got ", show(slice));
}

writeln("start");

// the second range is unaligned

test( 1..20 , ..10 by 2 , "1..10 by 2 align 1" );
test( 1..20 , 2.. by -2 , "2..20 by -2 align 0" );
test( 1..20 , .. by 2 , "1..20 by 2 align 1" );
test( 1..20 , .. by -2 , "1..20 by -2 align 0" );
test( 1.. by -2 align 1 , ..9 by 3 , "1..9 by -6 align 3" );
test( 1..9 by -2 , ..9 by 3 , "1..9 by -6 align 3" );
test( 1..8 by 2 , 4.. by -2 , "4..8 by -2 align 1" );
test( 1..8 by -2 , 5.. by -2 , "5..8 by 2 align 0" );
test( 1..8 by -2 , ..5 by  2 , "1..5 by -2 align 0" );
test( 1..7 , 4.. by -2 , "4..7 by -2 align 1" );
test( 1..7 , ..4 by 2 , "1..4 by 2 align 1" );
test( 1..7 by -1 , ..4 by 2 , "1..4 by -2 align 0" );
test(   1..10 by -3 , ..9 by 2 ,   "1..9 by -6 align 1" ); //unstable: align 4 ?
test( -10..10 by -3 , ..8 by 2 , "-10..8 by -6 align 1" ); //unstable: align 4 ?
test( -10..10 by -3 , ..7 by 2 , "-10..7 by -6 align 1" );
test( -10..10 by -3 , ..4 by 2 , "-10..4 by -6 align 4" );
test( -10..10 by  3 , .. by  2 , "-10..10 by 6 align 2" );
test( -10..10 by -3 , .. by  2 , "-10..10 by -6 align 4" );
test( -10..10 by  3 , .. by -2 , "-10..10 by -6 align 2" );//unstable: align 5 ?
test( -10..10 by -3 , .. by -2 , "-10..10 by 6 align 1" ); //unstable: align 4 ?

// both ranges are aligned

test( 1.. by -2 align 1 , ..6 , "1..6 by -2 align 1" );
test( .. by 2 align 0 , .. by 3 align 1 , ".. by 6 align 4" );
test( 1..9 by 2 , 1..7 by -2 , "1..7 by -2 align 1" );
test( 1..10 by 2 , 1..10 by -2 , "1..0 by -2 align 1" );  // todo: unstable?
test( 1..10 by 2 , 1..10 by -3 , "1..10 by -6 align 1" );
test( 1..9 by 2 , 1..9 by -4 , "1..9 by -4 align 1" );

// error: the first range is unaligned
// the string shows potential results
/*
test( 1.. by -2 , ..6 , "1..6 by -2" );
test( 1.. by -2 , ..20 by 3 , "1..20 by -6" );
test( ..9 by 3 , 1.. by -2 align 1 , "1..9 by 6 align 1" );
test( 1.. by -2 , 1.. by -3 align 1 , "1.. by -6 align 1" );
test( .. by 4 , .. by 9 , ".. by 36" );
test( .. by 4 , .. by 8 , ".. by 8" );
*/

writeln("done");
