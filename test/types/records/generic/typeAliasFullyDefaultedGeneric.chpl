record R {
  param flag = true;
}

config param coinflip = true;

type t = R;
type t2 = R();

writeln(isGenericType(R));
writeln(isGenericType(R()));
writeln(isGenericType(t));
writeln(isGenericType(t2));

type t3 = if coinflip then R else R;
type t4 = if coinflip then R() else R();

writeln(isGenericType(t3));
writeln(isGenericType(t4));

type t5 = R(?);
type t6 = if coinflip then R(?) else R(?);

writeln(isGenericType(t5));
writeln(isGenericType(t6));
