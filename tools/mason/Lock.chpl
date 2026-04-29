/*
 * Copyright 2020-2026 Hewlett Packard Enterprise Development LP
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

/*
  Lock contains a representing of the Mason.lock file
*/
module Lock {

import TOML.Toml;
import TomlHelpers;
import Manifest;
import MasonUtils.MasonError;
import List.list;
import ThirdParty.TemplateString.templateString;

record lockFile {
  var root: rootPackage;
  var dependencies: list(owned MasonDependency?);
  var systemDependencies: list(owned SystemDependency?);
  var externalDependencies: list(owned ExternalDependency?);
}

proc lockFile.toToml(): string throws {
  var entries: list(string);
  entries.pushBack(root.toToml());
  for d in dependencies do
    entries.pushBack(d!.toToml());
  if systemDependencies.size > 0 then
    entries.pushBack("[system]");
  for s in systemDependencies do
    entries.pushBack(s!.toToml());
  if externalDependencies.size > 0 then
    entries.pushBack("[external]");
  for e in externalDependencies do
    entries.pushBack(e!.toToml());
  return "\n".join(entries.toArray());
}
proc type lockFile.fromToml(tomlStr: ?T): lockFile throws
where T == string || isSubtype(T, fileReader) {
  var toml = parseToml(tomlStr);

  var lf = new lockFile();

  return lf;
}

record rootPackage {
  var name: string;
  var version: string;
  var chplVersion: string;
  var dependencies: list(string);
  proc toToml(): string throws {
    const t: templateString = """
    [root]
    name = "{{name}}"
    version = "{{version}}"
    chplVersion = "{{chplVersion}}"
    """.dedent();
    return t(["name"=>name, "version"=>version, "chplVersion"=>chplVersion]);
  }
  proc type fromToml(toml: borrowed Toml): rootPackage throws {
    if toml.tomlType != "toml" then
      throw new MasonError("Invalid root package in lock file");
    var r = new rootPackage();
    r.name = name;
    (r.version, r.chplVersion) =
      TomlHelpers.readStringsFromTable(toml, ("version", "chplVersion"));
    return r;
  }
}

class Dependency {
  var name: string;
  proc toToml(): string throws do return name;
  @chplcheck.ignore("UnusedFormal")
  proc type fromToml(name: string,
                     toml: borrowed Toml): owned Dependency throws {
    return new Dependency(name);
  }
}
class ChapelDependency: Dependency {
  var source: string;
  var chplVersion: string;
}
class MasonDependency: ChapelDependency {
  var version: string;
  override proc toToml(): string throws {
    const t: templateString = """
    [{{name}}]
    name = "{{name}}"
    source = "{{source}}"
    version = "{{version}}"
    chplVersion = "{{chplVersion}}"
    """.dedent();
    return t(["name"=>name, "source"=>source,
              "version"=>version, "chplVersion"=>chplVersion]);
  }
}
class GitDependency: ChapelDependency {
  var branch: string;
  var rev: string;
  override proc toToml(): string throws {
    const t: templateString = """
    [{{name}}]
    name = "{{name}}"
    source = "{{source}}"
    branch = "{{branch}}"
    rev = "{{rev}}"
    chplVersion = "{{chplVersion}}"
    """.dedent();
    return t(["name"=>name, "source"=>source, "branch"=>branch,
              "rev"=>rev, "chplVersion"=>chplVersion]);
  }
}

class SystemDependency: Dependency {
  var version: string;
  override proc toToml(): string throws {
    const t: templateString = """
    [systen.{{name}}]
    name = "{{name}}"
    version = "{{version}}"
    """.dedent();
    return t(["name"=>name, "version"=>version]);
  }
}
class ExternalDependency: Dependency {
  var spec: string;
  override proc toToml(): string throws {
    const t: templateString = """
    [external.{{name}}]
    name = "{{name}}"
    spec = "{{spec}}"
    """.dedent();
    return t(["name"=>name, "spec"=>spec]);
  }
}

}
