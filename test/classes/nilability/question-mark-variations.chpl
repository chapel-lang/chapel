class C { type t; var x:t; }

var y: unmanaged C(int)?;
var z: unmanaged C?(int);

assert(y.type == unmanaged C(int)?);
assert(z.type == unmanaged C(int)?);

var a0 = new C(int)?;             // owned C?
writeln("a0 ", a0.type:string, " ", a0);
var a1 = new owned C(int)?;       // owned C?
writeln("a1 ", a1.type:string, " ", a1);
var a2 = new shared C(int)?;      // shared C?
writeln("a2 ", a2.type:string, " ", a2);
var a3 = new borrowed C(int)?;    // borrowed C?
writeln("a3 ", a3.type:string, " ", a3);
var a4 = new unmanaged C(int)?;   // unmanaged C?
writeln("a4 ", a4.type:string, " ", a4);

var b0 = new C?(int);             // owned C?
writeln("b0 ", b0.type:string, " ", b0);
var b1 = new owned C?(int);       // owned C?
writeln("b1 ", b1.type:string, " ", b1);
var b2 = new shared C?(int);      // shared C?
writeln("b2 ", b2.type:string, " ", b2);
var b3 = new borrowed C?(int);    // borrowed C?
writeln("b3 ", b3.type:string, " ", b3);
var b4 = new unmanaged C?(int);   // unmanaged C?
writeln("b4 ", b4.type:string, " ", b4);

assert( (owned C)? == owned C? );
assert( (shared C)? == shared C?);
assert( (borrowed C)? == borrowed C?);
assert( (unmanaged C)? == unmanaged C?);

assert( (owned C(int))? == owned C(int)? );
assert( (shared C(int))? == shared C(int)?);
assert( (borrowed C(int))? == borrowed C(int)?);
assert( (unmanaged C(int))? == unmanaged C(int)?);

type A = C?; // nilable C, not knowing further instantiation information
type B = A(int); // could consider this as literally ``C?(int)`` via aliasing
var c0 = new A(int);
writeln("c0 ", c0.type:string, " ", c0);

proc factory1(type t) {
  return new t(int, 1);
}
proc factory2(type t) {
  return new t(1);
}
var d0 = factory1(C?);
writeln("d0 ", d0.type:string, " ", d0);
var d1 = factory2(C(int)?);
writeln("d1 ", d1.type:string, " ", d1);
var d2 = factory2(C?(int));
writeln("d2 ", d2.type:string, " ", d2);

delete a4, b4;
