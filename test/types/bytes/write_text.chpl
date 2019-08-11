var b: _bytes = "some\t\x00bytes with € (the euro char)";
writeln("Direct print: ", b);
writef("Print repr: %ht\n", b);
