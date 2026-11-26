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
module MasonRun {

import Package;
import BuildInfo;
import BuildInfo.MasonPackage;
import Compilation;
import Execution;

use ArgumentParser;
use List;

import MasonExample;
use MasonHelp;
import MasonEnv;
import MasonUtils;
import MasonUtils.{MasonError};
import MasonLogger;

private var log = MasonLogger.getLogger("mason run");

proc masonRun(args: [] string) throws {

  var parser = new argumentParser(helpHandler=new MasonRunHelpHandler());

  var showFlag = parser.addFlag(name="show", defaultValue=false);
  var releaseFlag = parser.addFlag(name="release", defaultValue=false);
  var buildFlag = parser.addFlag(name="build", defaultValue=false);

  // not actually flags for Run, but rather for build
  // TODO: I'm failure certain _present being true when defaultValue is set
  // is a bug in ArgumentParser, but changing that may have wider impacts
  // for now, use not default so _present is meaningful
  // for the purposes of this function, the value of forceFlag isn't used
  // anyways
  var forceFlag = parser.addFlag(name="force" /*defaultValue=false*/);
  var updateFlag = parser.addFlag(name="update", flagInversion=true);

  var exampleOpts = parser.addOption(name="example",
                                     numArgs=0..);

  var passArgs = parser.addPassThrough();

  parser.parseArgs(args);

  const show = showFlag.valueAsBool();
  const release = releaseFlag.valueAsBool();
  const force = forceFlag.hasValue() && forceFlag.valueAsBool();
  const build = buildFlag.valueAsBool();
  const execopts = new list(passArgs.values());
  const example = exampleOpts._present; // --example provided w/wo a value
  var skipUpdate = MasonEnv.MASON_OFFLINE;
  if updateFlag.hasValue() {
    if updateFlag.valueAsBool() then skipUpdate = false;
    else skipUpdate = true;
  }

  var package = Package.getMasonPackage(skipUpdate, show=show, force=force);
  if package.pkgType == Package.packageType.light then
    throw new MasonError("Mason light projects do not " +
                         "currently support 'mason run'");
  if !example && package.pkgType != Package.packageType.application then
    throw new MasonError(
      "Only mason applications can be run, but this is a Mason " +
      package.pkgType:string);

  // don't specify build flags unless we are actually building
  if !buildFlag.valueAsBool() {
    if forceFlag._present then
      throw new MasonError("The --force flag is only valid " +
                           "when used with --build");
    if updateFlag._present then
      throw new MasonError("The --[no]-update flag is only valid " +
                           "when used with --build");
  }
  var options = new BuildInfo.buildOptions(releaseMode=release);

  log.debug("Is example? ", example);

  if example {
    if !exampleOpts.hasValue() || exampleOpts.value().startsWith("-") {
      MasonExample.printAvailableExamples(package);
      return;
    }
    const exampleNames = new list(exampleOpts.values());
    if build {
      log.debug("Building example before running");
      Compilation.buildExamples(package, exampleNames, options,
                                force=force);
    }
    for exName in exampleNames {
      var idx = package.examples.find(exName);
      if idx == -1 then
        throw new MasonError("Mason could not find example: " + exName);
      var example = package.examples[idx];
      const info = Execution.runInfo.forExample(package, example, execopts);
      info.run(package.name, show=show,
                showStr="Executing [%s] target: ".format(if release
                                                          then "release"
                                                          else "debug"),
                rebuildName="mason build --example " + example.name:string);
    }
  } else {
    const info = Execution.runInfo.forMainTarget(package, options, execopts);
    if build {
      log.debug("Building project before running");
      Compilation.buildProgram(package, options, force=force);
    }
    info.run(package.name, show=show,
             showStr="Executing [%s] target: ".format(if release
                                                        then "release"
                                                        else "debug"));
  }
}

}
