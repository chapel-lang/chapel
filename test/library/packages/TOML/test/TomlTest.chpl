use IO, TOML;

config const f: string;

proc main() {
  var tomlChannel = openReader(f);
  var tomlData = parseToml(tomlChannel);
  writeln(tomlData);
  tomlChannel.close();
}
