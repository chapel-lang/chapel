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
  import IO.format;

  import MasonUtils;

  import ThirdParty.TemplateString.templateString;

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

  enum packageType {
    application, library, light
  }
  proc type packageType.default do return this.application;


  class Dependency {
    var name: string;
    proc toToml(): string throws {
      return name;
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

  record test {
    var name: string;
    proc toToml(): string throws {
      return name;
    }
  }
  record example {
    var name: string;
    var compopts: list(string);
    var execopts: list(string);
    proc toToml(): string throws {
      if compopts.isEmpty() && execopts.isEmpty() then
        return "";
      var s = "[examples.%s]".format(name);
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
  }

}
