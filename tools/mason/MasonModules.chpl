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

/**/
module MasonModules {

use ArgumentParser;
use FileSystem;
use List;
use Map;
use MasonUtils;
use MasonHelp;
use MasonEnv;
use MasonBuild;
use Subprocess;
use MasonUpdate;
import MasonExample;
import MasonTest;
use TOML;
import Path.joinPath;
import ThirdParty.Pathlib.path;
use ThirdParty.Pathlib.IOHelpers;
import Package;

enum OutputFormat {
  json,
  text
}

// A call to `mason modules` will print to screen the flags that are
// required to include the mason packages specified in the TOML file
// of a mason project.
// For example, if a mason project had two dependencies, the result
// would be a string of the paths to the modules that need to be added
// to a command line `chpl` call to use those modules. This can allow
// users to get compilation flags from mason without having to use
// `mason build` in their project.
proc masonModules(args: [] string) throws {

  var parser = new argumentParser(helpHandler=new MasonModulesHelpHandler());

  var updateFlag = parser.addFlag(name="update", flagInversion=true);
  var formatFlag = parser.addOption(name="format", defaultValue="text");

  parser.parseArgs(args);

  var skipUpdate = MASON_OFFLINE;
  if updateFlag.hasValue() {
    skipUpdate = !updateFlag.valueAsBool();
  }

  var outputFormat: OutputFormat;
  try {
    outputFormat = formatFlag.value():OutputFormat;
  } catch {
    throw new MasonError("Invalid output format specified. " +
                         "Valid options are 'json' and 'text'.");
  }

  const package = Package.getMasonPackage(skipUpdate, show=false, force=false);
  const projectHome = package.projectHome;

  var modules: list(string);
  for dep in package.allDependencies() do
    modules.pushBack(dep.getSourceFiles());

  const srcPath = projectHome / "src";
  const testPath = projectHome / "test";
  const examplePath = projectHome / "example";

  const sources = [f in srcPath.findFiles(recursive=true)]
                    if f.suffix == ".chpl" then f;
  const tests = [t in package.tests] testPath / t.name;
  const examples = [e in package.examples] examplePath / e.name;

  select outputFormat {
    when OutputFormat.text {
      var sep = "";
      for m in modules {
        write(sep, m:string);
        sep = " ";
      }
      writeln();
    }
    when OutputFormat.json {
      import JSON;
      var jsonObj = new map(string, list(string));
      jsonObj["modules"] = new list([m in modules] m:string);
      jsonObj["sources"] = new list([s in sources] s:string);
      jsonObj["tests"] = new list([t in tests] t:string);
      jsonObj["examples"] = new list([e in examples] e:string);
      stdout.withSerializer(new JSON.jsonSerializer()).writeln(jsonObj);
    }
    otherwise {
      // this should be impossible since we check the output format above
      throw new MasonError("Invalid output format specified. " +
                           "Valid options are 'json' and 'text'.");
    }
  }
}

}
