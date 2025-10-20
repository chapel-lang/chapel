/*
 * Test that using a bool as a vararg count produces a user error
 * rather than an internal error.
 *
 * See https://github.com/chapel-lang/chapel/issues/20371
 */

proc test(param n: bool, args ... n) {
    writeln(args);
}

test(true, true);
