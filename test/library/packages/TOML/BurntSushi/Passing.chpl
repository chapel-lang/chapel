use TOML, IO;

/*
  Tests from BurntSushi/toml-test 'valid' suite:

    https://github.com/BurntSushi/toml-test
*/

config const f = 'bool.toml';

proc main() {
  var tomlChannel = openreader(f);
  var tomlData = parseToml(tomlChannel);

  const j = f.replace('.toml', '.json.out');
  var jsonChannel = openwriter(j);

  tomlData.writeJSON(jsonChannel);

  jsonChannel.close();
  delete tomlData;
  tomlChannel.close();
}
