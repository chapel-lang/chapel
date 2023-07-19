use TOML;

config const f: string;

proc main() {
  var tomlChannel = openReader(f);
  var tomlData = parseToml(tomlChannel);
  writeln(tomlData);

  delete tomlData;
  tomlChannel.close();
}
