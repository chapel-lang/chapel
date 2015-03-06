// This test exercises interpretation of octal, hex, and binary configs.
config const foo: int = 0o777;
config const bar: int = 0xBEEF;
config const baz: int = 0b1010;

// Comments below denote the base case
write(foo);
writef(" %oi\n", foo); // The line this completes should read: 511 777
write(bar);
writef(" %xi\n", bar); // The line this completes should read: 48879 beef
write(baz);
writef(" %bi\n", baz); // The line this completes should read: 10 1010
