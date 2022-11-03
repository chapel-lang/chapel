var notAType = "hello";

var a = int;
type b = notAType;

var c: int;
param d = c;

record MyRecord {}
var e = MyRecord;

type aliasToRecord = MyRecord;
var f = aliasToRecord;
