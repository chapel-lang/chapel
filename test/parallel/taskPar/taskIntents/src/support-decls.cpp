enum EnumType {
  eFirst,
  eSecond,
  eLast
}

record RecordSmall {
  var xxx: int;
}
const rSmall = new RecordSmall(xxx=600033);

record RecordLarge {
  var yy01, yy02, yy03, yy04, yy05, yy06, yy07, yy08, yy09, yy10,
      yy11, yy12, yy13, yy14, yy15, yy16, yy17, yy18, yy19, yy20: int;
}
const rLarge = new RecordLarge(yy01=30001, yy07=30007, yy20=30020);

class ClassType {
  var zzz: int;
}
const cInstance = new ClassType(44444444);

union UnionType {
  var ufield111, ufield222: int;
}
var uInstanceVar: UnionType;
uInstanceVar.ufield222 = 339933;
const uInstance = uInstanceVar;

type DomType1 = domain(1);
const init1dom = {7770..7771};

type DomType2 = domain(2);
const init2dom = {110001..110002, 330004..330005};

type ArrType1 = [init1dom] int;
const init1arr: ArrType1 = 11011;

type ArrType2 = [init2dom] int;
const init2arr: ArrType2 = 33033;
