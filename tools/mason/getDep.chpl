
use TOML;
use FileSystem;

/*
proc main() {
  var depName = "easy";
  var depVersion = "0.1.0";
}*/

proc getManifests(deps: [?dom] (string, Toml)) {
  var manifests: [1..0] Toml;
  for dep in deps {
    var name = dep(1);
    var version: string = dep(2).toString();
    var toAdd = retrieveDep(name, version);
    manifests.push_back(toAdd);
  }
  return manifests;
}
   


proc retrieveDep(name: string, ver: string) {
  var version = ver.strip('"');
  var tomlPath = "/Users/spartee/.mason/registry/"+name+"/"+version+".toml";
  if isFile(tomlPath) {
    var tomlFile = open(tomlPath, iomode.r);
    var depToml = parseToml(tomlFile);
    return depToml;
  }
  else {
    halt("Could not find toml file for " + name);
  }
}
  