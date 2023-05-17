/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

use FileSystem;
use List;
use Map;
use ArgumentParser;
use MasonEnv;
use MasonExternal;
use MasonHelp;
use MasonSystem;
use MasonUtils;
use TOML;
import Path;

/*
Update: Performs the upfront dependency resolution and generates the lock file.

Incompatible Version Resolution Strategy:

The current resolution strategy for Mason 0.1.0 is the IVRS as described below:
    1. If multiple bug fixes of a package are present in the project,
       mason will use the latest bug fix. (ex. 1.1.0, 1.1.1 --> 1.1.1)
    2. If multiple minor versions of a package are present in the project,
       mason will use the latest minor version within the common major version.
       (ex. 1.4.3, 1.7.0 --> 1.7)
    3. If multiple major versions are present, mason will print an error.
       (ex. 1.13.0, 2.1.0 --> incompatible)
*/

private var failedChapelVersion: list(string);

proc masonUpdate(args: [?d] string) {
  var tf = "Mason.toml";
  var lf = "Mason.lock";
  var skipUpdate = MASON_OFFLINE;

  var parser = new argumentParser(helpHandler=new MasonUpdateHelpHandler());

  var updateFlag = parser.addFlag(name="update", flagInversion=true);

  parser.parseArgs(args);

  if updateFlag.hasValue() {
    skipUpdate = !updateFlag.valueAsBool();
  }

  return updateLock(skipUpdate, tf, lf);
}

/* Finds a Mason.toml file and updates the Mason.lock
   generating one if it doesnt exist */
proc updateLock(skipUpdate: bool, tf="Mason.toml", lf="Mason.lock", show=true) {

  try! {
    const cwd = here.cwd();
    const projectHome = getProjectHome(cwd, tf);
    const tomlPath = projectHome + "/" + Path.relPath(tf);
    const lockPath = projectHome + "/" + Path.relPath(lf);
    const openFile = openReader(tomlPath);
    const TomlFile = parseToml(openFile);
    var updated = false;
    if isFile(tomlPath) {
      if TomlFile.pathExists('dependencies') {
        if TomlFile['dependencies']!.A.size > 0 {
          updateRegistry(skipUpdate, show);
          updated = true;
        }
      }
      if !updated && show {
        writeln("Skipping registry update since no dependency found in manifest file.");
      }
    }
    if isDir(SPACK_ROOT) && TomlFile.pathExists('external') {
      if getSpackVersion < spackVersion then
      throw new owned MasonError("Mason has been updated. " +
                  "To install Spack, call: mason external --setup.");
    }
    const lockFile = createDepTree(TomlFile);
    if failedChapelVersion.size > 0 {
      const prefix = if failedChapelVersion.size == 1
        then "The following package is"
        else "The following packages are";
      stderr.writeln(prefix, " incompatible with your version of Chapel (", getChapelVersionStr(), ")");
      for msg in failedChapelVersion do
        stderr.writeln("  ", msg);
      exit(1);
    }
    // Generate Lock File
    genLock(lockFile, lockPath);
    // Close Memory
    openFile.close();

  }
  catch e: MasonError {
    stderr.writeln(e.message());
    exit(1);
  }
  return (tf, lf);
}


/* Writes out the lock file */
proc genLock(lock: borrowed Toml, lf: string) {
  const lockFile = open(lf, ioMode.cw);
  const tomlWriter = lockFile.writer();
  tomlWriter.writeln(lock);
  tomlWriter.close();
  lockFile.close();
}

proc checkRegistryChanged() {
  for ((_, registry), cached) in zip(MASON_REGISTRY, MASON_CACHED_REGISTRY) {
    if !isDir(cached) {
      return;
    }

    const oldRegistry = gitC(cached, "git config --get remote.origin.url", quiet=true).strip();

    if oldRegistry != registry {
      writeln("MASON_REGISTRY changed since last update:");
      writeln("  old: ", oldRegistry);
      writeln("  new: ", registry);
      writeln();
      writeln("Removing cached registry and sources to avoid conflicts");

      proc tryRemove(name : string) {
        if isDir(name) {
          writeln("Removing ", name);
          rmTree(name);
        }
      }

      tryRemove(cached);
      tryRemove(MASON_HOME + "/src");
    }
  }
}

