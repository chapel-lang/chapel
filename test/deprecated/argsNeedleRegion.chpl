var hello = "hello, world, world";

writeln("find: ", hello.find(needle="world"));
writeln("find: ", hello.find("world", region=11..));
writeln("find: ", hello.find(needle="world", region=11..));

writeln("rfind: ", hello.rfind(needle="world"));
writeln("rfind: ", hello.rfind("world", region=..12));
writeln("rfind: ", hello.rfind(needle="world", region=..12));

writeln("count: ", hello.count(needle="world"));
writeln("count: ", hello.count("world", region=11..));
writeln("count: ", hello.count(needle="world", region=11..));

writeln("replace: ", hello.replace(needle="world", "worlds"));

var byte_b = b"this is a bytes";

writeln("byte find: ", byte_b.find(needle=b"is"));
writeln("byte find: ", byte_b.find(b"is", region=4..));
writeln("byte find: ", byte_b.find(needle=b"is", region=4..));

writeln("byte rfind: ", byte_b.rfind(needle=b"is"));
writeln("byte rfind: ", byte_b.rfind(b"is", region=..5));
writeln("byte rfind: ", byte_b.rfind(needle=b"is", region=..5));

writeln("byte count: ", byte_b.count(needle=b"is"));
writeln("byte count: ", byte_b.count(b"is", region=..4));
writeln("byte count: ", byte_b.count(needle=b"is", region=..4));

writeln("byte replace: ", byte_b.replace(needle=b"is is a", b"ese are"));

