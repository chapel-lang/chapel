/*
 * Test that using a non-integral type (string) as a vararg count 
 * produces a user error rather than an internal error.
 *
 * See https://github.com/chapel-lang/chapel/issues/20371
 */

proc test(param n: string, args ... n) {
    writeln(args);
}

test("hello", 1);
