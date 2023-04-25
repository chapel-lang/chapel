use TOML, IO;

/*
  Tests from BurntSushi/toml-test 'valid' suite:

    https://github.com/BurntSushi/toml-test
*/

config const f = 'bool.toml';

proc main() {
  var tomlChannel = openReader(f);
  var tomlData = parseToml(tomlChannel);

  const j = f.replace('.toml', '.json.out');
  var jsonChannel = openWriter(j);

  tomlData.writeJSON(jsonChannel);

  jsonChannel.close();
  tomlChannel.close();
}
