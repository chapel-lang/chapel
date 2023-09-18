// {range,domain}.stridable is deprecated by Vass in 1.31

var r0: range(stridable=false) = 0..1;
var r1: range(stridable=true)  = 0..1 by 3;
var d0: domain(1,stridable=false) = r0;
var d1: domain(1,stridable=true)  = r1;

param bb = boundKind.both;
var rr0: range(int, bb, false) = r0;
var rr1: range(int, bb, true)  = r1;
var dd0: domain(1, int, false) = d0;
var dd1: domain(1, int, true)  = d1;

var a0: [d0] int;
var a1: [d1] int;

proc pr(arg: range(stridable=?s)) {
  var str = if s then "stridable " else "unit stride ";
  writeln("% pd: ", str, arg.type:string);
}
pr(r0);
pr(r1);

writeln("% ", (r1.stridable,
         d1.stridable,
         rr1.stridable,
         dd1.stridable,
         a1.stridable,
         r1.type.stridable));

/* domain(1, strides=?s) is not implemented currently:
proc pd(arg: domain(1, stridable=?s)) {
  var str = if s then "stridable " else "unit stride ";
  writeln("% pd: ", str, arg.type:string);
}
pd(d0); pd(d1);
*/
