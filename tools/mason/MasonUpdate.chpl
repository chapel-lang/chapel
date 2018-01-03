/*
 * Copyright 2004-2018 Cray Inc.
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

use TOML;
use FileSystem;
use MasonUtils;
use MasonEnv;

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

private var failedChapelVersion : [1..0] string;

/* Finds a Mason.toml file and updates the Mason.lock
   generating one if it doesnt exist */
proc UpdateLock(args: [] string, tf="Mason.toml", lf="Mason.lock") {
  if isFile(tf) {
    updateRegistry(tf, args);
    var openFile = openreader(tf);
    var TomlFile = parseToml(openFile);
    var lockFile = createDepTree(TomlFile);

    if failedChapelVersion.size > 0 {
      const prefix = if failedChapelVersion.size == 1
                     then "The following package is"
                     else "The following packages are";
      stderr.writeln(prefix, " incompatible with your version of Chapel (", getChapelVersionStr(), ")");
      for msg in failedChapelVersion do
        stderr.writeln("  ", msg);
      exit(1);
    }

    genLock(lockFile, lf);
    openFile.close();
    delete TomlFile;
    delete lockFile;
  }
  else writeln("Cannot update: no Mason.toml found");
}


/* Writes out the lock file */
proc genLock(lock: Toml, lf) {
  var lockFile = open(lf, iomode.cw);
  var tomlWriter = lockFile.writer();
  tomlWriter.writeln(lock);
  tomlWriter.close();
}

proc checkRegistryChanged() {
  if isDir(MASON_CACHED_REGISTRY) == false {
    return;
  }

  const oldRegistry = gitC(MASON_CACHED_REGISTRY, "git config --get remote.origin.url", quiet=true).strip();

  if oldRegistry != MASON_REGISTRY {
    writeln("MASON_REGISTRY changed since last update:");
    writeln("  old: ", oldRegistry);
    writeln("  new: ", MASON_REGISTRY);
    writeln();
    writeln("Removing cached registry and sources to avoid conflicts");

    proc tryRemove(name : string) {
      if isDir(name) {
        writeln("Removing ", name);
        rmTree(name);
      }
    }

    tryRemove(MASON_CACHED_REGISTRY);
    tryRemove(MASON_HOME + "/src");
  }
}

/* Pulls the mason-registry. Cloning if !exist */
proc updateRegistry(tf: string, args : [] string) {
  for a in args {
    if a == "--no-update-registry" {
      return;
    }
  }

  checkRegistryChanged();

  const registryHome = MASON_CACHED_REGISTRY;
  if isDir(registryHome) {
    var pullRegistry = 'git pull -q origin master';
    if tf == "Mason.toml" then writeln("Updating mason-registry");
    gitC(registryHome, pullRegistry);
  }
  // Registry has moved or does not exist
  else {
    mkdir(MASON_HOME + '/src', parents=true);
    const localRegistry = MASON_CACHED_REGISTRY;
    mkdir(localRegistry, parents=true);
    const cloneRegistry = 'git clone -q ' + MASON_REGISTRY + ' .';
    writeln("Initializing mason-registry...");
    gitC(localRegistry, cloneRegistry);
  }
}

private const maxVersion = new VersionInfo(max(int), max(int), max(int));

proc parseChplVersion(brick:Toml) {
  use Regexp;

  if brick.pathExists("chplVersion") == false {
    const name = brick["name"].s + "-" + brick["version"].s;
    stderr.writeln("Brick '", name, "' missing required 'chplVersion' field");
    exit(1);
  }

  const chplVersion = brick["chplVersion"].s;
  var low, hi : VersionInfo;

  try {
    const formatMessage = "\n\n" +
    "chplVersion format must be '<version>..<version>' or '<version>'\n" +
    "A <version> must be in one of the following formats:\n" +
    "  x.x.x\n" +
    "  x.x\n" +
    "where 'x' is a positive integer.\n";

    var versions = chplVersion.split("..");
    [v in versions] v = v.strip();

    // Expecting 1 or 2 version strings
    if versions.size > 2 || versions.size < 1 {
      throw new MasonError("Expecting 1 or 2 versions in chplVersion range." + formatMessage);
    } else if versions.size == 2 && (versions[1] == "" || versions[2] == "") {
      throw new MasonError("Unbounded chplVersion ranges are not allowed." + formatMessage);
    }

    proc parseString(ver:string) throws {
      var ret : VersionInfo;

      // Finds 'x.x' or 'x.x.x' where x is a positive number
      const pattern = "^(\\d+\\.\\d+(\\.\\d+)?)$";
      var semver : string;
      if compile(pattern).match(ver, semver).matched == false {
        throw new MasonError("Invalid Chapel version format: " + ver + formatMessage);
      }
      const nums = for s in semver.split(".") do s:int;
      ret.major = nums[1];
      ret.minor = nums[2];
      if nums.size == 3 then ret.bug = nums[3];

      return ret;
    }

    low = parseString(versions[1]);

    if (versions.size == 1) {
      hi = maxVersion;
    } else {
      hi = parseString(versions[2]);
    }

    if (low <= hi) == false then
      throw new MasonError("Lower bound of chplVersion must be <= upper bound: " + low.str() + " > " + hi.str());
  } catch e : Error {
    const name = brick["name"].s + "-" + brick["version"].s;
    stderr.writeln("Invalid chplVersion in package '", name, "': ", chplVersion);
    stderr.writeln("Details: ", e.message());
    exit(1);
  }

  return (low, hi);
}

