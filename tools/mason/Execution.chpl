module Execution {

import Package;
import BuildInfo;
import MasonLogger;
import MasonUtils;
import MasonUtils.{MasonError, getLastModified};

import List.list;

import ThirdParty.Pathlib.path;

private var log = MasonLogger.getLogger("execution");

record runInfo {
  var executable: path;
  var nLocales: int;
  var execopts: list(string);
  var commandLineExecopts: list(string);

  iter getCmd(): string {
    yield executable:string;
    yield "-nl" + nLocales:string;
    for opt in execopts do yield opt;
    for opt in commandLineExecopts do yield opt;
  }

  proc run(targetName: string, show=false,
           showStr = "Executing target: ",
           rebuildName = "mason build"): int throws {
    var cmd = getCmd();
    if show then
      log.info(showStr, " ".join(cmd.these()));
    if !executable.isFile() then
      throw new MasonError(targetName + " does not seem to be built yet. Please run '" + rebuildName + "' to build it before running.");
    const runResult = MasonUtils.runWithStatus(cmd, capture=false);
    return runResult;
  }

  proc type forMainTarget(package: borrowed Package.MasonPackage,
                          options: BuildInfo.buildOptions,
                          commandLineExecopts: list(string)): runInfo {
    const subdir: path = if options.releaseMode then "release" else "debug";
    const executable = package.getTargetExecutable(subdir);
    const nLocales = 1;
    const execopts = new list(string);
    return new runInfo(executable, nLocales, execopts, commandLineExecopts);
  }
  proc type forExample(package: borrowed Package.MasonPackage,
                       example: Package.example,
                       commandLineExecopts: list(string)): runInfo {
    const subdir: path = "examples";
    const executable = package.getTargetExecutable(subdir, example.plainName());
    const nLocales = 1;
    const execopts = example.execopts;
    return new runInfo(executable, nLocales, execopts, commandLineExecopts);
  }
}

}
