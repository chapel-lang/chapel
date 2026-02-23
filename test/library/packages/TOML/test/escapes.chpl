use TOML, IO;

var toml = parseToml(openReader("escapes.toml"));
writeln("toml output");
writeln(toml);


var mem = openMemFile();
var w = mem.writer();
w.writeln(toml);
w.close();

var roundTrippedToml = parseToml(mem.reader());
writeln("round-tripped toml output");
writeln(roundTrippedToml);

