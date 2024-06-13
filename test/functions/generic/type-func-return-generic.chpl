record R1 { }
record R2 { type t; }
record R3 { type t = int; }

proc bar(type t) type {
  compilerWarning(t:string);
  return t;
}

bar(int);

bar(R1);

// bar(R2); // warning: add (?)
bar(R2(?));
bar(R2(int));
bar(R3);
bar(R3(?));
bar(R3(real));


type x1 = bar(int);

type x2 = bar(R1);

// type x3 = bar(R2); // warning: add (?)
type x4 = bar(R2(?));
type x5 = bar(R2(int));
type x6 = bar(R3);
type x7 = bar(R3(?));
type x8 = bar(R3(real));