/* Pulls the mason-registry. Cloning if !exist */
proc updateRegistry(skipUpdate: bool, show=true) {
  if skipUpdate then return;

  checkRegistryChanged();
  for ((name, registry), registryHome) in zip(MASON_REGISTRY, MASON_CACHED_REGISTRY) {

    if isDir(registryHome) {
      var pullRegistry = 'git pull -q origin master';
      if show then writeln("Updating ", name);
      gitC(registryHome, pullRegistry);
    }
    // Registry has moved or does not exist
    else {
      mkdir(MASON_HOME + '/src', parents=true);
      const localRegistry = registryHome;
      mkdir(localRegistry, parents=true);
      const cloneRegistry = 'git clone -q ' + registry + ' .';
      if show then writeln("Updating ", name);
      gitC(localRegistry, cloneRegistry);
    }
  }
}

proc verifyChapelVersion(brick:borrowed Toml) {
  const tupInfo = getChapelVersionInfo();
  const current = new VersionInfo(tupInfo(0), tupInfo(1), tupInfo(2));
  var low, hi : VersionInfo;

  var ret = false;

  (low, hi) = parseChplVersion(brick);
  ret = low <= current && current <= hi;

  return (ret, low, hi);
}

proc prettyVersionRange(low, hi) {
  if low == hi then
    return low.str();
  else if hi.containsMax() then
    return low.str() + " or later";
  else
    return low.str() + ".." + hi.str();
}

proc chplVersionError(brick:borrowed Toml) {
  const info = verifyChapelVersion(brick);
  if info(0) == false {
    const low  = info(1);
    const hi   = info(2);
    const name = brick["name"]!.s + "-" + brick["version"]!.s;
    const msg  = name + " :  expecting " + prettyVersionRange(low, hi);
    failedChapelVersion.pushBack(msg);
  }
}

/* Responsible for creating the dependency tree
   from the Mason.toml. Starts at the root of the
   project and continues down dep tree recursively
   until each dep is recorded */
private proc createDepTree(root: Toml) {
  var dp: domain(string);
  var dps: [dp] shared Toml?;
  var depTree = new shared Toml(dps);
  if root.pathExists("brick") {
    depTree.set("root", new shared Toml(root["brick"]!));
  }
  else {
    stderr.writeln("Could not find brick; Mason cannot update");
    exit(1);
  }

  if root.pathExists("dependencies") {
    var deps = getDependencies(root);

    // gitDeps is a list of (name, url, branch, revision)
    // git repositories that don't have a revision specified
    // in the TOML will always be updated to the latest revision
    var gitDeps = pullGitDeps(getGitDeps(root));
    var manifests = getManifests(deps);
    var gitManifests = getGitManifests(gitDeps);

    // add dependencies found in TOML files of git deps
    for m in gitManifests do
      manifests.pushBack(m);

    depTree = createDepTrees(depTree, manifests, "root");
    depTree = addGitDeps(depTree, gitDeps);
  }

  //
  // At this point 'dependencies' is a list of package names without any
  // version or source information. This loop updates the strings to follow
  // the format of "<package-name> <version> <source>"
  //
  // We should not set the version in these dependencies while creating the
  // dependency tree because IVRS may not execute in the desired order. For
  // example, we might encounter foo-0.3.0 before foo-0.2.0.
  //
  for b in depTree.A.values() {
    var brick = b!;
    chplVersionError(brick);

    // Lock in the current Chapel version
    const curVer = getChapelVersionStr();
    brick.set("chplVersion", curVer + ".." + curVer);

    if brick.pathExists("dependencies") {
      for item in brick["dependencies"]!.arr {
        const brick = depTree[item!.s]!;
        item!.s = brick["name"]!.s + " " + brick["version"]!.s + " " + brick["source"]!.s;
      }
    }
  }

  // Check for pkg-config dependencies
  if root.pathExists("system") {
    const exDeps = getPCDeps(root["system"]!);
    depTree.set("system", exDeps);
  }

  // Check for non-Chapel dependencies
  if root.pathExists("external") {
    const externals = getExternalPackages(root["external"]!);
    depTree.set("external", externals);
  }
  return depTree;
}

