record R {
  param flag = true;
}

config param coinflip = true;

type t = R;
type t2 = R();

writeln(R:string);
writeln(R():string);
writeln(t:string);
writeln(t2:string);

type t3 = if coinflip then R else R;
type t4 = if coinflip then R() else R();

writeln(t3:string);
writeln(t4:string);

type t5 = R(?);
type t6 = if coinflip then R(?) else R(?);

writeln(t5:string);
writeln(t6:string);
