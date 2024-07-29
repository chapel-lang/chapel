proc A.foo() {}
proc ref A.foo() {}
proc const A.foo() {}
proc const ref A.foo() {}
proc in A.foo() {}
proc const in A.foo() {}
proc inout A.foo() {}
proc out A.foo() {}
proc param A.foo() {}
proc type A.foo() {}
proc var A.foo() {}
iter A.bar() {}
iter ref A.bar() {}
iter const A.bar() {}
iter const ref A.bar() {}
iter in A.bar() {}
iter const in A.bar() {}
iter inout A.bar() {}
iter out A.bar() {}
iter param A.bar() {}
iter type A.bar() {}
iter var A.bar() {}
operator A.+() {}
operator ref A.+() {}
operator const A.+() {}
operator const ref A.+() {}
operator in A.+() {}
operator const in A.+() {}
operator inout A.+() {}
operator out A.+() {}
operator param A.+() {}
operator type A.+() {}
operator var A.+() {}

record R1 {
  proc foo() {}
  proc ref foo() {}
  proc const foo() {}
  proc const ref foo() {}
  proc in foo() {}
  proc const in foo() {}
  proc inout foo() {}
  proc out foo() {}
  proc param foo() {}
  proc type foo() {}
  proc var foo() {}
  iter bar() {}
  iter ref bar() {}
  iter const bar() {}
  iter const ref bar() {}
  iter in bar() {}
  iter const in bar() {}
  iter inout bar() {}
  iter out bar() {}
  iter param bar() {}
  iter type bar() {}
  iter var bar() {}
  operator +() {}
  operator ref +() {}
  operator const +() {}
  operator const ref +() {}
  operator in +() {}
  operator const in +() {}
  operator inout +() {}
  operator out +() {}
  operator param +() {}
  operator type +() {}
  operator var +() {}
}


proc foo() {}
proc foo() ref {}
proc foo() const {}
proc foo() const ref {}
proc foo() in {}
proc foo() const in {}
proc foo() inout {}
proc foo() out {}
proc foo() param {}
proc foo() type {}
proc foo() var {}
iter bar() {}
iter bar() ref {}
iter bar() const {}
iter bar() const ref {}
iter bar() in {}
iter bar() const in {}
iter bar() inout {}
iter bar() out {}
iter bar() param {}
iter bar() type {}
iter bar() var {}
operator +() {}
operator +() ref {}
operator +() const {}
operator +() const ref {}
operator +() in {}
operator +() const in {}
operator +() inout {}
operator +() out {}
operator +() param {}
operator +() type {}
operator +() var {}

proc A.foo() {}
proc A.foo() ref {}
proc A.foo() const {}
proc A.foo() const ref {}
proc A.foo() in {}
proc A.foo() const in {}
proc A.foo() inout {}
proc A.foo() out {}
proc A.foo() param {}
proc A.foo() type {}
proc A.foo() var {}
iter A.bar() {}
iter A.bar() ref {}
iter A.bar() const {}
iter A.bar() const ref {}
iter A.bar() in {}
iter A.bar() const in {}
iter A.bar() inout {}
iter A.bar() out {}
iter A.bar() param {}
iter A.bar() type {}
iter A.bar() var {}
operator A.+() {}
operator A.+() ref {}
operator A.+() const {}
operator A.+() const ref {}
operator A.+() in {}
operator A.+() const in {}
operator A.+() inout {}
operator A.+() out {}
operator A.+() param {}
operator A.+() type {}
operator A.+() var {}

record R2 {
  proc foo() {}
  proc foo() ref {}
  proc foo() const {}
  proc foo() const ref {}
  proc foo() in {}
  proc foo() const in {}
  proc foo() inout {}
  proc foo() out {}
  proc foo() param {}
  proc foo() type {}
  proc foo() var {}
  iter bar() {}
  iter bar() ref {}
  iter bar() const {}
  iter bar() const ref {}
  iter bar() in {}
  iter bar() const in {}
  iter bar() inout {}
  iter bar() out {}
  iter bar() param {}
  iter bar() type {}
  iter bar() var {}
  operator +() {}
  operator +() ref {}
  operator +() const {}
  operator +() const ref {}
  operator +() in {}
  operator +() const in {}
  operator +() inout {}
  operator +() out {}
  operator +() param {}
  operator +() type {}
  operator +() var {}
}


type T1  = proc(): void;
type T2  = proc() ref: void;
type T3  = proc() const: void;
type T4  = proc() const ref: void;
type T5  = proc() in: void;
type T6  = proc() const in: void;
type T7  = proc() inout: void;
type T8  = proc() out: void;
type T9  = proc() param: void;
type T10 = proc() type: void;
type T11 = proc() var: void;
const p1  = proc() {};
const p2  = proc() ref {};
const p3  = proc() const {};
const p4  = proc() const ref {};
const p5  = proc() in {};
const p6  = proc() const in {};
const p7  = proc() inout {};
const p8  = proc() out {};
const p9  = proc() param {};
const p10 = proc() type {};
const p11 = proc() var {};


proc foo(A1,
         ref A2,
         const A3,
         const ref A4,
         in A5,
         const in A6,
         inout A7,
         out A8,
         param A9,
         type A10,
         var A11) {}
proc bar(A1...,
         ref A2...,
         const A3...,
         const ref A4...,
         in A5...,
         const in A6...,
         inout A7...,
         out A8...,
         param A9...,
         type A10...,
         var A11...) {}
type t = proc(_:int,
               ref _:int,
               const _:int,
               const ref _:int,
               in _:int,
               const in _:int,
               inout _:int,
               out _:int,
               param _:int,
               type _:int,
               var _:int);