private proc createDepTrees(depTree: Toml, ref deps: list(shared Toml), name: string) : shared Toml {
  var depList: list(shared Toml?);
  while deps.size > 0 {
    var dep = deps[0];

    var brick       = dep["brick"]!;
    var package     = brick["name"]!.s;
    var version     = brick["version"]!.s;
    var chplVersion = brick["chplVersion"]!.s;
    var source      = brick["source"]!.s;

    if depTree.pathExists(package) {
      var verToUse = IVRS(brick, depTree[package]!);
      version = verToUse["version"]!.s;
      chplVersion = verToUse["chplVersion"]!.s;
    }

    depList.pushBack(new shared Toml(package));

    if depTree.pathExists(package) == false {
      var dt: domain(string);
      var depTbl: [dt] shared Toml?;
      depTree.set(package, depTbl);
    }
    depTree[package]!.set("name", package);
    depTree[package]!.set("version", version);
    depTree[package]!.set("chplVersion", chplVersion);
    depTree[package]!.set("source", source);

    if dep!.pathExists("dependencies") {
      var subDeps = getDependencies(dep);
      var manifests = getManifests(subDeps);
      var dependency = createDepTrees(depTree, manifests, package);
    }
    deps.pop(0);
  }
  // Use toArray here to avoid making Toml aware of `list`, for now.
  if depList.size > 0 then
    depTree[name]!.set("dependencies", depList.toArray());
  return depTree;
}

private proc addGitDeps(depTree: Toml, ref gitDeps) {
  //val url branch revision
  for key in gitDeps {
    if !depTree.pathExists(key[0]) {
      var dt: domain(string);
      var depTbl: [dt] shared Toml?;
      depTree.set(key[0], depTbl);
      depTree[key[0]]!.set("name", key[0]);
    }
    depTree[key[0]]!.set("source", key[1]);
    if key[2] != "HEAD" then
      depTree[key[0]]!.set("branch", key[2]);
    depTree[key[0]]!.set("rev", key[3]);

    // version of -1 is a special
    depTree[key[0]]!.set("version", "-1");
  }
  return depTree;
}

//
// TODO: Accept an array of bricks
//
/* The Incompatible Version Resolution Strategy
   - differing major versions are not allowed
   - Always newest minor and patch
   - in accordance with semantic versioning  */
private proc IVRS(A: borrowed Toml, B: borrowed Toml) {
  const name = A["name"]!.s;
  const (okA, Alo, Ahi) = verifyChapelVersion(A);
  const (okB, Blo, Bhi) = verifyChapelVersion(B);
  const version1 = A["version"]!.s;
  const version2 = B["version"]!.s;
  if okA == false && okB == false {
    stderr.writeln("Dependency resolution error: unable to find version of '", name, "' compatible with your version of Chapel (", getChapelVersionStr(), "):");
    stderr.writeln("  v", version1, " expecting ", prettyVersionRange(Alo, Ahi));
    stderr.writeln("  v", version2, " expecting ", prettyVersionRange(Blo, Bhi));
    exit(1);
  } else if okA == true && okB == false {
    return A;
  } else if okA == false && okB == true {
    return B;
  }

  if version1 == version2 then return A;

  var vers1 = version1.split('.');
  var vers2 = version2.split('.');
  var v1 = vers1(0): int;
  var v2 = vers2(0): int;
  if vers1(0) != vers2(0) {
    stderr.writeln("Dependency resolution error: package '", name, "' used by multiple packages expecting different major versions:");
    stderr.writeln("  v", version1);
    stderr.writeln("  v", version2);
    exit(1);
  }
  else if vers1(1) != vers2(1) {
    v1 = vers1(1): int;
    v2 = vers2(1): int;
    if v1 > v2 {
      return A;
    }
    else return B;
  }
  else {
    v1 = vers1(2): int;
    v2 = vers2(2): int;
    if v1 > v2 {
      return A;
    }
    else return B;
  }
}




/* Returns the Mason.toml for each dep listed as a Toml */
private proc getManifests(deps: list((string, shared Toml?))) {
  var manifests: list(shared Toml);
  for dep in deps {
    var name = dep(0);
    var version: string = dep(1)!.s;
    var toAdd = retrieveDep(name, version);
    manifests.pushBack(toAdd);
  }
  return manifests;
}


/* Responsible for parsing the Mason.toml to be given
   back to a call from getManifests */
private proc retrieveDep(name: string, version: string) {
  for cached in MASON_CACHED_REGISTRY {
    const tomlPath = cached + "/Bricks/"+name+"/"+version+".toml";
    if isFile(tomlPath) {
      var tomlFile = open(tomlPath, ioMode.r);
      var depToml = parseToml(tomlFile);
      return depToml;
    }
  }

  stderr.writeln("No toml file found in mason-registry for " + name +'-'+ version);
  exit(1);
}

/* Returns the Mason.toml for each dep listed as a Toml */
private proc getGitManifests(deps: list((string, string, string, string))) {
  var manifests: list(shared Toml);
  for dep in deps {
    var toAdd = retrieveGitDep(dep(0), dep(2));
    manifests.pushBack(toAdd);
  }
  return manifests;
}

