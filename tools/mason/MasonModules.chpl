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

  if !isDir(MASON_HOME) {
    mkdir(MASON_HOME, parents=true);
  }
  const (tomlName, lockName) = updateLock(skipUpdate, show=false);

  const projectHome = getProjectHome(path.cwd());
  var tomlFile = parseToml(open(projectHome / tomlName, ioMode.r));
  var lockFile = parseToml(open(projectHome / lockName, ioMode.r));

  // generate list of dependencies and get src code
  var (sourceList, gitList) = genSourceList(lockFile);
  getSrcCode(sourceList, skipUpdate, false);
  getGitCode(gitList, skipUpdate, false);

  const depPath = MASON_HOME:path / "src";
  const gitDepPath = MASON_HOME:path / "git";
  var modules: list(path);
  // can't use _ since it will leak
  // see https://github.com/chapel-lang/chapel/issues/25926
  @chplcheck.ignore("UnusedLoopIndex")
  for (_x, name, version) in srcSource.iterList(sourceList) {
    const depM = depPath / (name + "-" + version) / "src" / (name + ".chpl");
    modules.pushBack(depM);
  }

  // can't use _ since it will leak
  // see https://github.com/chapel-lang/chapel/issues/25926
  @chplcheck.ignore("UnusedLoopIndex")
  for (_x, name, branch, _y) in gitSource.iterList(gitList) {
    const gitDepSrc =
      gitDepPath / (name + "-" + branch) / "src" / (name + ".chpl");
    modules.pushBack(gitDepSrc);
  }

  const srcPath = projectHome / "src";
  const testPath = projectHome / "test";
  const examplePath = projectHome / "example";
  const sources = [f in srcPath.findFiles(recursive=true)]
                    if f.suffix == ".chpl" then f;

  const tests =
    [f in MasonTest.getTests(lockFile, projectHome)] testPath / f;
  const examples =
    [f in MasonExample.getExamples(tomlFile, projectHome)] examplePath / f;

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
