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
module MasonBuild {

import Package;
import BuildInfo;
import BuildInfo.MasonPackage;
import Compilation;

use ArgumentParser;
use List;
use Map;
use MasonUtils;
use MasonHelp;
use MasonEnv;
use MasonUpdate;
use MasonSystem;
use MasonExample;
import MasonLogger;
use Subprocess;
use TOML;

import ThirdParty.Pathlib.path;
use ThirdParty.Pathlib.IOHelpers;

import Path;
import FileSystem;

private var log = MasonLogger.getLogger("mason build");

proc masonBuild(args: [] string) throws {

  var parser = new argumentParser(helpHandler=new MasonBuildHelpHandler());

  var showFlag = parser.addFlag(name="show", defaultValue=false);
  var releaseFlag = parser.addFlag(name="release", defaultValue=false);
  var forceFlag = parser.addFlag(name="force", defaultValue=false);
  var exampleOpts = parser.addOption(name="example", numArgs=0..);
  var updateFlag = parser.addFlag(name="update", flagInversion=true);

  var passArgs = parser.addPassThrough();

  parser.parseArgs(args);
  log.debug("Arguments parsed");

  var show = showFlag.valueAsBool();
  var release = releaseFlag.valueAsBool();
  var force = forceFlag.valueAsBool();
  var compopts = new list(passArgs.values());
  var example = exampleOpts._present; // --example provided w/wo a value
  var skipUpdate = MASON_OFFLINE;

  if updateFlag.hasValue() {
    if updateFlag.valueAsBool() then skipUpdate = false;
    else skipUpdate = true;
  }

  var package = Package.getMasonPackage(skipUpdate, show=show, force=force);
  if package.pkgType == Package.packageType.light then
    throw new MasonError("Mason light projects do not " +
                         "currently support 'mason build'");

  var options = new BuildInfo.buildOptions(releaseMode=release);
  options.commandLineCompopts = compopts;

  log.debug("Is example? ", example);
  if example {
    var examplesToBuild = new list(exampleOpts.values());
    Compilation.buildExamples(package, examplesToBuild, options, force=force);
  } else {
    log.debug("About to build program");
    Compilation.buildProgram(package, options, force=force);
  }
}

}
