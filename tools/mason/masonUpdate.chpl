

/*
Lock File generation depends on three parts of manifest
  1. Package Identifier (name)
  2. Address for retrieval (git repository)
  3. Immutable revision of software (commit hash)
*/

use TOML;
use getDep;

proc main(args: [] string) {
  var openFile = openreader(args[1]);
  var TomlFile = parseToml(openFile);
  var rootDeps = getDependencies(TomlFile);
  var dependencies = getDepTomls(rootDeps);
  var lockFile = createDepTree(TomlFile, dependencies);
  genLock(lockFile);
  openFile.close();
  delete TomlFile;
  delete lockFile;
}

proc genLock(lock: Toml) {
  var lockFile = open("Mason.lock", iomode.cw);
  var tomlWriter = lockFile.writer();
  tomlWriter.writeln(lock);
  tomlWriter.close();
}
  

proc createDepTree(root: Toml, deps: [?depsD]) { 
  var dp: domain(string);
  var dps: [dp] Toml;    // Future lock file
  var depTree: Toml = dps;

  depTree["root"] = new Toml(root["brick"]);
  

  var depList: [1..0] Toml;
  for dep in deps {
    var brick = dep["brick"];
    var depToAdd = brick["name"];
    depList.push_back(depToAdd);

    var deptblD: domain(string);
    var depTbl: [deptblD] Toml;
    var package = dep["name"].toString();
    depTree[package] = depTbl;
    depTree[package]["name"] = package;
    depTree[package]["version"] = dep["version"].toString();
    depTree[package]["source"] = dep["source"].toString();
    depTree[package]["dependencies"] = new Toml(dep["dependencies"]);
  }

  depTree["root"]["dependencies"] = depList;
  return depTree;
}
	 

proc getDependencies(tomlTbl: Toml) {
  var depsD: domain(1);
  var deps: [depsD] (string, Toml);
  for k in tomlTbl.D {
    if k == "dependencies" {
      for (a,d) in allFields(tomlTbl[k]) {
	deps.push_back((a, d));
      }
    }
  }
  return deps;
}

iter allFields(tomlTbl: Toml) { 
  for (k,v) in zip(tomlTbl.D, tomlTbl.A) {
    if v.tag == fieldToml {
      continue;
    }
    else {
      yield(k,v);
    }
  }
}
