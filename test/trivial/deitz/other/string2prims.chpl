writeln("4":int(8));
writeln("4":int(16));
writeln("4":int(32));
writeln("4":int(64));

writeln("-4":int(8));
writeln("-4":int(16));
writeln("-4":int(32));
writeln("-4":int(64));

writeln("4":uint(8));
writeln("4":uint(16));
writeln("4":uint(32));
writeln("4":uint(64));

// BLC: commenting out upon deciding not to support this, and
// based on discussion with Steve
//writeln("-4":uint(8));
//writeln("-4":uint(16));
//writeln("-4":uint(32));
//writeln("-4":uint(64));

writeln("3.14":real(32));
writeln("3.14":real(64));

// BLC: don't believe that we should support these conversions;
// Steve, if you disagree, please discuss with me
//writeln("3.14":imag(32));
//writeln("3.14":imag(64));

writeln("3.14i":imag(32));
writeln("3.14i":imag(64));

writeln("3.14":complex(64));
writeln("3.14":complex(128));

writeln("3.14+2.1i":complex(64));
writeln("3.14+2.1i":complex(128));

writeln("true":bool);
writeln("false":bool);
writeln("maybe":bool);
