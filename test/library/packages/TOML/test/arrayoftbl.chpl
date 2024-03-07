use IO, TOML;

config const f: string;

proc main() {
  var tomlChannel = openReader(f, locking=false);
  var tomlData = parseToml(tomlChannel);
  writeln(tomlData);

  tomlChannel.close();
}
