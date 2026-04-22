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
  Manifest provides a unified way of representing the information in a
  Mason.toml file as a Chapel record, rather than relying on raw TOML reads.
*/
module Manifest {
  use List;
  import IO.{format, fileReader};
  use TOML;
  import Reflection;

  import MasonUtils;
  import MasonUtils.MasonError;

  import ThirdParty.TemplateString.templateString;
  import ThirdParty.Pathlib.path;

  record manifestFile {
    // required
    var name: string;
    var version: string;
    var chplVersion: string;
    var license: string;
    var pkgType: packageType;

    // TODO: can only have 1 owned class
    // https://github.com/chapel-lang/chapel/issues/22261
    var dependencies: list(owned ChapelDependency?);
    var system: list(owned SystemDependency?);
    var external: list(owned ExternalDependency?);

    // optional
    var source: string;
    var authors: list(string);
    var copyrightYear: string;
    var compopts: list(string);
    var docopts: list(string);
    var tests: list(test);
    var examples: list(example);
  }

  proc type manifestFile.defaultNewPkg(name: string,
                                       pkgType = packageType.default) throws {
    var b = new manifestFile();
    b.name = name;
    b.version = "0.1.0";
    b.chplVersion = MasonUtils.getChapelVersionStr();
    b.license = "None";
    b.pkgType = pkgType;
    return b;
  }

  proc manifestFile.toToml(): string throws {
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
        "\n".join(for dep in dependencies do dep!.toToml());
    }

    if !system.isEmpty() {
      s += "\n\n[system]\n" +
        "\n".join(for dep in system do dep!.toToml());
    }
    if !external.isEmpty() {
      s += "\n\n[external]\n" +
        "\n".join(for dep in external do dep!.toToml());
    }

    return s;
  }


  proc readField(ref manifest: manifestFile,
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
    ref field = Reflection.getFieldRef(manifest, fieldName);
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
    } else {
      throw new MasonError(
        "Unsupported field type of '%s' in manifest for '%s'"
          .format(field.type:string, fieldName) +
        ", this is likely a developer error. " +
        "Please report it to the Mason developers.");
    }

  }
  proc type manifestFile.fromToml(tomlStr: ?T): manifestFile throws
  where T == string || isSubtype(T, fileReader) {
    var toml = parseToml(tomlStr);
    if !toml.pathExists("brick") then
      throw new MasonError("TOML must contain [brick] section");
    var m = new manifestFile();
    readField(m, toml, "brick.name", "name", required=true);
    readField(m, toml, "brick.version", "version", required=true);
    readField(m, toml, "brick.chplVersion", "chplVersion", required=true);
    readField(m, toml, "brick.license", "license", required=true);
    readField(m, toml, "brick.type", "pkgType", required=true);

    // read optional fields
    readField(m, toml, "brick.source", "source", required=false);
    readField(m, toml, "brick.authors", "authors", required=false);
    readField(m, toml, "brick.copyrightYear", "copyrightYear", required=false);
    readField(m, toml, "brick.compopts", "compopts", required=false);
    readField(m, toml, "brick.docopts", "docopts", required=false);

    // read tests and examples
    readField(m, toml, "brick.tests", "tests", required=false);
    readField(m, toml, "examples", "examples", required=false);
    // examples have other options too

    // TODO

    return m;
  }

  enum packageType {
    application, library, light
  }
  proc type packageType.default do return this.application;


  class Dependency {
    var name: string;
    proc toToml(): string throws {
      return name;
    }
    proc type fromToml(toml: borrowed Toml): owned Dependency throws {
      // TODO:
      return new Dependency("");
    }
  }
  class ChapelDependency: Dependency { }
  class MasonDependency : ChapelDependency {
    var version: string;
    override proc toToml(): string throws {
      return '%s = %"S'.format(name, version);
    }
  }
  class GitDependency: Dependency {
    var git: string;
    var branch: string;
    var rev: string;

    override proc toToml(): string throws {
      var s = '%s = { git = %"S'.format(name, git);
      if branch != "" then s += ', branch = %"S'.format(branch);
      if rev != "" then s += ', rev = %"S'.format(rev);
      s += " }";
      return s;
    }
  }
  class SystemDependency: Dependency {
    var version: string;

    override proc toToml(): string throws {
      return '%s = %"S'.format(name, version);
    }
  }
  class ExternalDependency: Dependency {
    var spec: string;

    override proc toToml(): string throws {
      return '%s = %"S'.format(name, spec);
    }
  }
  // TODO: allow prerequisites in the toml
  // enum prerequisiteType {
  //   makefile, script
  // }
  // proc type prerequisiteType.default do return this.makefile;
  // class PrerequisiteDependency: Dependency {
  //   var prereqType: prerequisiteType;
  //   var buildCommand: list(string);
  //   var printFlagsCommand: list(string);
  //   var cleanCommand: list(string);
  // }

  proc listFromToml(
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

  proc listFromToml(
    type resultType,
    tomlField: string,
    value: borrowed Toml
  ): resultType throws
  where isSubtype(resultType, list(test)) {
    var result: resultType;
    if value.tomlType != "array" then
      throw new MasonError("'%s' must be an array of strings"
                            .format(tomlField));
    for x in value.arr do
      result.pushBack(test.fromToml(x!));
    return result;
  }

  proc listFromToml(
    type resultType,
    tomlField: string,
    value: borrowed Toml
  ): resultType throws
  where isSubtype(resultType, list(example)) {
    var result: resultType;
    use IO;
    value.writeJSON(stdout);
    if const examplesList = value["examples"] {
      if examplesList.tomlType != "array" then
        throw new MasonError("'%s' must be a list of examples"
                              .format(tomlField+".examples"));
      for exampleToml in examplesList.arr {
        var ex = example.fromToml(exampleToml!);
        var compopts: list(string);
        var execopts: list(string);
        if value.pathExists(ex.name.stem) {
          // TODO parse compopts
        }
      }
      // TODO check that there is no `[examples.UNKNOWN]`
    }
    // if value.tomlType != "array" then
    //   throw new MasonError("'%s' must be an array of strings"
    //                         .format(tomlField));
    // for x in value.arr do
    //   result.pushBack(example.fromToml(x!));
    return result;
  }


  @chplcheck.ignore("UnusedFormal")
  proc listFromToml(
    type resultType,
    tomlField: string,
    value: borrowed Toml
  ): resultType throws {
    compilerError("Unhandled dependency type in manifest: " +
                  resultType:string);
  }


  record test {
    var name: path;
    proc toToml(): string throws {
      return name:string;
    }
    proc type fromToml(toml: borrowed Toml): test throws {
      if toml.tomlType != "string" then
        throw new MasonError("Tests must be specified as an array of strings");
      const name: path = toml.s.strip();
      if name.suffix != ".chpl" then
        throw new MasonError("Tests must be Chapel files");
      return new test(name);
    }
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
  }

}
