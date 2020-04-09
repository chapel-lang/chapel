var list = ["one", "two", "three"];

writeln(",".join([l in list] l));
writeln(",".join(forall l in list do l));
writeln(",".join(for l in list do l));

var x = [l in list] l;
writeln(",".join(x));

var v = [i in 1..3] (i : string);
writeln( "=", "".join( v ), "=" );
writeln( "=", "".join( ([i in 1..3] (i : string)) ), "=" );
writeln( "=", "-".join( ([i in 1..3] (i : string)) ), "=" );

