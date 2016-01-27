class C {
  var field1: if isConst then int else string;
  var field2: R(isConst = isConst);
  (if isConst then const else var) field3: int;
}

record R {
  var field4: int;
}

var c1 = new C();
writeln(c1.field1.type:string);  // string
c1.field2.field4 = 5;
c1.field3 = 6;

const c2 = new C();
writeln(c2.field1.type:string);  // int
c2.field2.field4 = 7;                   // compiler error - assigning to 'const'
c2.field3 = 8;                          // ditto


//
// let's see how this idea applies to records
//

var r1: R(isConst = false);   // 'isConst = false' is unnecessary
r1.field4 = 1;

var r2: R(isConst = true);    // overrides var-ness of r2? only partially?
r2.field4 = 2;                // must be disallowed
r2 = r1;                      // allowed?

const r3: R(isConst = false); // contradiction; suggestion: 'const' takes
                              // precedence, so this should be disallowed
                              // by the compiler

const r4: R(isConst = true);  // 'isConst = true' is unnecessary
r4.field4 = 3;                // compiler error - assigning to 'const'
r4 = r1;                      // ditto
