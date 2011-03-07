//
// This test code doesn't make any sense but it tickles a code path
//  in deadCodeElimination where you try to fold a conditional
//  within an expression that has already been folded out
//
var a:int, b:int;
var al:int(64), bl:int(64);
var ua:uint, ub:uint;
var ual:uint(64), ubl:uint(64);

config param t = 0;

proc dit(a, b) {
  if a < 0 {
    if b >= 0 then return true;
    else return false;
  } else {
    if b <= 0 then return false;
    return true;
  }
}

proc do_dit(a, b) {
  if dit(a, b) then writeln("dit(", a, ",", b, ")");
  else if dit(b, a) then writeln("dit(", b, ",", a, ")");
  else writeln("whatever");
}

if t==0 then do_dit(a, b);
 else if t==1 then do_dit(al, bl);
 else if t==2 then do_dit(ua, ub);
 else if t==3 then do_dit(ual, ubl);

