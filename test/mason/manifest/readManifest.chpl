use Package;
use TOML;
use JSON;
use IO;
import ThirdParty.Pathlib.path;
use ThirdParty.Pathlib.IOHelpers;

config const tomlPath: path;

proc main() {
  var reader = openReader(tomlPath);
  var m = MasonPackage.fromManifest(reader, tomlPath.parent);
  stdout.withSerializer(new jsonSerializer()).writeln(m);

  // var l = m.createLockFile();
  // writeln(l);
}