/* Responsible for parsing the Mason.toml that have been
   already pulled down from git dependencies */
private proc retrieveGitDep(name: string, branch: string) {
  var baseDir = MASON_HOME +'/git/';
  const tomlPath = baseDir + "/"+name+"-"+branch+"/Mason.toml";
  if isFile(tomlPath) {
    var tomlFile = open(tomlPath, ioMode.r);
    var depToml = parseToml(tomlFile);
    return depToml;
  }

  stderr.writeln("No toml file found in git dependency for " + name +'-'+ branch);
  exit(1);
}

/* Checks if a dependency has deps; if so, the
   dependencies are returned as a (string, Toml) */
private proc getDependencies(tomlTbl: Toml) {
  var depsD: domain(1);
  var deps: list((string, shared Toml?));
  for k in tomlTbl.A.keys() {
    if k == "dependencies" {
      for (a,d) in allFields(tomlTbl[k]!) {
        deps.pushBack((a, d));
      }
    }
  }
  return deps;
}

private proc getGitDeps(tomlTbl: Toml) {
  var gitDeps: list((string, string, shared Toml?));
  for k in tomlTbl["dependencies"]!.A.keys() {
    for (a, d) in allFields(tomlTbl["dependencies"]![k]!) {
      // name, type of field (url, branch, etc.), toml that it is set to
      gitDeps.pushBack((k, a, d));
    }
  }
  return gitDeps;
}

private proc pullGitDeps(gitDeps, show=false) {
  if !isDir(MASON_HOME + '/git/') {
    mkdir(MASON_HOME + '/git/', parents=true);
  }

  var gitDepsWithRevision: list((string, string, string, string));

  var gitDepMap: map(string, (string, string, string));

  // form map of name -> url, branch, revision
  for val in gitDeps {
    if val[1] == "git" then
      gitDepMap[val[0]][0] = val[2]!.s;
    else if val[1] == "branch" then
      gitDepMap[val[0]][1] = val[2]!.s;
    else if val[1] == "rev" then
      gitDepMap[val[0]][2] = val[2]!.s;
  }

  // Pull git repositories so that we can have access to the
  // current revision and TOML file to get dependencies
  var baseDir = MASON_HOME +'/git/';
  for val in gitDepMap.keys() {
    var (srcURL, origBranch, revision) = gitDepMap[val];

    // Default to head if branch isn't specified
    var branch = if origBranch == "" then "HEAD" else origBranch;
    const nameVers = val + "-" + branch;
    const destination = baseDir + nameVers;
    if !depExists(nameVers, '/git/') {
      writeln("Downloading dependency: " + nameVers);
      var getDependency = "git clone -q "+ srcURL + ' ' + destination +'/';
      runCommand(getDependency);

      if (branch != "HEAD") || (revision != "") {
        // Use the revision to checkout, if specified
        var toCheckout = if revision != "" then revision else branch;
        var checkout = "git checkout -q " + toCheckout;
        if show {
          getDependency = "git clone " + srcURL + ' ' + destination + '/';
          checkout = "git checkout " + toCheckout;
        }

        gitC(destination, checkout);
      }

      // get the revision to store in lock if not specified
      if revision == "" {
        var revParse = "git rev-parse HEAD";
        revision = gitC(destination, revParse, true).strip();
      }
      gitDepsWithRevision.pushBack((val, srcURL, branch, revision));
    } else {
      if revision != "" {
        writeln("Fetching latest changes for: " + nameVers + "...");
        var pullDependency = "git fetch -q --all";
        if show then pullDependency = "git fetch --all";
        gitC(destination, pullDependency);

        writeln("Checking out specified revision for " + nameVers + "...");
        // Use the revision to checkout, if specified
        var checkout = "git checkout -q " + revision;
        if show then checkout = "git checkout " + revision;

        gitC(destination, checkout);
      } else if branch != "HEAD" {
        writeln("Fetching latest changes for: " + nameVers + "...");
        var pullDependency = "git fetch -q --all";
        if show then pullDependency = "git fetch --all";
        gitC(destination, pullDependency);

        writeln("Checking out specified revision for " + nameVers + "...");

        var checkout = "git checkout -q " + branch;
        if show then checkout = "git checkout " + branch;

        gitC(destination, checkout);
      }

      // get the revision to store in lock if not specified
      if revision == "" {
        var revParse = "git rev-parse HEAD";
        revision = gitC(destination, revParse, true).strip();
      }
      gitDepsWithRevision.pushBack((val, srcURL, branch, revision));
    }
  }
  return gitDepsWithRevision;
}
