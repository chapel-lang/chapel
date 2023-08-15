// should emit a warning:
writef("'.3i': %.3i\n", 1);
writef("'.3u': %.3u\n", 1);

// behavior of %n and %r should match for integer values wrt precision:
writef("'.3n': %.3n\n", 1);
writef("'.3r': %.3r\n", 1);

// should also match for a real value:
writef("'.3n' of '1.0': %.3n\n", 1.0);
writef("'.3r' of '1.0': %.3r\n", 1.0);
