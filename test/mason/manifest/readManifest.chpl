use Manifest;
use TOML;
use IO;


config const tomlPath: string;

proc main() {
  var reader = openReader(tomlPath);
  var m = manifestFile.fromToml(reader);
  writeln(m);
}

