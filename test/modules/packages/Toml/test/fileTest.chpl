
use TOML;

config const f: string;

proc main() {
  var error: syserr;
  var TomlFile = open(error, f, iomode.r);
  var TomlData = parseToml(TomlFile);
  writeln(TomlData);

  delete TomlData;
}