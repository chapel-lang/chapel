module Compilation {

import Package;
import BuildInfo;
import BuildInfo.MasonPackage;
import MasonLogger;
import MasonUtils;
import MasonUtils.{MasonError, getLastModified};

import ThirdParty.Pathlib.path;

use IO;
use ThirdParty.Pathlib.IOHelpers;
import List.list;

private var log = MasonLogger.getLogger("compilation");

private iter projectFiles(package: borrowed Package.MasonPackage) throws {
  for file in (package.projectHome / "src").findFiles(recursive=true) do
    yield package.projectHome / "src" / file;
}
private iter exampleFiles(package: borrowed Package.MasonPackage,
                          example: Package.example) throws {
  for f in projectFiles(package) do yield f;
  yield package.projectHome / "examples" / example.name;
}

proc buildProgram(
  package: borrowed Package.MasonPackage, options: BuildInfo.buildOptions,
  force = false
) throws {
  const projectName = package.name;
  const projectHome = package.projectHome;
  const binLoc: path = if options.releaseMode then "release" else "debug";

  if shouldRebuild(package, binLoc, projectName, options,
                projectFiles(package), force) {

    // Make build files and check chapel version
    MasonUtils.makeTargetFiles(binLoc, projectHome);

    // Compile Program
    package.preBuild();
    const executable = package.getTargetExecutable(binLoc);
    const compopts = package.getBuildCmd(options);
    const cmd = getCompilationCmd(executable, compopts);
    log.infof("Compiling [%s] target: %s\n",
              if options.releaseMode then "release" else "debug", projectName);
    if compileSrc(executable, cmd) {
      log.info("Build Successful");
      package.postBuild();
    } else {
      const fpFile = package.getTargetFingerprintPath(binLoc);
      invalidateFingerprint(fpFile);
      throw new MasonError("Build Failed");
    }
  } else {
    log.info("Skipping Build... No changes to project");
  }
}

proc buildExamples(
  package: borrowed Package.MasonPackage, examples: list(string),
  options: BuildInfo.buildOptions, force = false
) throws {
  const projectHome = package.projectHome;
  const binLoc: path = if options.releaseMode then "release" else "debug";

  var examplesToBuild: list(Package.example);
  if examples.size > 0 {
    for example in examples {
      var idx = package.examples.find(example);
      if idx == -1 then
        throw new MasonError("Mason could not find example: " + example);
      examplesToBuild.pushBack(package.examples[idx]);
    }
  } else {
    examplesToBuild.pushBack(package.examples.these());
  }
  if examplesToBuild.size == 0 then
    throw new MasonError("No examples were found in /example");

  MasonUtils.makeTargetFiles(binLoc, projectHome);

  // TODO: ideally we only run the prebuild if examples need to be built,
  // but just moving it inside the loop will cause prebuild to run multiple times,
  // which is not great
  package.preBuild();
  for example in examplesToBuild {
    const plainName = example.plainName();
    const examplesSubdir: path = "examples";
    log.debug("Building example ", plainName, " at ", example.name);
    if shouldRebuild(package, examplesSubdir, plainName, options,
                  exampleFiles(package, example), force) {
      const executable =
        package.getTargetExecutable(examplesSubdir, plainName);
      const compopts = package.getBuildCmd(options, example);
      const cmd = getCompilationCmd(executable, compopts);
      log.info("Compiling example: ", example.name:string);
      if compileSrc(executable, cmd) {
        log.info("Compiled ", example.name:string, " successfully");
      } else {
        const fpFile =
          package.getTargetFingerprintPath(examplesSubdir, plainName);
        invalidateFingerprint(fpFile);
        throw new MasonError("Example build failed for example: " +
                              example.name:string);
      }
    } else {
      log.info("Skipping ", example.name:string,
                ": no changes made to project or example");
    }
  }
  package.postBuild();
}

proc getCompilationCmd(executable: path, compopts: list(string)): list(string) {
  var cmd: list(string);
  cmd.pushBack("chpl");
  cmd.pushBack(compopts);
  cmd.pushBack("-o");
  cmd.pushBack(executable:string);
  return cmd;
}

proc compileSrc(executable: path, cmd: list(string)): bool throws {
  if executable.isFile() then executable.remove();
  const command = cmd.toArray();
  log.debug("Compilation command: " + " ".join(command));
  var compilation = MasonUtils.runWithStatus(command);
  if compilation != 0 then
    return false;
  // Confirming File Structure
  return executable.isFile();
}


// TODO: ideally fingerprinting uses a checksum, but that requires adding
// a new dependency to mason (openssl) which is overkill for now
// proc sha256(s): string {
//   use Crypto;
//   var hasher = new Hash(Digest.SHA256);
//   var buffer = new CryptoBuffer(s);
//   var digest = hasher.getDigest(buffer);
//   return digest.toHexString();
// }

proc printChplEnv(): string {
  const printchplenv = MasonUtils.CHPL_HOME:path / "util" / "printchplenv";
  var output = "";
  try {
    output = MasonUtils.runCommand(
      [printchplenv:string, "--all", "--internal", "--simple"],
      quiet=true);
  } catch e {
    log.error("Could not run printchplenv to " +
              "get Chapel environment variables");
  }
  return output;
}
/*
  Returns the environment variables (and their values) which may affect
  the build process.

  This is not an exhaustive list of all environment variables which
  may affect the build process, but is intended to capture the most common
*/
proc getInterestingEnvVars(): string {
  const vars = ("CHPL_HOME",
                "CHPL_MODULE_PATH",
                "CHPL_CC_FLAGS", "CHPL_INCLUDE_PATH",
                "CHPL_LD_FLAGS", "CHPL_LIB_NAME", "CHPL_LIB_PATH");
  var output = "";
  for v in vars {
    const val = MasonUtils.getEnv(v);
    output += v + "=" + val + "\n";
  }
  return output;
}

proc computeFingerprint(options: BuildInfo.buildOptions): string throws {
  var fingerprint = "";
  fingerprint += "MasonVersion=" + MasonUtils.MASON_VERSION + "\n";
  fingerprint += "ChapelVersion=" +
                    MasonUtils.getChapelVersionInfo():string + "\n";
  fingerprint += printChplEnv();
  fingerprint += getInterestingEnvVars();
  fingerprint += options:string;
  // return sha256(fingerprint);
  return fingerprint;
}

/*
  Returns true if the fingerprint matches the stored fingerprint for the
  project, false otherwise. If no stored fingerprint exists, one is created.
*/
proc checkFingerprint(fingerprintFile: path,
                      fingerprint: string): bool throws {
  if !fingerprintFile.isFile() {
    if !fingerprintFile.parent.isDir() then
      fingerprintFile.parent.mkdir(parents=true);
    log.debug("No previous fingerprint found, creating new fingerprint");
    const writer = openWriter(fingerprintFile);
    writer.write(fingerprint);
    return false;
  } else {
    const reader = openReader(fingerprintFile);
    const old = reader.readAll(string);
    reader.close();
    if old != fingerprint {
      log.debug("Fingerprints do not match, rebuild required");
      // update fingerprint
      reader.close();
      const writer = openWriter(fingerprintFile);
      writer.write(fingerprint);
      return false;
    } else {
      log.debug("Fingerprints match, no rebuild required");
      return true;
    }
  }
}

proc invalidateFingerprint(fingerprintFile: path) throws {
  if fingerprintFile.isFile() then fingerprintFile.remove();
}

proc shouldRebuild(package: borrowed Package.MasonPackage,
                   targetSubdir: path, projectName: string,
                   options: BuildInfo.buildOptions, filesToCheck, force: bool
                  ): bool throws {
  // compute the fingerprint first so its always up-to-date,
  // even if we end up needing to rebuild due to file modifications
  const fingerprintFile =
    package.getTargetFingerprintPath(targetSubdir, projectName);
  const fingerprint = computeFingerprint(options);
  const fingerprintChanged = !checkFingerprint(fingerprintFile, fingerprint);

  if force then return true;

  // if no executable, rebuild
  const executable = package.getTargetExecutable(targetSubdir, projectName);
  if !executable.isFile() then return true;
  const executableModTime = getLastModified(executable);

  // check if the manifest or any of the filesToCheck have been
  // modified since the last build
  for file in filesToCheck {
    if getLastModified(file) > executableModTime then
      return true;
  }
  const manifestPath = package.projectHome / "Mason.toml";
  if getLastModified(manifestPath) > executableModTime then
    return true;

  // check the fingerprint
  if fingerprintChanged then return true;

  return false;
}
}
