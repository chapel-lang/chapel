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

  record lockFile {
    var dependencies: list(masonDependency);
    var systemDependencies: list(systemDependency);
    var externalDependencies: list(externalDependency);
  }

  record rootPackage {
    var name: string;
    var version: string;
    var chplVersion: string;
    var license: string;
    var pkgType: packageType;

    var compopts: list(string);
    var prereqs: list(prereq);
  }

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
  class ChapelDependency: Dependency {
    var compopts: list(string);
  }
  class MasonDependency: ChapelDependency {
    var version: string;
  }
  class GitDependency: ChapelDependency {
    var git: string;
    var branch: string;
    var rev: string;
  }

  class SystemDependency: Dependency {

  }
  class ExternalDependency: Dependency {

  }
  class PrerequisiteDependency: Dependency {
    var compopts: list(string);
  }

}
