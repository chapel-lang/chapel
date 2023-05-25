use TOML;
use IO;

/*
  Tests from TOML spec:

    https://github.com/toml-lang/toml
*/

config const f = 'example.good';

proc main() {
  var tomlChannel = openReader(f);
  var tomlData = parseToml(tomlChannel);
  writeln(tomlData);
  tomlChannel.close();
}
