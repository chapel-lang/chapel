use TOML;
use IO;

/*
  Tests from TOML spec:

    https://github.com/toml-lang/toml
*/

config const f = 'example.good';

proc main() {
  var tomlChannel = openreader(f);
  var tomlData = parseToml(tomlChannel);
  writeln(tomlData);
  delete tomlData;
  tomlChannel.close();
}
