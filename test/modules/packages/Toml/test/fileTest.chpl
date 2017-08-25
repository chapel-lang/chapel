use TOML;

config const f: string;

proc main() {
  var TomlFile: file;
  try! {
    TomlFile = open(f, iomode.r);
  }
  var TomlData = parseToml(TomlFile);
  writeln(TomlData);

  delete TomlData;
}
