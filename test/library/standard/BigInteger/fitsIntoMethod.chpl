use BigInteger;
use CTypes;

// var chpl_types_to_check: [0..#6] type = [int(8), uint(8), int(16), uint(16), int(32), uint(32), int(64), uint(64)];
// var c_types_to_check: [0..#6] type = [c_short, c_ushort, c_int, c_uint, c_long, c_ulong];

const lower_bounds : [0..#8] string = ["-128", "0", "-32768", "0", "-2147483648", "0", "-9223372036854775808", "0"];
const upper_bounds : [0..#8] string = ["127", "255", "32767", "65535", "2147483647", "4294967295", "9223372036854775807", "18446744073709551615"];

writeln("Checking Chapel Types:");
for i in 0..<8 {
    const b_lower_in = new bigint(lower_bounds[i]);
    const b_upper_in = new bigint(upper_bounds[i]);

    const b_lower_out = b_lower_in - 1;
    const b_upper_out = b_upper_in + 1;

    // writeln("in: (", b_lower_in, ", ", b_upper_in, ") \t out: (", b_lower_out, ", ", b_upper_out, ")");

    var correct = true;

    select i {
        when 0 {
            correct &= b_lower_in.fitsInto(int(8));
            correct &= b_upper_in.fitsInto(int(8));

            correct &= !b_lower_out.fitsInto(int(8));
            correct &= !b_upper_out.fitsInto(int(8));
        }
        when 1 {
            correct &= b_lower_in.fitsInto(uint(8));
            correct &= b_upper_in.fitsInto(uint(8));

            correct &= !b_lower_out.fitsInto(uint(8));
            correct &= !b_upper_out.fitsInto(uint(8));
        }
        when 2 {
            correct &= b_lower_in.fitsInto(int(16));
            correct &= b_upper_in.fitsInto(int(16));

            correct &= !b_lower_out.fitsInto(int(16));
            correct &= !b_upper_out.fitsInto(int(16));
        }
        when 3 {
            correct &= b_lower_in.fitsInto(uint(16));
            correct &= b_upper_in.fitsInto(uint(16));

            correct &= !b_lower_out.fitsInto(uint(16));
            correct &= !b_upper_out.fitsInto(uint(16));
        }
        when 4 {
            correct &= b_lower_in.fitsInto(int(32));
            correct &= b_upper_in.fitsInto(int(32));

            correct &= !b_lower_out.fitsInto(int(32));
            correct &= !b_upper_out.fitsInto(int(32));
        }
        when 5 {
            correct &= b_lower_in.fitsInto(uint(32));
            correct &= b_upper_in.fitsInto(uint(32));

            correct &= !b_lower_out.fitsInto(uint(32));
            correct &= !b_upper_out.fitsInto(uint(32));
        }
        when 6 {
            correct &= b_lower_in.fitsInto(int(64));
            correct &= b_upper_in.fitsInto(int(64));

            correct &= !b_lower_out.fitsInto(int(64));
            correct &= !b_upper_out.fitsInto(int(64));
        }
        when 7 {
            correct &= b_lower_in.fitsInto(uint(64));
            correct &= b_upper_in.fitsInto(uint(64));

            correct &= !b_lower_out.fitsInto(uint(64));
            correct &= !b_upper_out.fitsInto(uint(64));
        }
        otherwise { writeln("wut?"); }
    }

    writeln(correct);
}

writeln("Checking C Types:");
for i in 0..<8 {
    const b_lower_in = new bigint(lower_bounds[i]);
    const b_upper_in = new bigint(upper_bounds[i]);

    const b_lower_out = b_lower_in - 1;
    const b_upper_out = b_upper_in + 1;

    var correct = true;

    select i {
        when 0 {
            correct &= b_lower_in.fitsInto(c_schar);
            correct &= b_upper_in.fitsInto(c_schar);

            correct &= !b_lower_out.fitsInto(c_schar);
            correct &= !b_upper_out.fitsInto(c_schar);
        }
        when 1 {
            correct &= b_lower_in.fitsInto(c_uchar);
            correct &= b_upper_in.fitsInto(c_uchar);

            correct &= !b_lower_out.fitsInto(c_uchar);
            correct &= !b_upper_out.fitsInto(c_uchar);
        }
        when 2 {
            correct &= b_lower_in.fitsInto(c_short);
            correct &= b_upper_in.fitsInto(c_short);

            correct &= !b_lower_out.fitsInto(c_short);
            correct &= !b_upper_out.fitsInto(c_short);
        }
        when 3 {
            correct &= b_lower_in.fitsInto(c_ushort);
            correct &= b_upper_in.fitsInto(c_ushort);

            correct &= !b_lower_out.fitsInto(c_ushort);
            correct &= !b_upper_out.fitsInto(c_ushort);
        }
        when 4 {
            correct &= b_lower_in.fitsInto(c_int);
            correct &= b_upper_in.fitsInto(c_int);

            correct &= !b_lower_out.fitsInto(c_int);
            correct &= !b_upper_out.fitsInto(c_int);
        }
        when 5 {
            correct &= b_lower_in.fitsInto(c_uint);
            correct &= b_upper_in.fitsInto(c_uint);

            correct &= !b_lower_out.fitsInto(c_uint);
            correct &= !b_upper_out.fitsInto(c_uint);
        }
        when 6 {
            correct &= b_lower_in.fitsInto(c_long);
            correct &= b_upper_in.fitsInto(c_long);

            correct &= !b_lower_out.fitsInto(c_long);
            correct &= !b_upper_out.fitsInto(c_long);
        }
        when 7 {
            correct &= b_lower_in.fitsInto(c_ulong);
            correct &= b_upper_in.fitsInto(c_ulong);

            correct &= !b_lower_out.fitsInto(c_ulong);
            correct &= !b_upper_out.fitsInto(c_ulong);
        }
        otherwise { writeln("wut?"); }
    }

    writeln(correct);
}