proc verifyChapelVersion(brick:Toml) {
  const tupInfo = getChapelVersionInfo();
  const current = new VersionInfo(tupInfo(1), tupInfo(2), tupInfo(3));
  var low, hi : VersionInfo;

  var ret = false;

  (low, hi) = parseChplVersion(brick);
  ret = low <= current && current <= hi;

  return (ret, low, hi);
}

proc prettyVersionRange(low, hi) {
  if low == hi then
    return low.str();
  else if hi == maxVersion then
    return low.str() + " or later";
  else
    return low.str() + ".." + hi.str();
}

proc chplVersionError(brick:Toml) {
  const info = verifyChapelVersion(brick);
  if info(1) == false {
    const low  = info(2);
    const hi   = info(3);
    const name = brick["name"].s + "-" + brick["version"].s;
    const msg  = name + " :  expecting " + prettyVersionRange(low, hi);
    failedChapelVersion.push_back(msg);
  }
}

/* Responsible for creating the dependency tree
   from the Mason.toml. Starts at the root of the
   project and continues down dep tree recursively
   until each dep is recorded */
proc createDepTree(root: Toml) {
  var dp: domain(string);
  var dps: [dp] Toml;
  var depTree: Toml = dps;
  if root.pathExists("brick") {
    depTree["root"] = new Toml(root["brick"]);
  }
  else {
    stderr.writeln("Could not find brick; Mason cannot update");
    exit(1);
  }

  if root.pathExists("dependencies") {
    var deps = getDependencies(root);
    var manifests = getManifests(deps);
    depTree = createDepTrees(depTree, manifests, "root");
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
  for brick in depTree.A {
    chplVersionError(brick);

    // Lock in the current Chapel version
    const curVer = getChapelVersionStr();
    brick["chplVersion"] = curVer + ".." + curVer;

    if brick.pathExists("dependencies") {
      for item in brick["dependencies"].arr {
        const brick = depTree[item.s];
        item.s = brick["name"].s + " " + brick["version"].s + " " + brick["source"].s;
      }
    }
  }

  return depTree;
}

proc createDepTrees(depTree: Toml, deps: [?d] Toml, name: string) : Toml {
  var depList: [1..0] Toml;
  while deps.domain.size > 0 {
    var dep = deps[deps.domain.first];

    var brick       = dep["brick"];
    var package     = brick["name"].s;
    var version     = brick["version"].s;
    var chplVersion = brick["chplVersion"].s;
    var source      = brick["source"].s;

    if depTree.pathExists(package) {
      var verToUse = IVRS(brick, depTree[package]);
      version = verToUse["version"].s;
      chplVersion = verToUse["chplVersion"].s;
    }

    depList.push_back(new Toml(package));

    if depTree.pathExists(package) == false {
      var dt: domain(string);
      var depTbl: [dt] Toml;
      depTree[package] = depTbl;
    }
    depTree[package]["name"] = package;
    depTree[package]["version"] = version;
    depTree[package]["chplVersion"] = chplVersion;
    depTree[package]["source"] = source;

    if dep.pathExists("dependencies") {
      var subDeps = getDependencies(dep);
      var manifests = getManifests(subDeps);
      var dependency = createDepTrees(depTree, manifests, package);
    }
    delete dep;
    deps.remove(deps.domain.first);
  }
  if depList.domain.size > 0 then
    depTree[name]["dependencies"] = depList;
  return depTree;
}


//
// TODO: Accept an array of bricks
//
/* The Incompatible Version Resolution Strategy
   - differing major versions are not allowed
   - Always newest minor and patch
   - in accordance with semantic versioning  */
proc IVRS(A: Toml, B: Toml) {
  const name = A["name"].s;
  const (okA, Alo, Ahi) = verifyChapelVersion(A);
  const (okB, Blo, Bhi) = verifyChapelVersion(B);
  const version1 = A["version"].s;
  const version2 = B["version"].s;
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
  var v1 = vers1(1): int;
  var v2 = vers2(1): int;
  if vers1(1) != vers2(1) {
    stderr.writeln("Dependency resolution error: package '", name, "' used by multiple packages expecting different major versions:");
    stderr.writeln("  v", version1);
    stderr.writeln("  v", version2);
    exit(1);
  }
  else if vers1(2) != vers2(2) {
    v1 = vers1(2): int;
    v2 = vers2(2): int;
    if v1 > v2 {
      return A;
    }
    else return B;
  }
  else {
    v1 = vers1(3): int;
    v2 = vers2(3): int;
    if v1 > v2 {
      return A;
    }
    else return B;
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


/* Responsible for parsing the Mason.toml to be given
   back to a call from getManifests */
proc retrieveDep(name: string, version: string) {
  const tomlPath = MASON_CACHED_REGISTRY + "/Bricks/"+name+"/"+version+".toml";
  if isFile(tomlPath) {
    var tomlFile = open(tomlPath, iomode.r);
    var depToml = parseToml(tomlFile);
    return depToml;
  }
  else {
    stderr.writeln("No toml file found in mason-registry for " + name +'-'+ version);
    exit(1);
  }
}

/* Checks if a dependency has deps; if so, the
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

/* Iterator to collect fields from a toml
   TODO custom fields returned */
iter allFields(tomlTbl: Toml) {
  for (k,v) in zip(tomlTbl.D, tomlTbl.A) {
    if v.tag == fieldToml then
      continue;
    else yield(k,v);
  }
}

