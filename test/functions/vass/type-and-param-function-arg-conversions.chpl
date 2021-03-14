// Should calls to 'param' and 'type' functions read its actual arguments,
// esp. when the actuals are sync variables or such, and not known at compile
// time?

// As written, the test requires that 'param' functions do not read such
// actuals, whereas 'type' functions do. The rationale is that a 'type'
// function may need its run-time actuals e.g. to compute the domain
// of the array type that it returns.
// While some type functions may not need such actuals, currently
// the requirement is uniform for all type functions.

proc paramFun(i: int)     param  return 11111;
proc typeFunNoUse(i: int) type  return int;
proc typeFunUse(i:int)    type  return [1..i] int;

var sv1, sv2, sv3: sync int;

writeln(paramFun(sv1.readXX()));

sv2.writeEF(2);
type t2 = typeFunNoUse(sv2.readFE());
writeln(t2:string);

sv3 = 3;
type t3 = typeFunUse(sv3.readFE());
writeln(t3:string);
