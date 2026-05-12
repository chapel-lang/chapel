module Package {
import List.list;
import Map.map;
import Set.set;
import IO.{format, fileReader, ioMode};
use TOML;
import Reflection;
import Version;
import MasonUtils.{isCompatible, zero, max, containsMax, fromString};


import MasonUtils;
import MasonUtils.MasonError;
import MasonUpdate;
import MasonEnv;
import MasonExternal;
import MasonSystem;
import MasonPrereqs;
import MasonLogger;

import ThirdParty.TemplateString.templateString;
import ThirdParty.Pathlib.path;
use ThirdParty.Pathlib.IOHelpers;

import TomlHelpers;
import this.VersionResolution.{IVRS, ivrsResult};

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


  proc toToml(): string throws do return this.name;
  @chplcheck.ignore("UnusedFormal")
  proc type fromToml(name: string,
                     toml: borrowed Toml): shared Dependency throws do
    return new shared Dependency(name);
  @chplcheck.ignore("UnusedFormal")
  proc loadInfo(postResolve: bool = false) throws do
    log.debug("Nothing to load for dependency ", name);
  @chplcheck.ignore("UnusedFormal")
  proc addDepToLock(lock: shared Toml) throws {
    log.debug("No info to add to lock file for dependency ", name);
  }

  proc preBuild() throws do
    log.debug("No pre-build steps for dependency ", name);
  proc postBuild() throws do
    log.debug("No post-build steps for dependency ", name);

  // this should be 'iter Dependency.getSourceFiles(): string throws'
  // (and 'getCompopts' should be the same)
  // but error handling and virtual iterators don't play well together.
  // the workaround is
  // 'iter Dependency.getSourceFiles(ref err: owned Error?): string'
  // but that also doesn't work with deeply nested virtual iterators
  // (see https://github.com/chapel-lang/chapel/issues/28762)
  // at this point, I had had enough of workarounds for iterators so these are
  // functions just return lists
  proc getSourceFiles(): list(string) throws {
    log.debug("No source files for dependency ", name);
    return new list(string);
  }
  proc getCompopts(): list(string) throws {
    log.debug("No compopts for dependency ", name);
    return new list(string);
  }
}
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
  var version: Version.version;
  var chplVersion: versionRange;
  var license: string;
  var pkgType: packageType;

  var dependencies: list(shared MasonPackage);
  var system: list(shared SystemDependency);
  var external: list(shared ExternalDependency);

  // optional
  var source: string;
  var authors: list(string);
  var copyrightYear: string;
  var compopts: list(string);
  var docopts: list(string);
  var tests: packageTests;
  var examples: packageExamples;

  // not in TOML
  var prerequisites: list(shared PrerequisiteDependency);
  var projectHome: path;

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
  proc toToml(): string throws {
    const s: templateString = """
    tests = [{{tests}}]
    """.dedent();
    var testStr = ", ".join([t in this.tests] '%"S'.format(t.toToml()));
    return s(["tests" => testStr]);
  }
  proc type fromToml(toml: borrowed Toml): packageTests throws {
    if toml.tomlType != "array" then
      throw new MasonError("Tests must be specified as an array of strings");

    var result = new packageTests();
    result.specifiedInManifest = true;
    result.tests = new list([x in toml.arr] test.fromToml(x!));
    return result;
  }
  proc ref loadInfo(projectSourceDir: path) throws {
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
        this.add(new test(Path.relPath(t:string, testPath:string):path));
  }

  proc isEmpty(): bool do return this.tests.isEmpty();
  proc ref add(t: test) do this.tests.pushBack(t);
  proc this(i: int) do return this.tests[i];
  iter these() do for t in this.tests do yield t;
  proc find(name: string): int {
    record finder {
      var name: string;
      proc this(t: test) do return t.name.name == name;
    }
    return this.tests.find(new finder(name));
  }
  proc get(name: string): test do return this.tests[this.find(name)];
}
record test {
  var name: path;
  proc toToml(): string throws do return name:string;
  proc type fromToml(toml: borrowed Toml): test throws {
    if toml.tomlType != "string" then
      throw new MasonError("Tests must be specified as an array of strings");
    const name: path = toml.s.strip();
    if name.suffix != ".chpl" then
      throw new MasonError("Tests must be Chapel files");
    return new test(name);
  }
}
record packageExamples {
  var examples: list(example);
  var specifiedInManifest: bool = false;

