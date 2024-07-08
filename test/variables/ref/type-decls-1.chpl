
var globalVar: int;
ref globalRef = globalVar;

proc refFun(arg:int) ref do return globalVar;

record RR { type t1, t2; var x1: t1, x2: t2; }

ref r1: int = globalRef;
r1 = 23;
writeln(globalVar);

ref r2: int = refFun(0);
r2 = 32;
writeln(globalVar);

var v3: RR(int, string);
ref r3: RR(int, string) = v3;
r3 = new RR(int, string, 33, "hi3");
writeln(v3, "  ", r3.type:string);

ref r4: RR(?) = v3;
r4 = new RR(int, string, 44, "hi4");
writeln(v3, "  ", r4.type:string);

ref r5: RR(int,?);
r5 = v3;
r5 = new RR(int, string, 55, "hi5");
writeln(v3, "  ", r5.type:string);

var dom: domain(1);
ref r6: domain(?) = dom;
r6.setIndices((1..6,));
writeln(dom, "  ", r6.type:string);

ref r7: domain(1);
r7 = dom;
r7.setIndices((2..7,));
writeln(dom, "  ", r7.type:string);

const ref r8: [LocaleSpace] locale = Locales;
writeln(r8[0], "  ", r8.type:string);
