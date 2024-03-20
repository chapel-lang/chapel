


var globalVar: int;
ref globalRef = globalVar;

proc refFun(arg:int) ref do return globalVar;

record RR { type t1, t2; }




var v1 = 5;
ref r1: real = v1;  // error

var v2 = "hi";
ref r2: int = v2;   // error

ref r3: real;
r3 = v1;            // error (this is a split init)
r3 = v1;            // OK (this is an assignment)

const c1 = true;
const ref q1: int = c1;   // error

const ref q2: real;
q2 = v1;                  // error

var v4: RR(int, string);
ref r4: RR(?) = v4;      // OK
compilerWarning("r4 has type ", r4.type:string, 0);  // RR(int, string)

ref r5: RR(int,?);
r5 = v4;                 // OK
compilerWarning("r5 has type ", r5.type:string, 0);  // RR(int, string);

ref r6: RR(real,?) = v4; // error

const ref q3: RR(bool,string);
q3 = v4;                 // error

const ref q4: domain(?) = LocaleSpace;  // OK
compilerWarning("q4 has type ", q4.type:string, 0);  // domain(1)

const ref q5: domain(1,int(8));
q5 = LocaleSpace;        // error

var v6: [LocaleSpace.dim(0)] locale;
const ref q6: Locales;   // error: not a type
q6 = v6;                 // warning

const ref q7: [LocaleSpace] int = Locales;  // error

ref r71: real = globalRef;                  // error
ref r72: real;
r72 = globalRef;                            // error

const ref q81: real = refFun(5);            // error
const ref q82: real;
q82 = refFun(6);                            // error 
