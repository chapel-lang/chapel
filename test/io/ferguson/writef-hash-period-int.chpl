writef("%{###.}\n", 123);
writef("%{###.}\n", 1);

writef("%{###.}\n", 12.1);

// also test ##.## with integers
writef("%{###.##}\n", 123);
// and the integer version with no .
writef("%{###}\n", 123);
