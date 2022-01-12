/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
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

use ArgumentParser;
use FileSystem;
use Map;
use MasonUtils;
use Regex;
use TOML;


/* Modify manifest file */
proc masonModify(args: [] string) throws {

  var parser = new argumentParser(helpHandler=new MasonModifyHelpHandler());

  var extFlag = parser.addFlag("external", defaultValue=false);
  var sysFlag = parser.addFlag("system", defaultValue=false);
  var depArg = parser.addArgument("dep", numArgs=0..1);

  parser.parseArgs(args);

  if !depArg.hasValue() {
    stderr.writeln("package name missing value");
    masonModifyHelp();
    exit(1);
  }
  var add = false;
  if args[0] == "add" then add = true;
  // TODO: remove this once argument parser supports mutual exclusivity
  if extFlag.valueAsBool() && sysFlag.valueAsBool() {
    throw new owned MasonError("Use only '--external' or '--system'");
  }

  // Modify the manifest file based on arguments
  const cwd = here.cwd();
  const projectHome = getProjectHome(cwd, "Mason.toml");

  const result = modifyToml(add, depArg.value(), extFlag.valueAsBool(),
                            sysFlag.valueAsBool(), projectHome);
  generateToml(result[0], result[1]);
  delete result[0];
}

proc modifyToml(add: bool, spec: string, external: bool, system: bool,
                projectHome: string, tf="Mason.toml") throws {

  const tomlPath = '/'.join(projectHome, tf);
  const openFile = openreader(tomlPath);
  const toml = parseToml(openFile);
  var newToml: unmanaged Toml?;

  try! {

    // Adding a dependency
    if add {
      if spec.find("@") == -1 {
        throw new owned MasonError("Dependency formatted incorrectly.\nFormat: package@version");
      }
      const split = spec.split('@');
      const dependency = split[0];
      const version = split[1];
      // Name and version checks are only valid for mason packages
      if !external && !system {
        checkDepName(dependency);
        checkVersion(version);
      }

      if system && add {
        writeln(" ".join("Adding system dependency", dependency, "version", version));
        newToml = masonSystemAdd(toml, dependency, version);
      }
      else if external && add {
        writeln(" ".join("Adding external dependency with spec", spec));
        newToml = masonExternalAdd(toml, dependency, spec);
      }
      else {
        writeln(" ".join("Adding Mason dependency", dependency, "version", version));
        newToml = masonAdd(toml, dependency, version);
      }
    }

    // Removing a dependency
    else {
      var depName: string;
      if spec.find('@') != -1 {
        const split = spec.split('@');
        depName = split[0];
      }
      else depName = spec;
      const dependency = depName;
      checkDepName(depName);

      if !system && !external {
        writeln("Removing Mason dependency " + dependency);
        newToml = masonRemove(toml, dependency);
      }
      else if system{
        writeln("Removing system dependency " + dependency);
        newToml = masonSystemRemove(toml, dependency);
      }
      else if external{
        writeln("Removing external dependency " + dependency);
        newToml = masonExternalRemove(toml, dependency);
      }
    }
  }
  catch e: MasonError {
    writeln(e.message());
    exit(1);
  }
  return (newToml!, tomlPath);
}

/* Add a mason dependency to Mason.toml */
private proc masonAdd(toml: unmanaged Toml, toAdd: string, version: string) throws {
  if toml.pathExists("dependencies") {
    if toml.pathExists("dependencies." + toAdd) {
      throw new owned MasonError("A dependency by that name already exists in Mason.toml");
    }
    else {
      toml["dependencies"]!.set(toAdd, version);
    }
  }
  // Create dependency table if it doesnt exist
  else {
    var tdom: domain(string);
    var deps: [tdom] unmanaged Toml?;
    toml.set("dependencies", deps);
    toml["dependencies"]!.set(toAdd, version);
  }
  return toml;
}

/* Remove a mason dependency from Mason.toml */
private proc masonRemove(toml: unmanaged Toml, toRm: string) throws {
  if toml.pathExists("dependencies") {
    if toml.pathExists("dependencies." + toRm) {
      var old = toml["dependencies"]![toRm]!;
      toml["dependencies"]!.A.remove(toRm);
      delete old;
    }
    else {
      throw new owned MasonError("No dependency exists by that name");
    }
  }
  else {
    throw new owned MasonError("No dependencies");
  }
  return toml;
}

/* Add a system dependency to Mason.toml */
private proc masonSystemAdd(toml: unmanaged Toml, toAdd: string, version: string) throws {

  if toml.pathExists("system") {
    if toml.pathExists("system." + toAdd) {
      throw new owned MasonError("A dependency by that name already exists in Mason.toml");
    }
    else {
      toml["system"]!.set(toAdd, version);
    }
  }
  else {
    var pkgdom: domain(string);
    var pkgdeps: [pkgdom] unmanaged Toml?;
    toml.set("system", pkgdeps);
    toml["system"]!.set(toAdd, version);
  }
  return toml;
}

/* Remove a system dependency from Mason.toml */
private proc masonSystemRemove(toml: unmanaged Toml, toRm: string) throws {
  if toml.pathExists("system") {
    if toml.pathExists("system." + toRm) {
      var old = toml["system"]![toRm]!;
      toml["system"]!.A.remove(toRm);
      delete old;
    }
    else {
      throw new owned MasonError("No system dependency exists by " + toRm);
    }
  }
  else {
    throw new owned MasonError("No system dependency exists by " + toRm);
  }
  return toml;
}

/* Add an external dependency to Mason.toml */
private proc masonExternalAdd(toml: unmanaged Toml, toAdd: string, spec: string) throws {
  if toml.pathExists("external") {
    if toml.pathExists("external." + toAdd) {
      throw new owned MasonError("An external dependency by that name already exists in Mason.toml");
    }
    else {
      toml["external"]!.set(toAdd, spec);
    }
  }
  else {
    var exdom: domain(string);
    var exdeps: [exdom] unmanaged Toml?;
    toml.set("external", exdeps);
    toml["external"]!.set(toAdd, spec);
  }
  return toml;
}

/* Remove an external dependency from Mason.toml */
private proc masonExternalRemove(toml: unmanaged Toml, toRm: string) throws {
  if toml.pathExists("external") {
    if toml.pathExists("external." + toRm) {
      var old = toml["external"]![toRm]!;
      toml["external"]!.A.remove(toRm);
      delete old;
    }
    else {
      throw new owned MasonError("No external dependency exists by that name");
    }
  }
  else {
    throw new owned MasonError("No external dependency exists by that name");
  }
  return toml;
}

/* Generate the modified Mason.toml */
proc generateToml(toml: borrowed Toml, tomlPath: string) {
  const tomlFile = open(tomlPath, iomode.cw);
  const tomlWriter = tomlFile.writer();
  tomlWriter.writeln(toml);
  tomlWriter.close();
  tomlFile.close();
}

proc checkVersion(version: string) throws {

//  const pattern = compile("([0-9].[0-9].[0-9][a-zA-Z]?)");
  const pattern = compile("""^((([0-9]+)\.([0-9]+)\.([0-9]+)(?:-([0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?)(?:\+([0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?)$""");
  if !pattern.match(version) {
    throw new owned MasonError("Version formatting incorrect. ex. 1.2.3");
  }
}

private proc checkDepName(dep: string) throws {
  if !isIdentifier(dep) {
      throw new owned MasonError("Bad package name '" + dep +
                             "' - only Chapel identifiers are legal package names");
  }
}
