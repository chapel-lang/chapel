/*
This test checks that:
 `const ref` iterators are allowed to yield any storage class
 `ref` iterators are allowed to yield only ref-able things

The iterators cover all combinations of the following dimensions:
 the return intent: {ref, const ref}
 the return type: {implicit, declared, declared using a type function}
 the return type: {int, concrete record, generic record}
   for "generic record", the declared type is generic,
   whereas the calculated type is of course concrete

One dimension we can add is: {serial,standalone,leader,follower}.

The reason for exercising these dimensions is for compiler code coverage.
Certain aspects of resolution vary depending on some of these parameters.


*/

record RR { }          // a concrete record
record QQ { type t; }  // a generic record

var   v1 = 5, v2 = new RR(), v3 = new QQ(bool);
const c1 = 6, c2 = new RR(), c3 = new QQ(bool);

proc typ1() type return int;
proc typ2() type return RR;
proc typ3() type return QQ;

proc ref1() ref return v1;
proc ref2() ref return v2;
proc ref3() ref return v3;

proc pcr1() const ref return v1;
proc pcr2() const ref return v2;
proc pcr3() const ref return v3;

proc nonr1() return v1;
proc nonr2() return v2;
proc nonr3() return v3;

/////////// return type: not declared ///////////

iter dn_cr1() const ref {
  yield v1;
  yield c1;
  yield ref1();
  yield pcr1();
  yield nonr1(); //error
}
for i in dn_cr1() do writeln(i);

iter dn_r1() ref {
  yield v1;
  yield c1;      //error
  yield ref1();
  yield pcr1();  //error
  yield nonr1(); //error
}
for i in dn_r1() do writeln(i);

iter dn_cr2() const ref {
  yield v2;
  yield c2;
  yield ref2();
  yield pcr2();
  yield nonr2(); //error
}
for i in dn_cr2() do writeln(i);

iter dn_r2() ref {
  yield v2;
  yield c2;      //error
  yield ref2();
  yield pcr2();  //error
  yield nonr2(); //error
}
for i in dn_r2() do writeln(i);

iter dn_cr3() const ref {
  yield v3;
  yield c3;
  yield ref3();
  yield pcr3();
  yield nonr3(); //error
}
for i in dn_cr3() do writeln(i);

iter dn_r3() ref {
  yield v3;
  yield c3;      //error
  yield ref3();
  yield pcr3();  //error
  yield nonr3(); //error
}
for i in dn_r3() do writeln(i);

/////////// return type: 1:int 2:RR (concrete) 3:QQ (generic) ///////////

iter dy_cr1() const ref :int {
  yield v1;
  yield c1;
  yield ref1();
  yield pcr1();
  yield nonr1(); //error
}
for i in dy_cr1() do writeln(i);

iter dy_r1() ref :int {
  yield v1;
  yield c1;      //error
  yield ref1();
  yield pcr1();  //error
  yield nonr1(); //error
}
for i in dy_r1() do writeln(i);

iter dy_cr2() const ref :RR {
  yield v2;
  yield c2;
  yield ref2();
  yield pcr2();
  yield nonr2(); //error
}
for i in dy_cr2() do writeln(i);

iter dy_r2() ref :RR {
  yield v2;
  yield c2;      //error
  yield ref2();
  yield pcr2();  //error
  yield nonr2(); //error
}
for i in dy_r2() do writeln(i);

iter dy_cr3() const ref :QQ {
  yield v3;
  yield c3;
  yield ref3();
  yield pcr3();
  yield nonr3(); //error
}
for i in dy_cr3() do writeln(i);

iter dy_r3() ref :QQ {
  yield v3;
  yield c3;      //error
  yield ref3();
  yield pcr3();  //error
  yield nonr3(); //error
}
for i in dy_r3() do writeln(i);

/////////// return type: computed with a type function ///////////

iter df_cr1() const ref :typ1() {
  yield v1;
  yield c1;
  yield ref1();
  yield pcr1();
  yield nonr1(); //error
}
for i in df_cr1() do writeln(i);

iter df_r1() ref :typ1() {
  yield v1;
  yield c1;      //error
  yield ref1();
  yield pcr1();  //error
  yield nonr1(); //error
}
for i in df_r1() do writeln(i);

iter df_cr2() const ref :typ2() {
  yield v2;
  yield c2;
  yield ref2();
  yield pcr2();
  yield nonr2(); //error
}
for i in df_cr2() do writeln(i);

iter df_r2() ref :typ2() {
  yield v2;
  yield c2;      //error
  yield ref2();
  yield pcr2();  //error
  yield nonr2(); //error
}
for i in df_r2() do writeln(i);

iter df_cr3() const ref :typ3() {
  yield v3;
  yield c3;
  yield ref3();
  yield pcr3();
  yield nonr3(); //error
}
for i in df_cr3() do writeln(i);

iter df_r3() ref :typ3() {
  yield v3;
  yield c3;      //error
  yield ref3();
  yield pcr3();  //error
  yield nonr3(); //error
}
for i in df_r3() do writeln(i);
