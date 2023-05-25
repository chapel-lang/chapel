use BigInteger;

const check_to = 10;

record op_vs_op {
    proc this(const a: int, const b: int): (bool, int, bigint) {
        const a_bi = new bigint(a);
        const b_bi = new bigint(b);

        const amb = a % b;
        const amb_bi = a_bi % b_bi;

        return (
            amb == amb_bi,
            amb,
            amb_bi,
        );
    }
}

record op_vs_op_int {
    proc this(const a: int, const b: int): (bool, int, bigint) {
        const a_bi = new bigint(a);

        const amb = a % b;
        const amb_bi = a_bi % b;

        return (
            amb == amb_bi,
            amb,
            amb_bi,
        );
    }
}

record op_vs_op_uint {
    proc this(const a: int, const b: int): (bool, int, bigint) {
        if b < 0 {
            return (true, 0, new bigint());
        } else {
            const a_bi = new bigint(a);

            const amb = a % b;
            const amb_bi = a_bi % (b:uint);

            return (
                amb == amb_bi,
                amb,
                amb_bi,
            );
        }
    }
}

record op_assign {
    proc this(const a: int, const b: int): (bool, int, bigint) {
        var a_ = a;
        var a_bi = new bigint(a);
        const b_bi = new bigint(b);

        a_ %= b;
        a_bi %= b_bi;

        return (
            a_ == a_bi,
            a_,
            a_bi,
        );
    }
}

record met_vs_met {
    proc this(const a: int, const b: int): (bool, int, bigint) {
        const a_bi = new bigint(a);
        const b_bi = new bigint(b);

        const amb = mod(a, b);
        var amb_bi = new bigint();
        mod(amb_bi, a_bi, b_bi);

        return (
            amb == amb_bi,
            amb,
            amb_bi,
        );
    }
}

record met_vs_met_int {
    proc this(const a: int, const b: int): (bool, int, bigint) {
        var a_bi = new bigint(a);

        const amb = mod(a, b);
        const amb_bi = mod(a_bi, a_bi, b);

        return (
            amb == amb_bi && amb == a_bi,
            amb,
            a_bi,
        );
    }
}

record met_vs_met_uint {
    proc this(const a: int, const b: int): (bool, int, bigint) {
        if b < 0 {
            return (true, 0, new bigint());
        } else {
            var a_bi = new bigint(a);

            const amb = mod(a, b);
            const amb_bi = mod(a_bi, a_bi, b:uint);

            return (
                amb == amb_bi && amb == a_bi,
                amb,
                a_bi,
            );
        }
    }
}

proc check_matching(name: string, checker) {
    for a_positive in [true, false] {
        for b_positive in [true, false] {
            writeln("Checking ", name, " (a : ", ( if a_positive then "+" else "-" ), ", b : ", ( if b_positive then "+" else "-" ), ")");
            var all_match = true;

            for a_value in 0..check_to {
                for b_value in 1..check_to {
                    const a_i = a_value:int * ( if a_positive then 1 else -1 );
                    const b_i = b_value:int * ( if b_positive then 1 else -1 );

                    var (matched, int_sol, bigint_sol) = checker(a_i, b_i);
                    if !matched {
                        writeln("\tmismatch: (", a_i, ", ", b_i, ") \t\t", int_sol, " != ", bigint_sol);
                        all_match = false;
                    }
                }
            }

            writeln("\tAll Matched: ", all_match);
        }
    }
    writeln();
}

check_matching("Operator", new op_vs_op());
check_matching("Operator (b.type = int)", new op_vs_op_int());
check_matching("Operator (b.type = uint)", new op_vs_op_uint());
check_matching("Assigning Operator (%=)", new op_assign());

check_matching("Method", new met_vs_met());
check_matching("Method (b.type = int)", new met_vs_met_int());
check_matching("Method (b.type = uint)", new met_vs_met_uint());
