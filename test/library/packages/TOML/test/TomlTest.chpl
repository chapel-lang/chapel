use IO, TOML;

config const f: string;

proc main() {
  var tomlChannel = openreader(f);
  var tomlData = parseToml(tomlChannel);
  writeln(tomlData);
  delete tomlData;
  tomlChannel.close();
}
