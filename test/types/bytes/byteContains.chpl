
var b = b"Hello, hello, hello";
var c = b"cccccccccccccccccccccccccccc";
var empty: bytes = b"";

// Basic positive cases
writeln(b.contains(b"Hello"));
writeln(b.contains(b"hello"));
writeln(b.contains(b", "));
writeln(b.contains(b"Hello, hello, hello"));

// Basic negative cases
writeln(b.contains(b"xyz"));
writeln(b.contains(b"HELLO"));
writeln(b.contains(b"Hello, hello, hello!"));

// Repeated characters
writeln(c.contains(b"c"));
writeln(c.contains(b"C"));

// Empty cases
writeln(b.contains(b""));
writeln(empty.contains(b"hello"));
writeln(empty.contains(b""));

// With indices (restrict search range)
writeln(b.contains(b"Hello", 0..4));
writeln(b.contains(b"Hello", 1..18));
writeln(b.contains(b"hello", 7..18));
writeln(b.contains(b"hello", 0..4));
