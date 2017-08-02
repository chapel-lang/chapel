


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
  var lockFile = createDepTree(TomlFile);
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
  

proc createDepTree(root: Toml) {
  var dp: domain(string);
  var dps: [dp] Toml;
  var depTree: Toml = dps;
  depTree["root"] = new Toml(root);

  if root.pathExists("dependencies") {
    var deps = getDependencies(root);
    var depTomls = getDepTomls(deps);
    var fullTree = createDepTrees(depTree, depTomls);
  }
  else {
    return depTree;
  }
} 

proc createDepTrees(depTree: Toml, deps: [?d]) { 
  if deps.d.size == 0 {
    return depTree;
  }
  else {
    var dep = deps[deps.d.first];
    var brick = dep["brick"];
    var package = brick["name"].toString();
    if depTree.pathExists(package) {
      halt("for now. res strat will go here");
    }
    depTree[package]["name"] = package;
    depTree[package]["version"] = brick["version"].toString();
    depTree[package]["source"] = brick["source"].toString();

    if dep.pathExists("dependencies") {
      var deps = getDependencies(root);
      var depTomls = getDepTomls(deps);
      var fullTree = createDepTrees(depTree, depTomls);

      //find a way to recurse through the toml files
      //while still adding the dependencies to each table
      // maybe a seperate helper function that takes in a list of tomls
      //   and spits back the correct dependency list? 
      // yea def do that
	 

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
