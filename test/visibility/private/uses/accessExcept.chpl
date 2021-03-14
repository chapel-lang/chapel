use M;

writeln(a);
// Ensures that except lists doesn't break the privacy of the use.  I shouldn't
// be able to see 'a'
writeln("from accessExcept");
