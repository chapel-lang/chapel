// When this test works as expected, remove it. To test this pattern,
// remove the workaround in with-runtime-fields-errors.chpl

class C {
  type tt;
}

type T1 = owned C?;
var c1 = new T1(int);
compilerWarning(c1.type:string);

type T2 = shared C?;
var c2 = new T2(int);
compilerWarning(c2.type:string);

compilerError("done");
