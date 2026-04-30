module Package {
import List.list;
import Map.map;
import Set.set;
import IO.{format, fileReader, ioMode};
use TOML;
import Reflection;

import MasonUtils;
import MasonUtils.MasonError;
import MasonEnv;
import MasonExternal;
import MasonSystem;
import MasonPrereqs;
import MasonLogger;

import ThirdParty.TemplateString.templateString;
import ThirdParty.Pathlib.path;
use ThirdParty.Pathlib.IOHelpers;

import TomlHelpers;

private var log = MasonLogger.getLogger("package");

class Dependency {
  var name: string;

  proc isFromRegistry() do return false;
  proc isMasonPackage() do return (this:borrowed MasonPackage?) != nil;
  proc isSystemDependency() do return (this:borrowed SystemDependency?) != nil;
  proc isExternalDependency() do
    return (this:borrowed ExternalDependency?) != nil;
  proc isSystemOrExternalDependency() do
    return isSystemDependency() || isExternalDependency();
}
// class PackageLocation { }
// class RegistryPackage: PackageLocation {
//   var version: string;
// }
// class GitPackageLocation: PackageLocation {
//   var git: string;
//   var branch: string;
//   var rev: string;
//   var revLock: string;
//   proc reference do return if rev != "" then rev else branch;
// }
// class GitPackageInfo: PackageInfo {
//   var git: string;
//   var branch: string;
//   var rev: string;
// }
// class ChapelDependency: Dependency { } 
// class GitDependency: ChapelDependency {
//   var git: string;
//   var branch: string;
//   var rev: string;
//   var revLock: string;
//   proc reference do return if rev != "" then rev else branch;
// }
class SystemDependency: Dependency {
  var version: string;
  var info: MasonSystem.systemPkgInfo;
}
class ExternalDependency: Dependency {
  var spec: string;
  // TODO: external deps don't have a standalone structure for their info
  // one should be added in the future, but I am trying to touch
  // external deps as little as possible
  var info: shared Toml?;
}
class MasonPackage: Dependency {
  var version: string;
  var chplVersion: string;
  var license: string;
  var pkgType: packageType;

  var dependencies: list(shared MasonPackage);
  var system: list(shared SystemDependency);
  var external: list(shared ExternalDependency);
  var prerequisites: list(shared PrerequisiteDependency);

  // optional
  var source: string;
  var authors: list(string);
  var copyrightYear: string;
  var compopts: list(string);
  var docopts: list(string);
  var tests: packageTests;
  var examples: packageExamples;

  override proc isFromRegistry() do return true;
}
class GitMasonPackage: MasonPackage {
  var git: string;
  var branch: string;
  var rev: string;
  var revLock: string;
  proc reference do return if rev != "" then rev else branch;

  override proc isFromRegistry() do return false;
}
record packageTests {
  var tests: list(test);
  var specifiedInManifest: bool = false;
}
record test {
  var name: path;
}
record packageExamples {
  var examples: list(example);
  var specifiedInManifest: bool = false;
}
record example {
  var name: path;
  var compopts: list(string);
  var execopts: list(string);
}
// TODO: allow prerequisites in the toml
enum prerequisiteType {
  makefile, script
}
proc type prerequisiteType.default do return this.makefile;
private proc defaultPrereqVariables() {
  var m = new map(string, string);
  m["CHPL_HOME"] = "{{CHPL_HOME}}";
  m["MASON_PACKAGE_HOME"] = "{{MASON_PACKAGE_HOME}}";
  return m;
}
class PrerequisiteDependency: Dependency {
  var prereqType: prerequisiteType;
  var buildCommand: list(string);
  var printFlagsCommand: list(string);
  var cleanCommand: list(string);
  var envVariables: map(string, string) = defaultPrereqVariables();
  proc type defaultMakefilePrereq(name: string) do
    return new shared PrerequisiteDependency(
      name, prerequisiteType.makefile,
      new list(["make"]),
      new list(["make", "--quiet", "printchplflags"]),
      new list(["make", "clean"]),
      defaultPrereqVariables());
}
enum packageType {
  application, library, light
}
proc type packageType.default do return this.application;


record buildInfo {
  var sourceFiles: list(path);
  var compopts: list(string);
}
record docInfo {
  var sourceFiles: list(path);
  var docopts: list(string);
}

proc type MasonPackage.defaultNewPkg(name: string,
                                      pkgType = packageType.default) throws {
  var b = new shared MasonPackage();
  b.name = name;
  b.version = "0.1.0";
  b.chplVersion = MasonUtils.getChapelVersionStr();
  b.license = "None";
  b.pkgType = pkgType;
  return b;
}

proc MasonPackage.toManifest(): string throws {
    const baseToml: templateString = """
    [brick]
    name="{{name}}"
    version="{{version}}"
    chplVersion="{{chplVersion}}"
    license="{{license}}"
    type="{{packageType}}"
  """.dedent().strip(trailing=false);
  var s = baseToml(["name"=>this.name,
                      "version"=>this.version,
                      "chplVersion"=>this.chplVersion,
                      "license"=>this.license,
                      "packageType"=>this.pkgType:string]);

  if source != "" then
    s += '\nsource = %"S'.format(source);
  if !authors.isEmpty() {
    s += "\nauthors = [" +
      ", ".join(for author in authors do '%"S'.format(author)) + "]";
  }
  if copyrightYear != "" then
    s += '\ncopyrightYear = %"S'.format(copyrightYear);
  if !compopts.isEmpty() {
    s += "\ncompopts = [" +
      ", ".join(for compopt in compopts do '%"S'.format(compopt)) + "]";
  }
  if !docopts.isEmpty() {
    s += "\ndocopts = [" +
      ", ".join(for docopt in docopts do '%"S'.format(docopt)) + "]";
  }
  if !tests.isEmpty() {
    s += "tests = [" +
      ", ".join(for test in tests do '%"S'.format(test.toToml())) + "]";
  }
  if !examples.isEmpty() {
    s += "examples = [" +
      ", ".join(for example in examples do '%"S'.format(example.name)) + "]";
    s += "\n\n".join(for example in examples do example.toToml());
  }

  if dependencies.isEmpty() {
    s += "\n[dependencies]\n";
  } else {
    s += "\n[dependencies]\n" +
      "\n".join(for dep in dependencies do dep.toToml());
  }

  if !system.isEmpty() {
    s += "\n\n[system]\n" +
      "\n".join(for dep in system do dep.toToml());
  }
  if !external.isEmpty() {
    s += "\n\n[external]\n" +
      "\n".join(for dep in external do dep.toToml());
  }
}


private proc readField(package: borrowed MasonPackage,
                       toml: borrowed Toml,
                       tomlField: string,
                       param fieldName: string,
                       required: bool) throws {
  if !toml.pathExists(tomlField) {
    if required then
      throw new MasonError("Manifest must contain required field: " +
                            tomlField);
    else
      return;
  }
  var parentRef = package:borrowed Dependency;
  ref field = if Reflection.hasField(package.type, fieldName)
                then Reflection.getFieldRef(package, fieldName)
                else Reflection.getFieldRef(parentRef, fieldName);
  var value = toml![tomlField]!;
  if field.type == string {
    if value.tomlType != "string" then
      throw new MasonError("'%s' must be a string".format(tomlField));
    field = value.s;
  } else if field.type == packageType {
    if value.tomlType != "string" then
      throw new MasonError("'%s' must be a string".format(tomlField));
    var s = value.s;
    try {
      field = s:packageType;
    } catch e: IllegalArgumentError {
      throw new MasonError("Invalid package type: " + s);
    } catch {
      throw new MasonError("Unexpected error parsing package type: " + s);
    }
  } else if isSubtype(field.type, list(?)) {
    field = listFromToml(field.type, tomlField, value);
  } else if Reflection.canResolveTypeMethod(field.type, "fromToml", value) {
    field = field.type.fromToml(value);
  } else {
    throw new MasonError(
      "Unsupported field type of '%s' in manifest for '%s'"
        .format(field.type:string, fieldName) +
      ", this is likely a developer error. " +
      "Please report it to the Mason developers.");
  }
}

proc MasonPackage.fillFromManifest(
  tomlStr: ?T, manifestPath: path
): MasonPackage throws
where T == string || isSubtype(T, fileReader) {
  var toml = parseToml(tomlStr);
  if !toml.pathExists("brick") then
    throw new MasonError("TOML must contain [brick] section");
  readField(this, toml, "brick.name", "name", required=true);
  readField(this, toml, "brick.version", "version", required=true);
  readField(this, toml, "brick.chplVersion", "chplVersion", required=true);
  readField(this, toml, "brick.license", "license", required=true);
  readField(this, toml, "brick.type", "pkgType", required=true);

  // read optional fields
  readField(this, toml, "brick.source", "source", required=false);
  readField(this, toml, "brick.authors", "authors", required=false);
  readField(this, toml, "brick.copyrightYear", "copyrightYear", required=false);
  readField(this, toml, "brick.compopts", "compopts", required=false);
  readField(this, toml, "brick.docopts", "docopts", required=false);

  // read tests and examples
  readField(this, toml, "brick.tests", "tests", required=false);
  readField(this, toml, "examples", "examples", required=false);
  // examples have other options too

  readField(this, toml, "dependencies", "dependencies", required=false);
  readField(this, toml, "system", "system", required=false);
  readField(this, toml, "external", "external", required=false);
  // TODO: read prerequisites

  // TODO: some things like tests, examples, and prerequisites can be inferred
  // load them here


  // eagerly load dependencies here. this is slow, but if we usually load
  // packages from a lock file (like we should anyways, doesn't matter)
  for dep in this.allDependencies() do dep.loadInfo();
  this.tests.loadInfo(manifestPath.parent);
  this.examples.loadInfo(manifestPath.parent);
  
  for prereqPath in MasonPrereqs.prereqs(manifestPath.parent) {
    var p = PrerequisiteDependency.defaultMakefilePrereq(prereqPath.name);
    this.prerequisites.pushBack(p);
  }

}

proc type MasonPackage.fromManifest(
  tomlStr: ?T, manifestPath: path
): MasonPackage throws
where T == string || isSubtype(T, fileReader) {
  var m = new shared MasonPackage();
  m.fillFromManifest(tomlStr, manifestPath);
  m.resolveDependencies();
  return m;
}


iter MasonPackage.allDependencies(): shared Dependency {
  for dep in dependencies do yield dep;
  for dep in system do yield dep;
  for dep in external do yield dep;
}
// can't use a FCP since we need to capture current and they don't
// currently support that
record checkDuplicatesFunctor {
  var current: borrowed Dependency;
  proc this(x: borrowed Dependency): bool {
    // a duplicate cannot occur between a system/external dep and
    // a MasonPackage.
    if current.isMasonPackage() && x.isSystemOrExternalDependency() then
      return false;
    else if current.isSystemOrExternalDependency() &&
            x.isMasonPackage() then
      return false;
    else
      return x.name == current.name;
  }
}
record findMasonPackageNamedFunctor {
  var name: string;
  proc this(x: borrowed MasonPackage): bool {
    return x.name == name;
  }
}
// iter MasonPackage.allDependenciesNoRecurseHelper(): borrowed Dependency {
//   for dep in dependencies do yield (dep:borrowed Dependency?)!;
//   for dep in system do yield (dep:borrowed Dependency?)!;
//   for dep in external do yield (dep:borrowed Dependency?)!;
// }
// iter MasonPackage.allDependencies(
//   recursive = false, skipDuplicateChecks = false
// ): borrowed Dependency {
//   log.debugf("Getting all dependencies for package '%s', "+
//             "recursive: %s, skipDuplicateChecks: %s",
//             name, recursive, skipDuplicateChecks);
//   // yield all direct dependencies first, then yield recursive dependencies
//   // topdown. cycles/IVRS is handled here
//   var toYield = new list(borrowed Dependency);
//   // TODO: ideally we use a queue and do BFS, but Chapel lists
//   // are not well suited to queues.
//   var stack = new list(allDependenciesNoRecurseHelper());
//   while !stack.isEmpty() {
//     var current = stack.popBack();
//     log.debugf("Processing dependency '%s'", current.name);
//     const duplicateIdx =
//       if !skipDuplicateChecks
//         then toYield.find(new checkDuplicatesFunctor(current))
//         else -1;
//     if duplicateIdx >= 0 {
//       var duplicate = toYield[duplicateIdx];
//       // if we have seen this dependency before, we need to disambiguate
//       // for system/external deps, we just warn FOR NOW
//       // for MasonPackage deps, disambiguate on IVRS
//       // for GitMasonPackage deps, print an error and just pick the first one
//       // this logic DOES NOT handle cases where 2 versions have different
//       // dependencies, the first one wins

//       log.debugf("Found a duplicate dependency on '%s'", current.name);

//       if duplicate.isSystemOrExternalDependency() &&
//           current.isSystemOrExternalDependency() {
//         log.warnf(
//           "Duplicate system/external dependencies on '%s'. This is not " +
//           "currently handled by Mason, but may cause unexpected behavior " +
//           "if the version requirements are different.", current.name);
//       }

//       if duplicate.typeID() == borrowed MasonPackage &&
//           current.typeID() == borrowed MasonPackage {
//         // TODO: IVRS
//         log.error("TODO: no IVRS yet");
//       } else {
//         // at this point, we should only have MasonPackages and at least
//         // one is not in the registry. We can't disambiguate them, error
//         log.errorf(
//           "Duplicate dependencies on package '%s' cannot currently be " +
//           "disambiguated using Incompatible Version Resolution Strategy. " +
//           "Using the first one found, this may cause unexpected behavior.",
//           current.name);
//       }
//       continue;
//     }
//     log.debugf("Adding dependency '%s' to list of dependencies to yield",
//                current.name);
//     toYield.pushBack(current);
//     if recursive && current.isMasonPackage() {
//       for dep in
//         (current:borrowed MasonPackage?)!.allDependenciesNoRecurseHelper() do
//         stack.pushBack(dep);
//     }
//   }
//   for dep in toYield do yield dep;
// }


proc MasonPackage.resolveDependencies() throws {
  // walk the dependencies and handle duplicates/version resolution

  var originalPackage = this;
  var resolvedDeps = new list(shared Dependency);
  // TODO: ideally we use a queue and do BFS, but Chapel lists
  // are not well suited to queues.
  // stack holds the parent of the dependency, if the parent is nil its the
  // originalPackage
  var stack = new list((shared MasonPackage?, shared Dependency));
  for dep in originalPackage.allDependencies() do
    stack.pushBack((nil, dep));
  while !stack.isEmpty() {
    var (currentParent, current) = stack.popBack();
    log.debugf("Processing dependency '%s'", current.name);

    // if the current dependency is the originalPackage, thats a hard error
    if current.isMasonPackage() && current.name == originalPackage.name then
      throw new MasonError("Package '%s' has a cyclic dependency on itself"
                        .format(originalPackage.name));

    const duplicateIdx = resolvedDeps.find(new checkDuplicatesFunctor(current));
    if duplicateIdx >= 0 {
      var duplicate = resolvedDeps[duplicateIdx];
      // if we have seen this dependency before, we need to disambiguate
      // for system/external deps, we just warn FOR NOW
      // for MasonPackage deps, disambiguate on IVRS
      // for GitMasonPackage deps, print an error and just pick the first one
      // this logic DOES NOT handle cases where 2 versions have different
      // dependencies, the first one wins

      log.debugf("Found a duplicate dependency on '%s'", current.name);

      if duplicate.isSystemOrExternalDependency() &&
          current.isSystemOrExternalDependency() {
        log.warnf(
          "Duplicate system/external dependencies on '%s'. This is not " +
          "currently handled by Mason and may cause unexpected behavior " +
          "if the version requirements are different.", current.name);
      } else if duplicate.isFromRegistry() && current.isFromRegistry() {
        // TODO: IVRS
        log.error("TODO: no IVRS yet");
      } else {
        // at this point, we should only have MasonPackages and at least
        // one is not in the registry. We can't disambiguate them, error
        log.errorf(
          "Duplicate dependencies on package '%s' cannot currently be " +
          "disambiguated using Incompatible Version Resolution Strategy. " +
          "Using the first one found, this may cause unexpected behavior.",
          current.name);
        // remove the duplicate from the parent
        const parentToUse =
          if currentParent != nil then currentParent! else originalPackage;
        const idxToRemove =
          parentToUse.dependencies.find(
            new findMasonPackageNamedFunctor(current.name));
        const removed = parentToUse.dependencies.getAndRemove(idxToRemove);
        log.debugf("Removed the duplicate dependency '%s' from parent '%s'",
                   removed.name, parentToUse.name);
      }
      continue;
    }
    log.debugf("Keeping the resolved dependency '%s'", current.name);
    resolvedDeps.pushBack(current);
    if current.isMasonPackage() {
      const newParent = (current:MasonPackage);
      for dep in newParent.allDependencies() do
        stack.pushBack((newParent, dep));
    }
  }
}


proc MasonPackage.IVRS(
  other: borrowed MasonPackage
): borrowed MasonPackage throws {
  
}

proc MasonPackage.createLockFile(): shared Toml throws {
  log.debug("Creating lock file for package: ", name);
  var root = new shared Toml();
  root.set("root", new shared Toml(new map(string, shared Toml?)));
  root.set("root.name", this.name);
  root.set("root.version", this.version);
  root.set("root.chplVersion", this.chplVersion);
  root.set("root.license", this.license);
  root.set("root.type", this.pkgType:string);
  if this.source != "" then
    root.set("root.source", this.source);
  if !this.authors.isEmpty() {
    const authorsList = [a in this.authors] new shared Toml?(a);
    root.set("root.authors", authorsList);
  }
  if this.copyrightYear != "" then
    root.set("root.copyrightYear", this.copyrightYear);
  if !this.compopts.isEmpty() {
    const compoptsList = [c in this.compopts] new shared Toml?(c);
    root.set("root.compopts", compoptsList);
  }
  if !this.docopts.isEmpty() {
    const docoptsList = [d in this.docopts] new shared Toml?(d);
    root.set("root.docopts", docoptsList);
  }
  if !this.tests.isEmpty() {
    const testNames = [t in this.tests] new shared Toml?(t.name:string);
    root.set("root.tests", testNames);
  }
  // TODO why are examples not included? Just an oversight?


  // TODO: just dump the full structure

  // // packagesToProcess exists solely to keep track of new shared MasonPackage instances
  // // for lifetime reasons
  // var packagesToProcess = new list(owned MasonPackage?);
  // var depsToProcess = new list(this.allDependencies());
  // while !depsToProcess.isEmpty() {
  //   var dep = depsToProcess.getAndRemove(0);
  //   log.debug("Processing dependency: ", dep.name);
  //   // for MasonPackages, also process their dependencies
  //   if dep:MasonPackage? {
  //     var tomlPath = (dep:MasonPackage).getDepManifestPath();
  //     packagesToProcess.pushBack(
  //       MasonPackage.fromManifest(openReader(tomlPath)));
  //     depsToProcess.pushBack(packagesToProcess.last!.allDependencies());

  //     // use the newly gotten packagesToProcess to add the dep, since
  //     // that has all the info
  //     packagesToProcess.last!.addDepToLock(root);
  //   } else {
  //     dep.addDepToLock(root);
  //   }
  // }

  return root;

}

proc Dependency.toToml(): string throws do return this.name;
proc type Dependency.fromToml(
  name: string, toml: borrowed Toml
): shared Dependency throws {
  return new shared Dependency(name);
}
// proc Dependency.addDepToLock(root: shared Toml) throws { }
proc Dependency.loadInfo() throws {
  log.debug("Generic dependency has no info to load");
}



override proc MasonPackage.toToml(): string throws {
  return '%s = %"S'.format(name, version);
}
// BUG: see https://github.com/chapel-lang/chapel/issues/28722
override proc type MasonPackage.fromToml(
  name: string, toml: borrowed Toml
): shared MasonPackage throws {
  if toml.tomlType != "string" then
    throw new MasonError("Mason dependencies must be specified as a " +
                          "single version string");
  var m = new shared MasonPackage(name, toml.s);
  // TODO: check that the version is correct format
  return m;
}
override proc MasonPackage.loadInfo() throws {
  const manifestPath = this.getDepManifestPath();
  log.debugf("Loading manifest for dependency '%s' from path '%?'",
             this.name, manifestPath);
  this.fillFromManifest(openReader(manifestPath), manifestPath);
}
// override proc MasonPackage.addDepToLock(root: shared Toml) throws {

//   var version = this.version;
//   var chplVersion = this.chplVersion;

//   if root.pathExists(this.name) {
//     // todo, resolve version conflicts using IVRS
//     log.errorf("unhandled multiple dependencies on package '%s'", this.name);
//   }

//   var newToml = new shared Toml(new map(string, shared Toml?));
//   newToml.set("name", this.name);
//   newToml.set("version", version);
//   newToml.set("chplVersion", chplVersion);
//   newToml.set("source", this.source);
//   if !this.compopts.isEmpty() {
//     const compoptsList = [c in this.compopts] new shared Toml?(c);
//     newToml.set("compopts", compoptsList);
//   }
//   if !this.docopts.isEmpty() {
//     const docoptsList = [d in this.docopts] new shared Toml?(d);
//     newToml.set("docopts", docoptsList);
//   }
//   root.set(this.name, newToml);

// }

override proc GitMasonPackage.toToml(): string throws {
  var s = '%s = { git = %"S'.format(name, git);
  if branch != "" then s += ', branch = %"S'.format(branch);
  if rev != "" then s += ', rev = %"S'.format(rev);
  s += " }";
  return s;
}
// BUG: see https://github.com/chapel-lang/chapel/issues/28722
override proc type GitMasonPackage.fromToml(
  name: string, toml: borrowed Toml
): shared GitMasonPackage throws {
  if toml.tomlType != "toml" then
    throw new MasonError("Git dependencies must be specified as a table");
  var g = new shared GitMasonPackage();
  g.name = name;
  (g.git, g.branch, g.rev) =
    TomlHelpers.readStringsFromTable(toml, ("git", "branch", "rev"),
                                     extraFieldsError=true,
                                     missingFieldsError=false);
  if g.git == "" then
    throw new MasonError("Git dependencies must specify a 'git' field");
  if g.branch != "" && g.rev != "" then
    throw new MasonError("Git dependencies cannot specify both " +
                         "'branch' and 'rev'");
  // default to using the latest commit if neither is specified
  if g.branch == "" && g.rev == "" then
    g.rev = "HEAD";
  return g;
}

override proc SystemDependency.toToml(): string throws {
  return '%s = %"S'.format(name, version);
}
proc type SystemDependency.fromToml(
  name: string, toml: borrowed Toml
): shared SystemDependency throws {
  if toml.tomlType != "string" then
    throw new MasonError("System dependencies must be specified as a string");
  var s = new shared SystemDependency(name, toml.s);
  // TODO: check that the version is correct format
  return s;
}
override proc SystemDependency.loadInfo() throws {
  this.info = MasonSystem.getPCDep(name, version);
}
// override proc SystemDependency.addDepToLock(root: shared Toml) throws {
//   if !root.pathExists("system") then
//     root.set("system", new shared Toml(new map(string, shared Toml?)));
//   if root.pathExists("system." + this.name) {
//     // we already processed a dependency of this name. this was unhandled
//     // in the old implementation and could occur if there are multiple
//     // dependencies on the same system package with different version
//     // requirements.
//     // For now, show a warning and use the first one we processed
//     log.warnf("Multiple dependencies on system package '%s' with " +
//               "potentially different version requirements. This is " +
//               "not currently handled by Mason", this.name);
//   } else {
//     const (name, toml) =
//       MasonSystem.getPCDep(this.name, this.version).toToml();
//     root.set("system." + name, toml);
//   }
// }

override proc ExternalDependency.toToml(): string throws {
  return '%s = %"S'.format(name, spec);
}
proc type ExternalDependency.fromToml(
  name: string, toml: borrowed Toml
): shared ExternalDependency throws {
  if toml.tomlType != "string" then
    throw new MasonError("External dependencies must be " +
                          "specified as a string");
  var s = new shared ExternalDependency(name, toml.s);
  // TODO: check that the spec is correct format
  return s;
}
override proc ExternalDependency.loadInfo() throws {
  this.info = MasonExternal.getExternalPackage(this.name, this.spec)[1];
}
// override proc ExternalDependency.addDepToLock(root: shared Toml) throws {
//   if !root.pathExists("external") then
//     root.set("external", new shared Toml(new map(string, shared Toml?)));
//   if root.pathExists("external." + this.name) {
//     log.warnf("Multiple dependencies on external package '%s'. This is " +
//               "not currently handled by Mason", this.name);
//   } else {
//     const (name, toml) =
//       MasonExternal.getExternalPackage(this.name, this.spec);
//     root.set("external." + name, toml);
//   }
// }


proc packageTests.toToml(): string throws {
  const s: templateString = """
  tests = [{{tests}}]
  """.dedent();
  var testStr = ", ".join([t in this.tests] '%"S'.format(t.toToml()));
  return s(["tests" => testStr]);
}
proc type packageTests.fromToml(toml: borrowed Toml): packageTests throws {
  if toml.tomlType != "array" then
    throw new MasonError("Tests must be specified as an array of strings");

  var result = new packageTests();
  result.specifiedInManifest = true;
  result.tests = new list([x in toml.arr] test.fromToml(x!));
  return result;
}
proc test.toToml(): string throws {
  return name:string;
}
proc type test.fromToml(toml: borrowed Toml): test throws {
  if toml.tomlType != "string" then
    throw new MasonError("Tests must be specified as an array of strings");
  const name: path = toml.s.strip();
  if name.suffix != ".chpl" then
    throw new MasonError("Tests must be Chapel files");
  return new test(name);
}
proc packageExamples.toToml(): string throws {
  const s: templateString = """
  [examples]
  examples = [{{examples}}]
  {{examplesTables}}
  """.dedent();
  const exampleStr = ", ".join([e in this.examples] '%"S'.format(e.name));
  const examplesTables = [e in this.examples] e.toToml();
  const examplesTablesStr =
    "\n\n".join([e in examplesTables] if e != "" then e);
  return s(["examples" => exampleStr, "examplesTables" => examplesTablesStr]);
}
proc type packageExamples.fromToml(
  toml: borrowed Toml
): packageExamples throws {
  if toml!.tomlType != "toml" then
    throw new MasonError("Examples must be specified as a table");

  var result = new packageExamples();
  result.specifiedInManifest = true;
  // if const examplesList = toml!["examples"] {
  //   if examplesList.tomlType != "array" then
  //   throw new MasonError("The 'examples' field must be an array of strings");
  //   for x in examplesList.arr do
  //     result.examples.pushBack(example.fromToml(x!));
  // }
  // var result: resultType;
  // // use IO;
  // // value.writeJSON(stdout);
  // if const examplesList = value["examples"] {
  //   if examplesList.tomlType != "array" then
  //     throw new MasonError("'%s' must be a list of examples"
  //                           .format(tomlField+".examples"));
  //   for exampleToml in examplesList.arr {
  //     var ex = example.fromToml(exampleToml!);
  //     var compopts: list(string);
  //     var execopts: list(string);
  //     if value.pathExists(ex.name.stem) {
  //       // TODO parse compopts
  //     }
  //   }
  //   // TODO check that there is no `[examples.UNKNOWN]`
  // }
  // if value.tomlType != "array" then
  //   throw new MasonError("'%s' must be an array of strings"
  //                         .format(tomlField));
  // for x in value.arr do
  //   result.pushBack(example.fromToml(x!));

  return result;
}
proc example.toToml(): string throws {
  if compopts.isEmpty() && execopts.isEmpty() then
    return "";
  var s = "[examples.%s]".format(name:string);
  if !compopts.isEmpty() {
    s += "\ncompopts = [" +
      ", ".join(for c in compopts do '%"S'.format(c)) + "]";
  }
  if !execopts.isEmpty() {
    s += "\nexecopts = [" +
      ", ".join(for e in execopts do '%"S'.format(e)) + "]";
  }
  return s;
}

proc type example.fromToml(toml: borrowed Toml): example throws {
  if toml.tomlType != "string" then
    throw new MasonError("Examples must be a string");
  const name: path = toml.s.strip();
  if name.suffix != ".chpl" then
    throw new MasonError("Example names must be Chapel files");
  return new example(name, new list(string), new list(string));
}



proc ref packageTests.loadInfo(projectSourceDir: path) throws {
  import Path; // TODO: if Pathlib supports relPath we can remove this
  if this.specifiedInManifest {
    log.debug("Tests were explicitly specified in the manifest, ",
              "no need to infer them");
    return;
  }
  const testPath = projectSourceDir / "test";
  if !testPath.isDir() {
    log.debug("No 'test' directory found '%s', no tests to load"
                .format(testPath:string));
    return;
  }
  for t in testPath.findFiles(recursive=true, hidden=false) do
    if t.suffix == ".chpl" then
      this.tests.pushBack(
        new test(Path.relPath(t:string, testPath:string):path));
}
proc ref packageExamples.loadInfo(projectSourceDir: path) throws {
  import Path; // TODO: if Pathlib supports relPath we can remove this
  if this.specifiedInManifest {
    log.debug("Examples were explicitly specified in the manifest, ",
              "no need to infer them");
    return;
  }
  const examplePath = projectSourceDir / "example";
  if !examplePath.isDir() {
    log.debug("No 'example' directory found '%s', no examples to load"
                .format(examplePath:string));
    return;
  }
  for e in examplePath.findFiles(recursive=true, hidden=false) do
    if e.suffix == ".chpl" then
      this.examples.pushBack(
        new example(Path.relPath(e:string, examplePath:string):path));
}


proc MasonPackage.getDepManifestPath(): path throws {
  for cached in MasonEnv.MASON_CACHED_REGISTRY {
    const tomlPath =
      cached:path / "Bricks" / this.name / (this.version + ".toml");
    if tomlPath.isFile() then return tomlPath;
  }
  throw new MasonError("No toml file found in mason-registry for " +
                       this.name + "@" + this.version);
}
override proc GitMasonPackage.getDepManifestPath(): path throws {
  const base = MasonEnv.MASON_HOME:path / "git";
  const slug = "%s-%s".format(this.name, this.reference);
  const gitDir = base / slug;
  if !gitDir.isDir() {
    // we have not yet cloned this dependency, so do that now
    this.cloneSource();
  }
  const tomlPath = gitDir / "Mason.toml";
  if tomlPath.isFile() then return tomlPath;
  throw new MasonError("No toml file found in git dependency for " +
                        this.name + "@" + this.reference);
}


proc GitMasonPackage.cloneSource() throws {
  const base = MasonEnv.MASON_HOME:path / "git";
  const slug = "%s-%s".format(name, reference);
  const gitDir = base / slug;
  if !gitDir.isDir() {
    writeln("Downloading dependency: ", slug);
    log.debugf("Cloning git dependency from %s into %?", git, gitDir);
    MasonUtils.cloneSource(git, gitDir, quiet=true);

    if reference != "HEAD" {
      log.debugf("Checking out reference '%s' for git dependency %s",
                reference, name);
      MasonUtils.checkoutSource(gitDir, reference, quiet=true);
    }
  } else {
    log.debug("Git dependency already cloned: ", slug);
    if branch != "" || reference == "HEAD" {
      writeln("Fetching latest changes for: ", slug);
      MasonUtils.gitC(gitDir, ["git", "fetch", "-q", "--all"], quiet=true);
      const remoteBranch = "origin/" + reference;
      MasonUtils.gitC(gitDir,
                      ["git", "reset", "-q", "--hard", remoteBranch],
                      quiet=true);
    }
  }
  revLock = MasonUtils.gitC(gitDir,
                            ["git", "rev-parse", "HEAD"], quiet=true).strip();
  log.debugf("Locked git dependency %s to rev %s", name, revLock);
}

private proc listFromToml(
  type resultType,
  tomlField: string,
  value: borrowed Toml
): resultType throws
where isSubtype(resultType, list(string)) {
  var result: resultType;
  if value.tomlType == "array" {
    for x in value.arr {
      if x!.tomlType != "string" then
        throw new MasonError("All elements of '%s' must be strings"
                              .format(tomlField));
      result.pushBack(x!.s);
    }
  } else if value.tomlType == "string" {
    for s in value.s.split() do
      if s.strip() != "" then
        result.pushBack(s.strip());
  } else {
    throw new MasonError("'%s' must be a string or array of strings"
                          .format(tomlField));
  }
  return result;
}

private proc listFromToml(
  type resultType,
  tomlField: string,
  value: borrowed Toml
): resultType throws
where isSubtype(resultType, list(shared MasonPackage)) {
  var result: resultType;
  if value.tomlType == "toml" {
    for k in value.keys() {
      const val = value[k]!;
      if val.tomlType == "string" then
        result.pushBack(MasonPackage.fromToml(k, val));
      else if val.tomlType == "toml" then
        result.pushBack(GitMasonPackage.fromToml(k, val));
      else
        throw new MasonError("Dependency '%s' has an unknown format".format(k));
    }
  } else {
    throw new MasonError("'%s' must be a table"
                          .format(tomlField));
  }
  return result;
}

private proc listFromToml(
  type resultType,
  tomlField: string,
  value: borrowed Toml
): resultType throws
where isSubtype(resultType, list(shared SystemDependency)) ||
      isSubtype(resultType, list(shared ExternalDependency)) {
  var result: resultType;
  if value.tomlType == "toml" {
    for k in value.keys() {
      const val = value[k]!;
      result.pushBack(resultType.eltType.fromToml(k, val));
    }
  } else {
    throw new MasonError("'%s' must be a table"
                          .format(tomlField));
  }
  return result;
}

@chplcheck.ignore("UnusedFormal")
private proc listFromToml(
  type resultType,
  tomlField: string,
  value: borrowed Toml
): resultType throws {
  compilerError("Unhandled dependency type in manifest: " +
                resultType:string);
}
}
