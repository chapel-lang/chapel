use TOML;
use FileSystem;
use MasonUtils;


/* Finds a Mason.toml file and updates the Mason.lock
   generating one if it doesnt exist */
proc UpdateLock(tf="Mason.toml", lf="Mason.lock") {
  if isFile(tf) {
    updateRegistry(tf);
    var openFile = openreader(tf);
    var TomlFile = parseToml(openFile);
    var lockFile = createDepTree(TomlFile);
    genLock(lockFile, lf);
    openFile.close();
    delete TomlFile;
    delete lockFile;
  }
  else {
    writeln("Cannot update: no Mason.toml found");
  }
}


/* Writes out the lock file */
proc genLock(lock: Toml, lf) {
  var lockFile = open(lf, iomode.cw);
  var tomlWriter = lockFile.writer();
  tomlWriter.writeln(lock);
  tomlWriter.close();
}

proc updateRegistry(tf: string) {
  var masonHome = getMasonHome();
  var registryHome = masonHome + '/.mason/registry';
  if isDir(registryHome) {
    var pullRegistry = 'git -C ' + registryHome + '/ pull -q origin master';
    if tf == "Mason.toml" then writeln("Updating mason-registry");
    runCommand(pullRegistry);
  }
  // Registry has moved or does not exist
  else {
    mkdir(masonHome + '/.mason');
    mkdir(masonHome + '/.mason/src');
    var registry = "https://github.com/chapel-lang/mason-registry";
    var cloneRegistry = 'git -C ' + masonHome + '/.mason/ clone -q ' + registry + ' registry';
    writeln('Could not find Registry...cloning registry...');
    runCommand(cloneRegistry);
  } 
}

/* Responsible for creating the dependency tree
   from the Mason.toml. Starts at the root of the
   project and continues down dep tree recursivly
   until each dep is recorded */
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
  if depList.domain.size > 0 {
    depTree[name]["dependencies"] = depList;
  }
  return depTree;
}



/* The Incompatible Version Resolution Stategy 
   - differing major versions are not allowed
   - Always newest minor and patch 
   - in accordance with semver  */
proc IVRS(version1: string, version2: string) {
  if version1 == version2 then return version1;
  var vers1 = version1.split('.');
  var vers2 = version2.split('.');
  var v1 = vers1(1): int;
  var v2 = vers2(1): int;
  if vers1(1) != vers2(1) {
    halt("Differing Major versions of dependencies are not supported");
  }
  else if vers1(2) != vers2(2) {
    v1 = vers1(2): int;
    v2 = vers2(2): int;
    if v1 > v2 {
      return version1;
    }
    else return version2;
  }
  else {
    v1 = vers1(3): int;
    v2 = vers2(3): int;
    if v1 > v2 {
      return version1;
    }
    else return version2;
  }
}




/* Returns the Mason.toml for each dep listed as a Toml */
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


/* Resposible for parsing the Mason.toml to be given
   back to a call from getManifests */ 
proc retrieveDep(name: string, version: string) {
  var home = getMasonHome();
  var tomlPath = home + "/.mason/registry/Bricks/"+name+"/"+version+".toml";
  if isFile(tomlPath) {
    var tomlFile = open(tomlPath, iomode.r);
    var depToml = parseToml(tomlFile);
    return depToml;
  }
  else {
    halt("Could not find toml file for " + name);
  }
}

/* Checks if a dependecy has deps; if so, the
   dependencies are returned as a (string, Toml) */
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

/* iterator to collect fields from a toml
   TODO custom fields returned */
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

