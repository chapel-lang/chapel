use BigInteger;
use CTypes;

const lower_bounds : [0..#8] string = ["-128", "0", "-32768", "0", "-2147483648", "0", "-9223372036854775808", "0"];
const upper_bounds : [0..#8] string = ["127", "255", "32767", "65535", "2147483647", "4294967295", "9223372036854775807", "18446744073709551615"];

proc check_bounds(test_idx: int, type t: integral) {
    const b_lower_in = new bigint(lower_bounds[test_idx]);
    const b_upper_in = new bigint(upper_bounds[test_idx]);

    const b_lower_out = b_lower_in - 1;
    const b_upper_out = b_upper_in + 1;

    writeln(b_lower_in.fitsInto(t) && b_upper_in.fitsInto(t) &&
        !b_lower_out.fitsInto(t) && !b_upper_out.fitsInto(t));
}

writeln("Checking Chapel Types:");
check_bounds(0, int(8));
check_bounds(1, uint(8));
check_bounds(2, int(16));
check_bounds(3, uint(16));
check_bounds(4, int(32));
check_bounds(5, uint(32));
check_bounds(6, int(64));
check_bounds(7, uint(64));

writeln("Checking C Types:");
check_bounds(0, c_schar);
check_bounds(1, c_uchar);
check_bounds(2, c_short);
check_bounds(3, c_ushort);
check_bounds(4, c_int);
check_bounds(5, c_uint);
check_bounds(6, c_long);
check_bounds(7, c_ulong);
