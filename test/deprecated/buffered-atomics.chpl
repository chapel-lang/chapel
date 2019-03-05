use BufferedAtomics;

var a: atomic int;
a.addBuff(1);
flushAtomicBuff();
writeln(a);
