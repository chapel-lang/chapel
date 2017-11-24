
use TOML;

config const str: string = "[table] \
key = 'value' ";

proc main() {
  var TomlData = parseToml(str);
  writeln(TomlData);

  delete TomlData;
}

