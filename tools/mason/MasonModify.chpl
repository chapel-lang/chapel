/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

use Regexp;
use TOML;
use MasonUtils;
private use Map;

/* Modify manifest file */
proc masonModify(args) throws {
  try! {

    // Check for help flags
    for arg in args[1..] {
      if arg == '-h' || arg == '--help' {
        masonModifyHelp();
        exit(0);
      }
    }

    // Check for incorrect usage
    if args.size < 3 {
      masonModifyHelp();
      exit(0);
    }

    // Parse arguments
    var external = false;
    var system = false;
    var add = false;
    var remove = false;
    var dep = "";
    for arg in args[1..] {
      if arg.startsWith('-') {
        // Parse optional arguments
        select (arg) {
          when '--system' {
            system = true;
          }
          when '--external' {
            external = true;
          }
          otherwise {
            throw new owned MasonError("Unrecognized flag: " + arg);
          }
        }
      } else {
        // Parse positional arguments
        select (arg) {
          when 'add' {
            add = true;
          }
          when 'rm' {
            remove = true;
          } otherwise {
            if dep.isEmpty() then dep = arg;
            else throw new owned MasonError("More than one package specified: " + dep + ', ' + arg);
          }
        }
      }
    }

    // Check for errors in argument values
    if external && system then
      throw new owned MasonError("Use only '--external' or '--system'");
    if dep == "" then
      throw new owned MasonError("Must enter a dependency");
    if add && remove then
      throw new owned MasonError("Use only 'add' or 'rm'");
    // Note: We don't need to check for !add && !remove because
    //       this function is only executed when 'add' or 'rm' is passed

    // Modify the manifest file based on arguments
    const cwd = getEnv("PWD");
    const projectHome = getProjectHome(cwd, "Mason.toml");

    const result = modifyToml(add, dep, external, system, projectHome);
    generateToml(result[1], result[2]);
    delete result[1];
  }
  catch e: MasonError {
    writeln(e.message());
    exit(1);
  }
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
      if spec.find("@") == 0 {
        throw new owned MasonError("Dependency formatted incorrectly.\nFormat: package@version");
      }
      const split = spec.split('@');
      const dependency = split[1];
      const version = split[2];
      checkDepName(dependency);
      checkVersion(version);

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
      if spec.find('@') != 0 {
        const split = spec.split('@');
        depName = split[1];
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

  const pattern = compile("([0-9].[0-9].[0-9][a-zA-Z]?)");
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
