use TOML, IO;

config const f: string;

proc main() {
  var TomlFile = open(f, ioMode.r);
  var TomlData = parseToml(TomlFile);
  writeln(TomlData);
}
