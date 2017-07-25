use TOML;

config const file: string;

proc main() {
  var tomlChannel = openreader(file);
  var tomlData = parseToml(tomlChannel);
  writeln(tomlData);
}