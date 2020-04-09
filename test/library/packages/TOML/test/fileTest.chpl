use TOML, IO;

config const f: string;

proc main() {
  var TomlFile = open(f, iomode.r);
  var TomlData = parseToml(TomlFile);
  writeln(TomlData);

  delete TomlData;
}
