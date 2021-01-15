module variableDefaultValues {

  var x, y, z: int;

  class C { var aField: int; }
  var c: unmanaged C?;

  var literalDefault = 42;

  var varDefault = x;

  var notDefault = !x;
  var negDefault = -x;
  var bitwiseNegDefault = ~x;

  var notNilDefault = c!;

  //var isNilDefault: C? = nil;

  var plusDefault = x+y;
  var minusDefault = x-y;
  var multDefault = x*y;
  var divDefault = x/y;
  var modDefault = x%y;

  var orDefault = x||y;
  var binOrDefault = x|y;
  var andDefault = x&&y;
  var binAndDefault = x&y;
  var binXorDefault = x^y;

  var threeOrDefault = x||y||z;
  var threeBinOrDefault = x|y|z;
  var threeAndDefault = x&&y&&z;
  var threeBinAndDefault = x&y&z;

  var powDefault = x**y;
  var lshiftDefault = x<<y;
  var rshiftDefault = x>>y;

  var eqDefault = x==y;
  var neqDefault = x!=y;
  var lessDefault = x<y;
  var gtDefault = x>y;
  var lessEqDefault = x<=y;
  var gtEqDefault = x>=y;

  var minDefault = min(int);
  var maxDefault = max(int);

  var castDefault = x:int(32);

  //var tuple1Default = (x,);
  var tuple2Default = (x,y);
  var tuple3Default = (x,y,z);

  var dotTypeDefault = x.type;

  var dotFieldDefault = c!.aField;

  var ternaryDefault = if x then y else z;

  proc f0() {}
  proc f1(x) {}
  proc f2(x, y) {}
  proc f3(x, y, z) {}

  var noArgFuncDefault = f0();
  var oneArgFuncDefault = f1(x);
  var twoArgFuncDefault = f2(x,y);
  var threeArgFuncDefault = f3(x,y,z);

  record R0 {}
  record R1 { param a: int; }
  record R2 { param a,b: int; }
  record R3 { param a,b,c: int; }

  var R0TypedArgNoDefault:R0;
  var R1TypedArgNoDefault:R1(1);
  var R2TypedArgNoDefault:R2(1,2);
  var R3TypedArgNoDefault:R3(1,2,3);

  var R0TypedArgDefault:R0 = new R0();
  var R1TypedArgDefault:R1(1) = new R1(1);
  var R2TypedArgDefault:R2(1,2) = new R2(1,2);
  var R3TypedArgDefault:R3(1,2,3) = new R3(1,2,3);
}
