use CTypes;
extern const ComplexVar:c_int;

proc readMacro() { return ComplexVar; }

writeln(ComplexVar);
writeln(readMacro());
writeln(ComplexVar);