  proc toToml(): string throws {
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
  proc type fromToml(toml: borrowed Toml): packageExamples throws {
    if toml.tomlType != "toml" then
      throw new MasonError("Examples must be specified as a table");
    if !toml.pathExists("examples") then
      throw new MasonError("Missing list of examples under [examples] section");
    if toml["examples"]!.tomlType != "array" then
      throw new MasonError("Examples must be specified as an array of strings");

    var result = new packageExamples();
    result.specifiedInManifest = true;

    result.examples = new list([x in toml["examples"]!.arr] example.fromToml(x!));
    log.debug("Parsed examples from manifest: ", result.examples);
    for k in toml.keys() {
      if k == "examples" then continue;
      const idx = result.findByName(k);
      if idx == -1 then
        throw new MasonError("Unknown example '%s' specified in manifest".format(k));
      result.examples[idx].getOptionsFromToml(toml[k]!);
    }
    log.debug("Parsed example options from manifest: ", result.examples);

    return result;
  }
  proc toLock(): shared Toml throws {
    var newToml = new shared Toml(new map(string, shared Toml?));
    newToml.set("examples", TomlHelpers.iterableToTomlArray(this.examples,
        proc(e:example): string { return e.name:string;}));
    for e in this.examples {
      newToml.set(e.name.stem,
                new shared Toml(new map(string, shared Toml?)));
      newToml.set(e.name.stem + ".compopts",
              TomlHelpers.iterableToTomlArray(e.compopts));
      newToml.set(e.name.stem + ".execopts",
              TomlHelpers.iterableToTomlArray(e.execopts));
    }
    return newToml;
  }

  proc ref loadInfo(projectSourceDir: path) throws {
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

  proc isEmpty(): bool do return this.examples.isEmpty();
  proc ref add(e: example) do this.examples.pushBack(e);
  proc this(i: int) do return this.examples[i];
  iter these() do for e in this.examples do yield e;
  proc find(name: string): int {
    record finder {
      var name: string;
      proc this(e: example) do return e.name.name == name;
    }
    return this.examples.find(new finder(name));
  }
  proc findByName(name: string): int {
    record finder {
      var name: string;
      proc this(e: example) do return e.name.stem == name;
    }
    return this.examples.find(new finder(name));
  }
  proc get(name: string): example do return this.examples[this.find(name)];

}
record example {
  var name: path;
  var compopts: list(string);
  var execopts: list(string);

  proc toToml(): string throws {
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
  proc type fromToml(toml: borrowed Toml): example throws {
    if toml.tomlType != "string" then
      throw new MasonError("Examples must be a string");
    const name: path = toml.s.strip();
    if name.suffix != ".chpl" then
      throw new MasonError("Example names must be Chapel files");
    return new example(name, new list(string), new list(string));
  }

  proc ref getOptionsFromToml(toml: borrowed Toml) throws {
    if toml.tomlType != "toml" then
      throw new MasonError("Example options must be a table");
    for k in toml.keys() {
      select k {
        when "compopts" do
          this.compopts = listFromToml(list(string), k, toml[k]!);
        when "execopts" do
          this.execopts = listFromToml(list(string), k, toml[k]!);
        otherwise
          throw new MasonError("Unexpected field '%s' in example options"
                                .format(k));
      }
    }
  }
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
  var location: path;
  var prereqType: prerequisiteType;
  var buildCommand: list(string);
  var printFlagsCommand: list(string);
  var cleanCommand: list(string);
  var envVariables: map(string, string) = defaultPrereqVariables();
  proc type defaultMakefilePrereq(location: path) do
    return new shared PrerequisiteDependency(
      location.name, location, prerequisiteType.makefile,
      new list(["make"]),
      new list(["make", "--quiet", "printchplflags"]),
      new list(["make", "clean"]),
      defaultPrereqVariables());
}
enum packageType {
  application, library, light
}
proc type packageType.default do return this.application;

proc type MasonPackage.defaultNewPkg(name: string,
                                      pkgType = packageType.default) throws {
  var b = new shared MasonPackage();
  b.name = name;
  b.version = new Version.version(0, 1, 0);
  b.chplVersion = new versionRange(MasonUtils.getChapelVersionInfo());
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
                      "version"=>this.version:string,
                      "chplVersion"=>this.chplVersion:string,
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

private proc readField(ref field,
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
  } else if isSubtype(field.type, Version.version) {
    if value.tomlType != "string" then
      throw new MasonError("'%s' must be a string".format(tomlField));
    field = MasonUtils.parseChplVersionString(value.s);
  } else if isSubtype(field.type, versionRange) {
    if value.tomlType != "string" then
      throw new MasonError("'%s' must be a string".format(tomlField));
    const (low, high) = MasonUtils.checkChplVersion(value.s);
    field = new versionRange(low, high);
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

private proc readField(package: borrowed MasonPackage,
                       toml: borrowed Toml,
                       tomlField: string,
                       param fieldName: string,
                       required: bool) throws {
  var parentRef = package:borrowed Dependency;
  ref field = if Reflection.hasField(borrowed MasonPackage, fieldName)
                then Reflection.getFieldRef(package, fieldName)
                else Reflection.getFieldRef(parentRef, fieldName);
  readField(field, toml, tomlField, fieldName, required);
}
private proc readField(package: borrowed GitMasonPackage,
                       toml: borrowed Toml,
                       tomlField: string,
                       param fieldName: string,
                       required: bool) throws {
  var masonRef = package:borrowed MasonPackage;
  var parentRef = package:borrowed Dependency;
  ref field = if Reflection.hasField(borrowed GitMasonPackage, fieldName)
                then Reflection.getFieldRef(package, fieldName)
                else if Reflection.hasField(borrowed MasonPackage, fieldName)
                      then Reflection.getFieldRef(masonRef, fieldName)
                      else Reflection.getFieldRef(parentRef, fieldName);
  readField(field, toml, tomlField, fieldName, required);
}
private proc readField(package: borrowed SystemDependency,
                       toml: borrowed Toml,
                       tomlField: string,
                       param fieldName: string,
                       required: bool) throws {
  var parentRef = package:borrowed Dependency;
  ref field = if Reflection.hasField(borrowed SystemDependency, fieldName)
                then Reflection.getFieldRef(package, fieldName)
                else Reflection.getFieldRef(parentRef, fieldName);
  readField(field, toml, tomlField, fieldName, required);
}

proc MasonPackage.fillFromManifest(
  toml: borrowed Toml, postResolve=false
): MasonPackage throws {
  if !toml.pathExists("brick") {
    log.debug("Toml content: ", toml);
    throw new MasonError("TOML must contain [brick] section");
  }
  if !postResolve {
    readField(this, toml, "brick.name", "name", required=true);
    readField(this, toml, "brick.version", "version", required=true);
    readField(this, toml, "brick.chplVersion", "chplVersion", required=true);
    readField(this, toml, "brick.license", "license", required=true);
    readField(this, toml, "brick.type", "pkgType", required=true);
  }

  // read optional fields
  if !postResolve {
    readField(this, toml, "brick.source", "source", required=false);
    readField(this, toml, "brick.authors", "authors", required=false);
    readField(this, toml, "brick.copyrightYear", "copyrightYear", required=false);
  } else {
    readField(this, toml, "brick.compopts", "compopts", required=false);
    readField(this, toml, "brick.docopts", "docopts", required=false);
  }

  // read tests and examples
  if postResolve {
    readField(this, toml, "brick.tests", "tests", required=false);
    readField(this, toml, "examples", "examples", required=false);
  }

  if !postResolve {
    readField(this, toml, "dependencies", "dependencies", required=false);
    readField(this, toml, "system", "system", required=false);
    readField(this, toml, "external", "external", required=false);
  }

  // eagerly load dependencies here. this is slow, but if we usually load
  // packages from a lock file (like we should anyways, doesn't matter)
  for dep in this.allDependencies() do dep.loadInfo(postResolve=postResolve);

  // infer tests/examples if not specified in manifest
  if postResolve {
    this.tests.loadInfo(this.getPackageHome());
    this.examples.loadInfo(this.getPackageHome());
  }

  // infer prereqs if not specified in manifest.
  if postResolve {
    for prereqPath in MasonPrereqs.prereqs(this.getPackageHome()) {
      var p = PrerequisiteDependency.defaultMakefilePrereq(prereqPath);
      this.prerequisites.pushBack(p);
    }
  }

}

proc type MasonPackage.fromManifest(
  tomlStr: ?T, projectHome: path
): MasonPackage throws
where T == string || isSubtype(T, fileReader) {
  if !projectHome.isAbsolute() then
    throw new MasonError("projectHome must be an absolute path");

  var m = new shared MasonPackage();
  m.projectHome = projectHome;
  log.debug("Creating package from manifest at ", projectHome);
  var toml = parseToml(tomlStr);
  log.debug("Manifest Contents", toml);
  m.fillFromManifest(toml, postResolve=false);
  log.debug("Filled in initial fields from manifest, ",
            "now resolving dependencies");
  m.resolveDependencies();
  log.debug("Resolved all the dependencies, ",
            "now filling in post-resolve fields");
  m.fillFromManifest(toml, postResolve=true);
  return m;
}
proc type MasonPackage.fromLock(
  tomlStr: ?T,
  projectHome: path
): MasonPackage throws
where T == string || isSubtype(T, fileReader) {
  if !projectHome.isAbsolute() then
    throw new MasonError("projectHome must be an absolute path");

  var p = new shared MasonPackage();
  p.projectHome = projectHome;

  var toml = parseToml(tomlStr);
  log.debug("Creating package from lock file at ", projectHome);
  log.debug("Lock file contents: ", toml);

  if !toml.pathExists("lockfileVersion") || toml["lockfileVersion"]!.tomlType != "string" then
    throw new MasonError(
      "Unknown lockfile version. Please delete your lock file " +
      " and run 'mason update' to generate a new one.");
  const lockFileVersion = toml["lockfileVersion"]!.s;
  if lockFileVersion != MasonUtils.MASON_LOCKFILE_VERSION then
    throw new MasonError(
      "Lockfile version mismatch. Expected lockfile version '" +
      MasonUtils.MASON_LOCKFILE_VERSION + "', but found '" + lockFileVersion +
      "'. Please delete your lock file and run 'mason update' " +
      "to generate a new one.");

  var deps = new list(shared MasonPackage);
  var systemDeps = new list(shared SystemDependency);
  var externalDeps = new list(shared ExternalDependency);

  // read root only fields
  log.debug("Reading root-level fields from lock file");
  readField(p, toml, "root.license", "license", required=true);
  readField(p, toml, "root.type", "pkgType", required=true);
  readField(p, toml, "root.authors", "authors", required=true);
  readField(p, toml, "root.copyrightYear", "copyrightYear", required=true);
  readField(p, toml, "root.tests", "tests", required=true);
  readField(p, toml, "root.examples", "examples", required=true);
  // fill in the remaining fields
  log.debug("Reading remaining fields from lock file");
  p.fillFromLock(toml, toml["root"]!, deps, systemDeps, externalDeps);

  return p;
}
proc MasonPackage.fillFromLock(
  lock: borrowed Toml,
  toml: borrowed Toml,
  ref dependencyList: list(shared MasonPackage),
  ref systemList: list(shared SystemDependency),
  ref externalList: list(shared ExternalDependency)
) throws {
  log.debug("Filling in package fields from lock file");
  readField(this, toml, "name", "name", required=true);
  readField(this, toml, "version", "version", required=true);
  readField(this, toml, "chplVersion", "chplVersion", required=true);
  readField(this, toml, "source", "source", required=true);
  readField(this, toml, "compopts", "compopts", required=true);
  readField(this, toml, "docopts", "docopts", required=true);

  log.debug("Adding dependencies from lock file to package");
  if const deps = lock.get("dependencies") {
    var depNames = listFromToml(list(string), "dependencies", deps!);
    for depName in depNames {
      const existingIdx =
        dependencyList.find(new findDependencyNamedFunctor(depName));
      if existingIdx != -1 {
        log.debug(("Dependency '%s' already read in, " +
                   "reusing existing dependency").format(depName));
        const existingDep = dependencyList[existingIdx];
        this.dependencies.pushBack(existingDep);
      } else {
        log.debug("Reading dependency '%s' from lock file".format(depName));
        if !lock.pathExists(depName) {
          throw new MasonError(
            ("Lock file is missing information for dependency '%s'. " +
            "Please delete your lock file and run 'mason update' to " +
            "generate a new one.").format(depName));
        }
        // if depToml has a 'git' field, it's a GitMasonPackage,
        // otherwise it's a MasonPackage
        const depToml = lock[depName]!;
        var newDep: shared MasonPackage;
        if depToml.pathExists("git") {
          newDep = new shared GitMasonPackage();
        } else {
          newDep = new shared MasonPackage();
        }
        newDep.fillFromLock(lock, depToml, dependencyList,
                              systemList, externalList);
        dependencyList.pushBack(newDep);
        this.dependencies.pushBack(newDep);
      }
    }
  }

  log.debug("Adding system dependencies from lock file to package");
  if const systemDeps = lock.get("system") {
    var sysDepNames = listFromToml(list(string), "system", systemDeps!);
    for depName in sysDepNames {
      const existingIdx =
        systemList.find(new findDependencyNamedFunctor(depName));
      if existingIdx != -1 {
        log.debug(("System dependency '%s' already read in, " +
                   "reusing existing dependency").format(depName));
        const existingDep = systemList[existingIdx];
        this.system.pushBack(existingDep);
      } else {
        log.debug(("Reading system dependency '%s' " +
                   "from lock file").format(depName));
        if !lock.pathExists(depName) {
          throw new MasonError(
            ("Lock file is missing information for system dependency '%s'. " +
            "Please delete your lock file and run 'mason update' to " +
            "generate a new one.").format(depName));
        }
        const depToml = lock[depName]!;
        var newDep = new shared SystemDependency();
        newDep.fromLock(depToml);
        systemList.pushBack(newDep);
        this.system.pushBack(newDep);
      }
    }
  }

  // TODO: external
  // TODO: prereqs

}

override proc GitMasonPackage.fillFromLock(
  lock: borrowed Toml,
  toml: borrowed Toml,
  ref dependencyList: list(shared MasonPackage),
  ref systemList: list(shared SystemDependency),
  ref externalList: list(shared ExternalDependency)
) throws {
  super.fillFromLock(lock, toml, dependencyList, systemList, externalList);
  readField(this, toml, "git", "git", required=true);
  readField(this, toml, "branch", "branch", required=true);
  readField(this, toml, "rev", "rev", required=true);
  readField(this, toml, "revLock", "revLock", required=true);
}

proc SystemDependency.fromLock(toml: borrowed Toml) throws {
  readField(this, toml, "name", "name", required=true);
  readField(this, toml, "version", "version", required=true);
  this.info.name = this.name;
  this.info.version = this.version;
  readField(this.info.libs, toml, "libs", "libs", required=true);
  readField(this.info.includes, toml, "includes", "includes", required=true);
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
record findDependencyNamedFunctor {
  var name: string;
  proc this(x: borrowed Dependency): bool do return x.name == name;
}
record findMasonPackageNameVersionFunctor {
  var name: string;
  var version: Version.version;
  proc this(x: borrowed MasonPackage): bool do
    return x.name == name && x.version == version;
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
        var A = duplicate:borrowed MasonPackage;
        var B = current:borrowed MasonPackage;
        log.debugf("Resolving duplicate MasonPackage dependencies on '%s' " +
                   "between '%?' and '%?' with IVRS",
                   A.name, A.version, B.version);
        const (res, selected) = IVRS(A, B);
        log.debugf("Selected %s@%? from IVRS",
                    selected.name, selected.version);
        const parentToUse =
          if currentParent != nil then currentParent! else originalPackage;
        var idxToRemove = -1;
        if res == ivrsResult.keepA {
          idxToRemove = parentToUse.dependencies.find(
            new findMasonPackageNameVersionFunctor(B.name, B.version));
        } else if res == ivrsResult.keepB {
          idxToRemove = parentToUse.dependencies.find(
            new findMasonPackageNameVersionFunctor(A.name, A.version));
        }
        const removed = parentToUse.dependencies.getAndRemove(idxToRemove);
        log.debugf("Removed the duplicate dependency '%s' from parent '%s'",
                   removed.name, parentToUse.name);
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
            new findDependencyNamedFunctor(current.name));
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

/*
  Creates a lock file representation of this package and all its dependencies.
  This is a dump of all the information about this package and its dependencies,
  so it can be used to recreate the exact same package in the future without
  needing to resolve dependencies again.
*/
proc MasonPackage.createLockFile(): shared Toml throws {
  if !this.projectHome.isAbsolute() then
    throw new MasonError("projectHome must be an absolute path");

  log.debug("Creating lock file for package: ", name);
  var lock = new shared Toml();
  lock.set("root", new shared Toml(new map(string, shared Toml?)));
  lock.set("root.name", this.name);
  lock.set("root.version", this.version:string);
  lock.set("root.chplVersion", this.chplVersion.toBounded():string);
  lock.set("root.license", this.license);
  lock.set("root.type", this.pkgType:string);
  lock.set("root.source", this.source);
  lock.set("root.authors", TomlHelpers.iterableToTomlArray(this.authors));
  lock.set("root.copyrightYear", this.copyrightYear);
  lock.set("root.compopts", TomlHelpers.iterableToTomlArray(this.compopts));
  lock.set("root.docopts", TomlHelpers.iterableToTomlArray(this.docopts));
  lock.set("root.tests", TomlHelpers.iterableToTomlArray(this.tests,
      proc(t:test): string { return t.name:string;}));
  lock.set("root.examples", this.examples.toLock());

  // list the direct dependencies of the root package
  this.addDependencyListToLock(lock, "root.dependencies", this.dependencies);
  this.addDependencyListToLock(lock, "root.system", this.system);
  this.addDependencyListToLock(lock, "root.external", this.external);

  if !this.prerequisites.isEmpty() then
    lock.set("root.prerequisites",
                  new shared Toml(new map(string, shared Toml?)));
  for p in this.prerequisites do
    lock.set("root.prerequisites." + p.name, p.toLock()!);

  var deps: list(borrowed Dependency);
  for d in this.allDependencies() do deps.pushBack(d!);
  while !deps.isEmpty() {
    var dep = deps.popBack();
    log.debugf("Writing '%s' to lock file", dep.name);
    dep.addDepToLock(lock);
    if dep.isMasonPackage() {
      for d in (dep:MasonPackage).allDependencies() do deps.pushBack(d!);
    }
  }

  // embed extra metadata in the lock file that we can use to determine
  // if the lock file is out of date and needs to be regenerated.
  lock.set("lockfileVersion", MasonUtils.MASON_LOCKFILE_VERSION);

  return lock;

}

proc MasonPackage.addDependencyListToLock(lock: shared Toml, lockField: string,
                                         deps: list(?)) throws {
  const depNames = [dep in deps] new shared Toml?(dep.name);
  lock.set(lockField, depNames);
}

// TODO: we can generate a proper toml file for upload to the registry
// proc MasonPackage.createRegistryEntry(): shared Toml throws { }

override proc MasonPackage.toToml(): string throws {
  return '%s = %"S'.format(name, version:string);
}
// BUG: see https://github.com/chapel-lang/chapel/issues/28722
override proc type MasonPackage.fromToml(
  name: string, toml: borrowed Toml
): shared MasonPackage throws {
  if toml.tomlType != "string" then
    throw new MasonError("Mason dependencies must be specified as a " +
                          "single version string");
  var m = new shared MasonPackage(name, MasonUtils.parseChplVersionString(toml.s));
  return m;
}
override proc MasonPackage.loadInfo(postResolve=false) throws {
  const manifestPath =
    if !postResolve
      then this.getRegistryManifestPath()
      else this.getManifestPath();
  log.debugf("Loading manifest for dependency '%s' from path '%?'",
             this.name, manifestPath);
  const toml = parseToml(openReader(manifestPath));
  log.debug("Manifest contents: ", toml);
  this.fillFromManifest(toml, postResolve=postResolve);
}
override proc MasonPackage.addDepToLock(lock: shared Toml) throws {
  if lock.pathExists(this.name) {
    // this should not happen, we should never generate lock files
    // before resolving dependencies
    log.errorf("unhandled multiple dependencies on package '%s'", this.name);
  }

  var newToml = new shared Toml(new map(string, shared Toml?));
  newToml.set("name", this.name);
  newToml.set("version", this.version:string);
  newToml.set("chplVersion", this.chplVersion.toBounded():string);
  newToml.set("source", this.source);
  const compoptsList = [c in this.compopts] new shared Toml?(c);
  newToml.set("compopts", compoptsList);
  const docoptsList = [d in this.docopts] new shared Toml?(d);
  newToml.set("docopts", docoptsList);

  // list the direct dependencies of the root package
  this.addDependencyListToLock(newToml, "dependencies", this.dependencies);
  this.addDependencyListToLock(newToml, "system", this.system);
  this.addDependencyListToLock(newToml, "external", this.external);

  if !this.prerequisites.isEmpty() then
    newToml.set("prerequisites",
                  new shared Toml(new map(string, shared Toml?)));
  for p in this.prerequisites do
    newToml.set("prerequisites." + p.name, p.toLock()!);

  lock.set(this.name, newToml);
}
override proc GitMasonPackage.addDepToLock(lock: shared Toml) throws {
  super.addDepToLock(lock);

  lock.set(this.name + ".git", this.git);
  lock.set(this.name + ".branch", this.branch);
  lock.set(this.name + ".rev", this.rev);
  lock.set(this.name + ".revLock", this.revLock);

}

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
override proc SystemDependency.loadInfo(postResolve=false) throws {
  if postResolve then return;
  this.info = MasonSystem.getPCDep(name, version);
}
override proc SystemDependency.addDepToLock(lock: shared Toml) throws {
  if !lock.pathExists("system") then
    lock.set("system", new shared Toml(new map(string, shared Toml?)));
  if lock.pathExists("system." + this.name) {
    // this should not happen, we should never generate lock files
    // before resolving dependencies
    log.errorf("Duplicate system dependency added for ", this.name);
  }
  const (name, toml) = this.info.toToml();
  lock.set("system." + name, toml);
}

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
override proc ExternalDependency.loadInfo(postResolve=false) throws {
  if postResolve then return;
  this.info = MasonExternal.getExternalPackage(this.name, this.spec)[1];
}
override proc ExternalDependency.addDepToLock(lock: shared Toml) throws {
  if !lock.pathExists("external") then
    lock.set("external", new shared Toml(new map(string, shared Toml?)));
  if lock.pathExists("external." + this.name) {
    // this should not happen, we should never generate lock files
    // before resolving dependencies
    log.errorf("Duplicate external dependency added for ", this.name);
  }
  lock.set("external." + this.name, this.info!);
}

proc MasonPackage.getPackageHome(): path throws {
  log.debugf("Getting package home for dependency '%s'", this.name);
  if this.projectHome != "":path then
    return this.projectHome;
  const base = MasonEnv.MASON_HOME:path / "src";
  const slug = "%s-%s".format(this.name, this.version:string);
  const pkgDir = base / slug;
  this.cloneSource();
  if (pkgDir / "Mason.toml").isFile() then
    return pkgDir;
  else
    throw new MasonError("No toml file found in dependency for " +
                          this.name + "@" + this.version:string);
}
proc MasonPackage.getRegistryManifestPath(): path throws {
  for cached in MasonEnv.MASON_CACHED_REGISTRY {
    const tomlPath =
      cached:path / "Bricks" / this.name / (this.version:string + ".toml");
    if tomlPath.isFile() then return tomlPath;
  }
  throw new MasonError("No toml file found in mason-registry for " +
                       this.name + "@" + this.version:string);
}
proc MasonPackage.getManifestPath(): path throws {
  return getPackageHome() / "Mason.toml";
}

override proc GitMasonPackage.getPackageHome(): path throws {
  log.debugf("Getting package home for git dependency '%s'", this.name);
  const base = MasonEnv.MASON_HOME:path / "git";
  const slug = "%s-%s".format(this.name, this.reference);
  const gitDir = base / slug;
  this.cloneSource();
  if (gitDir / "Mason.toml").isFile() then
    return gitDir;
  else
    throw new MasonError("No toml file found in git dependency for " +
                          this.name + "@" + this.reference);
}
override proc GitMasonPackage.getRegistryManifestPath(): path throws {
  return getManifestPath();
}

proc MasonPackage.cloneSource() throws {
  const base = MasonEnv.MASON_HOME:path / "src";
  const slug = "%s-%s".format(name, version:string);
  const pkgDir = base / slug;
  if !pkgDir.isDir() {
    log.info("Downloading dependency: ", slug);
    log.debugf("Cloning dependency from %s into %?", source, pkgDir);
    if source == "" then
      throw new MasonError("No source specified for package '%s', cannot clone"
                            .format(name));
    MasonUtils.cloneSource(source, pkgDir, quiet=true);
    MasonUtils.checkoutSource(pkgDir, "v" + version:string, quiet=true);
  } else {
    log.debug("Dependency already cloned: ", slug);
  }
}

override proc GitMasonPackage.cloneSource() throws {
  const base = MasonEnv.MASON_HOME:path / "git";
  const slug = "%s-%s".format(name, reference);
  const gitDir = base / slug;
  if !gitDir.isDir() {
    log.info("Downloading dependency: ", slug);
    log.debugf("Cloning git dependency from %s into %?", git, gitDir);
    MasonUtils.cloneSource(git, gitDir, quiet=true);

    if reference != "HEAD" {
      log.debugf("Checking out reference '%s' for git dependency %s",
                reference, name);
      MasonUtils.checkoutSource(gitDir, reference, quiet=true);
    }
  } else {
    log.debug("Git dependency already cloned: ", slug);
    if revLock != "" {
      log.debug("Already updated to locked rev: ", revLock);
      return;
    }
    if branch != "" || reference == "HEAD" {
      log.info("Fetching latest changes for: ", slug);
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


proc PrerequisiteDependency.toLock(): shared Toml? throws {
  var newToml = new shared Toml(new map(string, shared Toml?));
  newToml.set("name", this.name);
  newToml.set("type", this.prereqType:string);
  newToml.set("build", TomlHelpers.iterableToTomlArray(this.buildCommand));
  newToml.set("printFlags",
    TomlHelpers.iterableToTomlArray(this.printFlagsCommand));
  newToml.set("clean", TomlHelpers.iterableToTomlArray(this.cleanCommand));
  newToml.set("env", TomlHelpers.iterableToTomlArray(
    for (k,v) in zip(this.envVariables.keys(), this.envVariables.values()) do
      (k + "=" + v)));
  return newToml;
}

record versionRange {
  var low: Version.version;
  var high: Version.version;
  proc init() {
    this.low = Version.version.zero();
    this.high = Version.version.max();
  }
  proc init(low, high) {
    this.low = low;
    this.high = high;
  }
  proc init(low) {
    this.low = low;
    this.high = Version.version.max();
  }

  proc isUnboundedUpper() do return high == Version.version.max();
  proc toPrettyStr(): string do return MasonUpdate.prettyVersionRange(low, high);
  proc toTuple() do return (low, high);
  /* Returns a new versionRange with the max set to the current Chapel version*/
  proc toBounded() throws do if this.isUnboundedUpper()
    then return new versionRange(this.low, MasonUtils.getChapelVersionInfo());
    else return this;
}
operator :(x: versionRange, type t: string): string do
  if x.isUnboundedUpper() then return x.low:string;
                          else return x.low:string + ".." + x.high:string;
versionRange implements writeSerializable;
proc versionRange.serialize(writer, ref serializer) throws do
  writer.write(this:string);

module VersionResolution {
import super.MasonPackage;
import MasonUtils;
import MasonUpdate;
import MasonLogger;
import Version;
import IO.format;

private var log = MasonLogger.getLogger("package");
/*
The current resolution strategy for Mason 0.2.0 is the IVRS as described below:
    1. If multiple bug fixes of a package are present in the project,
       mason will use the latest bug fix. (ex. 1.1.0, 1.1.1 --> 1.1.1)
    2. If multiple minor versions of a package are present in the project,
       mason will use the latest minor version within the common major version.
       (ex. 1.4.3, 1.7.0 --> 1.7)
    3. If multiple major versions are present, mason will print an error.
       (ex. 1.13.0, 2.1.0 --> incompatible)
*/
enum ivrsResult { keepA, keepB }
proc IVRS(
  A: borrowed MasonPackage, B: borrowed MasonPackage
): (ivrsResult, borrowed MasonPackage) throws {
  const name = A.name;
  const okA = checkChapelVersion(A);
  const okB = checkChapelVersion(B);
  if !okA && !okB {
    throw new MasonUtils.MasonError(
      "Dependency resolution error: unable to find version of '" +
      name + "' compatible with your version of Chapel (" +
      MasonUtils.getChapelVersionInfo():string + "): " +
      "v" + A.version:string + " expecting " + A.chplVersion.toPrettyStr() +
      ", v" + B.version:string + " expecting " + B.chplVersion.toPrettyStr());
  } else if okA && !okB {
    return (ivrsResult.keepA, A);
  } else if !okA && okB {
    return (ivrsResult.keepB, B);
  }

  if A.version == B.version then return (ivrsResult.keepA, A);
  if A.version.major != B.version.major {
    throw new MasonUtils.MasonError(
      "Dependency resolution error: package '" + name +
      "' used by multiple packages expecting different major versions: " +
      "v" + A.version:string + ", v" + B.version:string);
  } else if A.version.minor != B.version.minor {
    if A.version.minor > B.version.minor then
      return (ivrsResult.keepA, A);
    else
      return (ivrsResult.keepB, B);
  } else {
    if A.version.update > B.version.update then
      return (ivrsResult.keepA, A);
    else
      return (ivrsResult.keepB, B);
  }
}

proc checkChapelVersion(x: borrowed MasonPackage) throws {
  const current = MasonUtils.getChapelVersionInfo();
  var (low, hi) = x.chplVersion.toTuple();
  return low <= current && current <= hi;
}
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
