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
use IO;
use MasonEnv;
use MasonHelp;
use MasonModify;
use MasonUpdate;
use MasonUtils;
use MasonNew;
use Path;
use Subprocess;


/*
  Creates a new library project at a given directory
  mason new <projectName/directoryName>
*/
proc masonLightweight(args: [] string) throws {
  var parser = new argumentParser(helpHandler=new MasonNewHelpHandler());
  var nameOpt = parser.addOption(name="legalname",
                                 opts=["--name"]);
  var dirArg = parser.addArgument(name="directory", numArgs=0..1);

  parser.parseArgs(args);

  var packageName = '';
  var dirName = '';
  var version = '';
  var chplVersion = '';
  var license = 'None';

  try! {
    if args.size == 1 {
      var metadata = beginInteractiveSession('','','','');
      packageName = metadata[0];
      dirName = here.cwd();
      version = metadata[1];
      chplVersion = metadata[2];
      license = metadata[3];
    }
    if validatePackageName(dirName=packageName) {
      makeBasicToml(dirName=packageName, path=dirName, version, chplVersion, license);
      writeln("Created toml file for lightweight mason");
    }
  }
  catch e: MasonError {
    writeln(e.message());
    exit(1);
  }
}