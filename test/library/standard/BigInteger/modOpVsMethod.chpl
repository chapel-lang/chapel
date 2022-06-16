use BigInteger;

for a_positive in [true, false] {
    for b_positive in [true, false] {
        
        writeln("Checking Operator (a : ", ( if a_positive then "+" else "-" ), ", b : ", ( if b_positive then "+" else "-" ), ")");
        var all_match = true;
        
        for a_value in 0..10 {
            for b_value in 1..10 {
                const a_i = a_value:int * ( if a_positive then 1 else -1 );
                const b_i = b_value:int * ( if b_positive then 1 else -1 );
                const a_bi = new bigint(a_i);
                const b_bi = new bigint(b_i);

                const amb_i : int = a_i % b_i;
                const amb_bi : bigint = a_bi % b_bi;

                if amb_i != amb_bi:int {
                    writeln("\tmismatch: (", a_i, " % ", b_i, ") \t\t", amb_i, " != ", amb_bi);
                    all_match = false;
                }
            }   
        }

        writeln("\tAll Matched: ", all_match);
    }
}

writeln();

for a_positive in [true, false] {
    for b_positive in [true, false] {
        
        writeln("Checking Method (a : ", ( if a_positive then "+" else "-" ), ", b : ", ( if b_positive then "+" else "-" ), ")");
        var all_match = true;
        
        for a_value in 0..10 {
            for b_value in 1..10 {
                const a_i = a_value:int * ( if a_positive then 1 else -1 );
                const b_i = b_value:int * ( if b_positive then 1 else -1 );
                const a_bi = new bigint(a_i);
                const b_bi = new bigint(b_i);

                const amb_i : int = mod(a_i, b_i);
                var amb_bi = new bigint();
                amb_bi.mod(a_bi, b_bi);

                if amb_i != amb_bi:int {
                    writeln("\tmismatch: mod(", a_i, ", ", b_i, ") \t\t", amb_i, " != ", amb_bi);
                    all_match = false;
                }
            }   
        }

        writeln("\tAll Matched: ", all_match);
    }
}

writeln();

for a_positive in [true, false] {
    for b_positive in [true, false] {
        
        writeln("Checking Method (with b.type = int) (a : ", ( if a_positive then "+" else "-" ), ", b : ", ( if b_positive then "+" else "-" ), ")");
        var all_match = true;
        
        for a_value in 0..10 {
            for b_value in 1..10 {
                const a_i = a_value:int * ( if a_positive then 1 else -1 );
                const b_i = b_value:int * ( if b_positive then 1 else -1 );
                const a_bi = new bigint(a_i);

                const amb_i : int = mod(a_i, b_i);
                var amb_bi = new bigint();
                amb_bi.mod(a_bi, b_i); // haven't messed with this method yet!!!

                if amb_i != amb_bi:int {
                    writeln("\tmismatch: mod(", a_i, ", ", b_i, ") \t\t", amb_i, " != ", amb_bi);
                    all_match = false;
                }
            }   
        }

        writeln("\tAll Matched: ", all_match);
    }
}