


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
  if root.pathExists("brick") {
    depTree["root"] = new Toml(root["brick"]);
  }
  else {
    halt("Could not find brick; Mason cannot update");
  }

  if root.pathExists("dependencies") {
    var deps = getDependencies(root);
    var manifests = getManifests(deps);
    var fullTree = createDepTrees(depTree, manifests, "root");
    return fullTree;
  }
  else {
    return depTree;
  }
} 

proc createDepTrees(depTree: Toml, deps: [?d] Toml, name: string) : Toml { 
  var depList: [1..0] Toml;
  while deps.domain.size > 0 {
    var dep = deps[deps.domain.first];
    var brick = dep["brick"];
    var package = brick["name"].s;
    var version = brick["version"].s;
    if depTree.pathExists(package) {
      var verToUse = IVRS(version, depTree[package]["version"].s);
      version = verToUse;
    }
    var source = brick["source"].s;
    var toAdd: Toml = package + ' ' + version + ' ' + source;
    depList.push_back(toAdd);


    var dt: domain(string);
    var depTbl: [dt] Toml;
    depTree[package] = depTbl;
    depTree[package]["name"] = package;
    depTree[package]["version"] = version;
    depTree[package]["source"] = source;      
    
    
    if dep.pathExists("dependencies") {
      var subDeps = getDependencies(dep);
      var manifests = getManifests(subDeps);
      var dependency = createDepTrees(depTree, manifests, package);
    }
    deps.remove(deps.domain.first);
  }
  depTree[name]["dependencies"] = depList;
  return depTree;
}


proc IVRS(version1: string, version2: string) {
  if version1 == version2 then return version1;
  var vers1 = version1.split('.');
  var vers2 = version2.split('.');
  if vers1(1) != vers2(1) {
    halt("Differing Major versions of dependencies are not allowed");
  }
  else if vers1(2) != vers2(2) {
    var v1 = ascii(vers1(2));
    var v2 = ascii(vers2(2));
    if v1 > v2 {
      writeln(v1, " ", v2);
      return version1;
    }
    else return version2;
  }
  else {
    var v1 = ascii(vers1(3));
    var v2 = ascii(vers2(3));
    if v1 > v2 {
      return version1;
    }
    else return version2;
  }
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
