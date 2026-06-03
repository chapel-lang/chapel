use TOML;
use IO;


const tomlFile = """
[myTable]
field1 = "value1"

[myTable.subTable]
field2 = "value2"
""";

proc toml2Str(toml) {
  var f = openMemFile();
  var w = f.writer();
  w.write(toml);
  w.close();
  var data = f.reader().readAll(string);
  return data;
}


var data = parseToml(tomlFile);

try {
  const unknown = data["unknown"];
  writeln("Unknown TOML entry: ", unknown);
} catch {
  writeln("Caught exception when accessing unknown TOML entry.");
}

writef("After accessing unknown TOML entry, data is:\n%s\n---\n", toml2Str(data));


try {
  const unknownNested = data["myTable.unknownField"];
  writeln("Unknown nested TOML entry: ", unknownNested);
} catch {
  writeln("Caught exception when accessing unknown nested TOML entry.");
}

writef("After accessing unknown TOML entry, data is:\n%s\n---\n", toml2Str(data));

