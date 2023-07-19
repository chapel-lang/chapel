inline proc param string.length param do return __primitive("string_length_codepoints", this);

writeln("test".length);
