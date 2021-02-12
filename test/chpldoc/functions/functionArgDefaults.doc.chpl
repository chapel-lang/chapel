var x, y, z: int;

class C { var aField: int; }
var c: unmanaged C?;

proc noArgs() {}

proc literalDefault(arg=42) {}

proc varDefault(arg=x) {}

proc notDefault(arg=!x) {}
proc negDefault(arg=-x) {}
proc bitwiseNegDefault(arg=~x) {}

proc notNilDefault(arg=c!) {}
proc isNilDefault(arg=nil) {}

proc plusDefault(arg=x+y) {}
proc minusDefault(arg=x-y) {}
proc multDefault(arg=x*y) {}
proc divDefault(arg=x/y) {}
proc modDefault(arg=x%y) {}

proc orDefault(arg=x||y) {}
proc binOrDefault(arg=x|y) {}
proc andDefault(arg=x&&y) {}
proc binAndDefault(arg=x&y) {}
proc binXorDefault(arg=x^y) {}

proc threeOrDefault(arg=x||y||z) {}
proc threeBinOrDefault(arg=x|y|z) {}
proc threeAndDefault(arg=x&&y&&z) {}
proc threeBinAndDefault(arg=x&y&z) {}

proc powDefault(arg=x**y) {}
proc lshiftDefault(arg=x<<y) {}
proc rshiftDefault(arg=x>>y) {}

proc eqDefault(arg=x==y) {}
proc neqDefault(arg=x!=y) {}
proc lessDefault(arg=x<y) {}
proc gtDefault(arg=x>y) {}
proc lessEqDefault(arg=x<=y) {}
proc gtEqDefault(arg=x>=y) {}

proc minDefault(arg=min(int)) {}
proc maxDefault(arg=max(int)) {}

proc castDefault(arg=x:int(32)) {}

proc tuple1Default(arg=(x,)) {}
proc tuple2Default(arg=(x,y)) {}
proc tuple3Default(arg=(x,y,z)) {}

proc dotTypeDefault(arg=x.type) {}

proc dotFieldDefault(arg=c!.aField) {}

proc ternaryDefault(arg = if x then y else z) {}

proc f0() {}
proc f1(x) {}
proc f2(x, y) {}
proc f3(x, y, z) {}

proc noArgFuncDefault(arg=f0()) {}
proc oneArgFuncDefault(arg=f1(x)) {}
proc twoArgFuncDefault(arg=f2(x,y)) {}
proc threeArgFuncDefault(arg=f3(x,y,z)) {}

proc twoArgsWithDefaults(arg1=x+y, arg2=x-y) {}
proc threeArgsWithDefaults(arg1=x*y, arg2=x/y, arg3=x**y) {}

record R0 {}
record R1 { param a: int; }
record R2 { param a,b: int; }
record R3 { param a,b,c: int; }

proc R0TypedArgNoDefault(arg:R0) {}
proc R1TypedArgNoDefault(arg:R1(1)) {}
proc R2TypedArgNoDefault(arg:R2(1,2)) {}
proc R3TypedArgNoDefault(arg:R3(1,2,3)) {}

proc R0TypedArgDefault(arg:R0 = new R0()) {}
proc R1TypedArgDefault(arg:R1(1) = new R1(1)) {}
proc R2TypedArgDefault(arg:R2(1,2) = new R2(1,2)) {}
proc R3TypedArgDefault(arg:R3(1,2,3) = new R3(1,2,3)) {}
