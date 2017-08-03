
use TOML;
use FileSystem;
use system;

proc main() {
  var depName = "easy";
  var depVersion = "0.1.0";
  writeln(retrieveDep(depName, depVersion));
}

proc getManifests(deps: [?dom] (string, Toml)) {
  var manifests: [1..0] Toml;
  for dep in deps {
    var name = dep(1);
    var version: string = dep(2).s;
    var toAdd = retrieveDep(name, version);
    manifests.push_back(toAdd);
  }
  return manifests;
}
   


proc retrieveDep(name: string, version: string) {
  var home = getVar("homePath");
  var tomlPath = home + "/.mason/registry/"+name+"/"+version+".toml";
  if isFile(tomlPath) {
    var tomlFile = open(tomlPath, iomode.r);
    var depToml = parseToml(tomlFile);
    return depToml;
  }
  else {
      halt("Could not find toml file for " + name);
    }
}
  