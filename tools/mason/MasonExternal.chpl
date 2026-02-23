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

use ArgumentParser;
use FileSystem;
use List;
use Map;
use MasonEnv;
use MasonHelp;
use MasonUtils;
use MasonLogger;
use Path;
use SpecParser;
use TOML;

var log = new logger("mason external");

// We could consider bumping this version up as needed.
const minSpackVersion = new versionInfo('1.0.0');
const major = minSpackVersion.major:string;
const minor = minSpackVersion.minor:string;
const spackBranch = 'releases/v' + '.'.join(major, minor);
const spackDefaultPath = MASON_HOME + "/spack";
const spackRegistryDefaultPath = MASON_HOME + "/spack-registry";

proc masonExternal(args: [] string) {

  var parser = new argumentParser(helpHandler=new MasonExternalHelpHandler());

  var subCmds = new map(string, shared Argument);

  // define all the supported subcommand strings here
  var cmds = ["search", "compiler", "install", "uninstall", "info", "find"];
  for cmd in cmds {
    subCmds.add(cmd,parser.addSubCommand(cmd));
  }

  var versionFlag = parser.addFlag(name="versionFlag",
                                   opts=["-V","--version"],
                                   defaultValue=false);

  var setupFlag = parser.addFlag(name="setup", defaultValue=false);
  var specFlag = parser.addFlag(name="spec", defaultValue=false);

  parser.parseArgs(args);

  if versionFlag.valueAsBool() {
    printSpackVersion();
    exit(0);
  }
  if specFlag.valueAsBool() {
    specHelp();
    exit(0);
  }

  try! {
    if setupFlag.valueAsBool() {
      // if MASON_OFFLINE is set, then cannot install spack
      if MASON_OFFLINE {
        throw new MasonError('Cannot setup Spack when MASON_OFFLINE ' +
                             'is set to true');
      }

      // If spack and spack registry is present with latest version, print
      if isDir(SPACK_ROOT) &&
         isDir(spackRegistryDefaultPath) &&
         getSpackVersion() == minSpackVersion &&
         SPACK_ROOT == spackDefaultPath {

        throw new owned MasonError("Spack backend is already installed");
      }

      // If both spack and spack registry not installed then setup spack
      if !isDir(SPACK_ROOT) && !isDir(spackRegistryDefaultPath) then
        setupSpack();

      // Engin: Why do we clone spack itself while creating our own registry?
      // If spack registry is not installed then install it
      if !isDir(spackRegistryDefaultPath) {
        log.infoln("Installing Spack Registry ...");
        const dest = spackRegistryDefaultPath;
        const branch = ' --branch releases/latest ';
        const status = cloneSpackRepository(branch, dest);
        if status != 0 then
          throw new owned MasonError("Spack registry installation failed.");
      } else {
        log.infof("Using existing Spack Registry at %s\n",
                  spackRegistryDefaultPath);
      }

      /* Engin: Unclear to me why we do this. We _definitely_ shouldn't do it if
         mason doesn't own the spack installation to say the least. I am not
         sure if mason should ever own its own spack either.
      // If spack is installed and version is outdated, update it
      if isDir(SPACK_ROOT) && getSpackVersion() != minSpackVersion {
        writeln("Updating Spack backend ... ");
        const status = updateSpackCommandLine();
        if isDir(MASON_HOME + '/spack-registry') then generateYAML();
        if status != 0 then throw new owned MasonError("Spack update failed.");
      }
       */

      // If spack is not installed then install it. Note that this is going to
      // be a spack installation owned by Mason, in that it will make it use its
      // own registry. I am not sure if we should do that vs. making spack a
      // mason dependency if a package has a spack-based dependency.
      if !isDir(SPACK_ROOT) {
        writeln("Installing Spack backend ... ");
        const spackLatestBranch = ' --branch ' + spackBranch + ' ';
        const status = cloneSpackRepository(spackLatestBranch, SPACK_ROOT);
        if isDir(spackRegistryDefaultPath) then generateYAML();
        if status != 0 then
          throw new owned MasonError("Spack installation failed.");
      }
      // check that after all this, the version of spack is as we expect it
      if getSpackVersion() <= minSpackVersion then
        throw new MasonError("Spack update or installation failed. " +
                             "Expected v%s, got v%s".format(
                                minSpackVersion.str(),
                                getSpackVersion().str()));
      exit(0);
    }
    if spackInstalled() {
      var usedCmd:string;
      var cmdList:list(string);
      // identify which, if any, subcommand was used and collect its arguments
      for (cmd, arg) in zip(subCmds.keys(), subCmds.values()) {
        if arg.hasValue() {
          usedCmd = cmd;
          cmdList = new list(arg.values());
          break;
        }
      }
      var cmdArgs = cmdList.toArray();
      select (usedCmd) {
        when "search" do searchSpkgs(cmdArgs);
        when "compiler" do compiler(cmdArgs);
        when "install" do installSpkg(cmdArgs);
        when "uninstall" do uninstallSpkg(cmdArgs);
        when "info" do spkgInfo(cmdArgs);
        when "find" do findSpkg(cmdArgs);
        otherwise {
          writeln('error: no such subcommand %s'.format(usedCmd));
          writeln('try mason external --help');
          exit(1);
        }
      }
    }
  } catch e: MasonError {
    writeln(e.message());
    exit(1);
  }
}

/* Prints Spack spec help menu */
private proc specHelp() {
  const command = "spack help --spec";
  const status = runSpackCommand(command);
}

/* Checks if updated spack and spack registry is installed*/
proc spackInstalled() throws {
  if !isDir(SPACK_ROOT) {
    throw new MasonError("To use mason external, call: mason external --setup");
  }
  if !isDir(getSpackRegistry) {
    throw new MasonError("Mason has been updated. To use mason external, "
                               + "call: mason external --setup");
  }
  // if local spack version is lower than required version
  if getSpackVersion() < minSpackVersion && SPACK_ROOT == spackDefaultPath {
    throw new MasonError("Mason has been updated and requires a newer " +
          "version of Spack (%s).".format(minSpackVersion.str()) +
          "\nTo use mason external, call: mason external --setup");
  }
  // if local version is a major or minor version higher than required version
  if !minSpackVersion.isCompatible(getSpackVersion()) {
    log.warnf("Your version of Spack (v%s) differs from that supported by " +
              "Mason (v%s).\nThis may lead to unexpected behavior",
              getSpackVersion().str(), minSpackVersion.str());
  }
  return true;
}

/* Spack installed to MASON_HOME/spack */
proc setupSpack() throws {
  writeln("Installing Spack backend ...");
  const destCLI = MASON_HOME + "/spack/";
  const spackLatestBranch = ' --branch v' + minSpackVersion.str() + ' ';
  const destPackages = MASON_HOME + "/spack-registry";
  const spackMasterBranch = ' --branch releases/latest ';
  const statusCLI = cloneSpackRepository(spackLatestBranch, destCLI);
  const statusPackages = cloneSpackRepository(spackMasterBranch, destPackages);
  generateYAML();
  if statusCLI != 0 && statusPackages != 0 {
    throw new owned MasonError("Spack installation failed");
  }
}

/* Clones the Spack repository */
proc cloneSpackRepository(branch : string, dest: string) {
  const repo = "https://github.com/spack/spack ";
  const depth = '--depth 1 ';
  const command = 'git clone -q -c advice.detachedHead=false ' +
                  branch + depth + repo + dest;
  const statusPackages = runWithStatus(command);
  if statusPackages != 0 then return -1;
  else return 0;
}

/* git checkout command run at SPACK_ROOT */
proc gitCheckOutSpack(tag: string) {
  const checkOutCommand = 'git ' + '-C ' + SPACK_ROOT +
                      ' checkout -q ' + tag;
  const status = runWithStatus(checkOutCommand);
  if status != 0 then return -1;
  else return 0;
}

/* git fetch command run at SPACK_ROOT */
proc gitFetch(branch: string) {
  const command = 'git ' + '-C ' + SPACK_ROOT + ' fetch -q ' +
                  ' --depth 1 origin ' + branch;
  const status = runWithStatus(command);
  if status != 0 then return -1;
  else return 0;
}

/* Updates the spack directory used for spack commands */
private proc updateSpackCommandLine() {
  const releaseTag = 'v' + minSpackVersion.str();
  var tag = 'refs/tags/' + releaseTag;
  tag = tag + ':' + tag;
  const statusFetch = gitFetch(tag);
  const statusCheckOut = gitCheckOutSpack(releaseTag);
  if statusFetch != 0 || statusCheckOut != 0 then return -1;
  else return 0;
}


// ENGIN: If you have your independent spack installation, this function
// basically hijacks it. This doesn't feel right.
/*
  Generate site-specific repos.yaml :-
  Replaces package repository path of repos.yaml file at
  /spack/etc/spack/defaults with that of spack-registry
*/
private proc generateYAML() {
  const yamlFilePath = SPACK_ROOT + '/etc/spack/defaults/repos.yaml';
  if isFile(yamlFilePath) {
    remove(yamlFilePath);
  }
  const reposOverride =
    'repos:\n'+
    '  - ' + MASON_HOME + '/spack-registry/var/spack/repos/builtin \n';
  var yamlFile = open(yamlFilePath,ioMode.cw);
  var yamlWriter = yamlFile.writer(locking=false);
  yamlWriter.write(reposOverride);
  yamlWriter.close();
}

/* Prints spack version */
private proc printSpackVersion() {
  const command = "spack --version";
  const version = runSpackCommand(command);
}

/* Returns spack version */
proc getSpackVersion(): versionInfo {
  const command = "spack --version";
  @functionStatic
  ref tmpVersion = getSpackResult(command,true).strip();
  // on systems with their own spack, spack --version can provide
  // a version string like x.x.x.xxxx (hash)
  // partitioning the string allows us to separate the major.minor.bug
  // from the remaining values
  const version = tmpVersion.partition(" ");
  return new versionInfo(version[0]);
}

/* Lists available spack packages */
private proc listSpkgs() {
  const command = "spack list";
  const status = runSpackCommand(command);
}

/* Queries spack for package existence */
private proc searchSpkgs(args: [] string) {
  var parser = new argumentParser(helpHandler=
                                  new MasonExternalSearchHelpHandler());

  var descFlag = parser.addFlag(name="description",
                                opts=["-d","--desc"],
                                defaultValue=false);
  var searchArg = parser.addArgument(name="searchString", numArgs=0..1);

  parser.parseArgs(args);

  var command = "spack list";
  var pkgName: string;
  if !searchArg.hasValue() {
    listSpkgs();
    exit(0);
  } else {
    pkgName = searchArg.value();
  }

  if descFlag.valueAsBool() {
    command = " ".join(command, "--search-description");
  }

  command = " ".join(command, pkgName);
  const status = runSpackCommand(command);
}

/* Lists all installed spack packages for user */
private proc listInstalled() {
  const command = "spack find";
  const status = runSpackCommand(command);
}

/* User facing function to show packages installed on
   system. Takes all spack arguments ex. -df <package> */
private proc findSpkg(args: [] string) {
  var parser = new argumentParser(helpHandler=
                                  new MasonExternalFindHelpHandler());

  var findArgs = parser.addArgument(name="package", numArgs=0..);

  parser.parseArgs(args);

  var command = "spack find";
  var findArray = findArgs.values();
  var packageWithArgs = " ".join(findArray);
  const status = runSpackCommand(" ".join(command, packageWithArgs));
}

/* Entry point into the various info subcommands */
private proc spkgInfo(args: [] string) {

  var parser = new argumentParser(helpHandler=
                                  new MasonExternalInfoHelpHandler());

  var archFlag = parser.addFlag(name="architecture",
                                opts=["--arch"],
                                defaultValue=false);
  // TODO: Argument parser may need support for mutually exclusive, or
  // required if other value, or not required if other value setups
  // but doesn't have them yet. As a workaround, look for 0 or 1 args here
  // to allow for processing arguments without a package arg
  var packageArg = parser.addArgument(name="package", numArgs=0..1);

  parser.parseArgs(args);

  if archFlag.valueAsBool() {
    printArch();
  } else if packageArg.hasValue() {
    var status = runSpackCommand("spack info " + packageArg.value());
  } else {
    masonExternalInfoHelp();
  }
}

/* Print system arch info */
private proc printArch() {
  const command = "spack arch";
  const status = runSpackCommand(command);
}


/* Queries system to see if package is installed on system */
proc spkgInstalled(spec: string) {
  const command = "spack find -df --show-full-compiler " + spec;
  const pkgInfo = getSpackResult(command, quiet=true);
  var found = false;
  var dependencies: [1..0] string; // a list of pkg dependencies
  for item in pkgInfo.split() {
    if item.rfind(spec) != -1 {
      return true;
    }
  }
  return false;
}


/* Entry point into the various compiler functions */
private proc compiler(args: [] string) {

  var parser = new argumentParser(helpHandler=new MasonCompilerHelpHandler());

  var listFlag = parser.addFlag(name="list", defaultValue=true);
  var findFlag = parser.addFlag(name="find", defaultValue=false);
  var editFlag = parser.addFlag(name="edit", defaultValue=false);
  var quietFlag = parser.addFlag(name="quiet", defaultValue=false);

  parser.parseArgs(args);

  if findFlag.valueAsBool() {
    findCompilers(quietFlag.valueAsBool());
  } else if editFlag.valueAsBool() {
    editCompilers();
  } else { // handle default when no flags passed or when --list passed
    listCompilers();
  }
}

/* Lists available compilers on system */
private proc listCompilers() {
  const command = "spack compilers";
  const status = runSpackCommand(command);
 }

/* Finds available compilers */
private proc findCompilers(quiet=false) {
  const command = "spack compiler find";
  const status = runSpackCommand(command, quiet);
}

/* Opens the compiler configuration file in $EDITOR */
private proc editCompilers() {
  const command = "spack config edit compilers";
  const status = runSpackCommand(command);
}


/*
  Given a toml of external dependencies returns
  the dependencies in a toml in lock file format

  Returns an associative array of tomls
*/
proc getExternalPackages(exDeps: Toml) {

  var exDom: domain(string, parSafe=false);
  var exDepTree: [exDom] shared Toml?;

  for (name, spc) in zip(exDeps.A.keys(), exDeps.A.values()) {
    try! {
      var spec = spc!;
      select spec.tag {
          when fieldtag.fieldToml do continue;
          otherwise {
            // Take key from toml file if not present in spec
            var fullSpec = spec.s;
            // TODO: Should it be an error if TOML value includes name?
            if !spec.s.startsWith(name) {
              fullSpec = "@".join(name, spec.s);
            }

            log.debugf("Dep %s: fullSpec: %s\n", name, fullSpec);

            const resolvedSpec = resolveSpec(fullSpec);

            log.debugf("Dep %s: resolvedSpec: %s\n", name, resolvedSpec);

            var dependencies = getSpkgDependencies(resolvedSpec);
            const pkgInfo = getSpkgInfo(resolvedSpec, dependencies);

            if !exDom.contains(name) then
              exDom += name;

            exDepTree[name] = pkgInfo;
          }
        }
    } catch e: MasonError {
      writeln(e.message());
      exit(1);
    }
  }
  return exDepTree;
}


/* Retrieves build information for MasonUpdate */
proc getSpkgInfo(spec: string, dependencies: list(string)): shared Toml throws {

  var depList: list(shared Toml);
  var spkgDom: domain(string, parSafe=false);
  var spkgToml: [spkgDom] shared Toml?;
  var spkgInfo = new shared Toml(spkgToml);

  try {
    const specFields = getSpecFields(spec);
    var pkgName = specFields[0];
    var version = specFields[1];
    var compiler = specFields[2];

    // Remove variants from spec
    var simpleSpec = pkgName + '@' + version + '%' + compiler;

    if spkgInstalled(simpleSpec) {
      const spkgPath = getSpkgPath(simpleSpec);
      const libs = joinPath(spkgPath, "lib");
      const includePath = joinPath(spkgPath, "include");
      const other = joinPath(spkgPath, "other");

      if isDir(other) {
        spkgInfo.set("other", other);
      }
      spkgInfo.set("name", pkgName);
      spkgInfo.set("version", version);
      spkgInfo.set("compiler", compiler);
      spkgInfo.set("libs", libs);
      spkgInfo.set("include", includePath);

      for dep in dependencies {
        var depSpec = dep.split("@", 1);
        var name = depSpec[0];

        // put dep into current packages dep list
        depList.pushBack(new shared Toml(name));

        // get dependencies of dep
        var depsOfDep = getSpkgDependencies(dep);

        // get a toml that contains the dependency info and put it
        // in a subtable of the current dependencies table
        spkgInfo.set(name, getSpkgInfo(dep, depsOfDep));
      }
      if depList.size > 0 {
        // Temporarily use toArray here to avoid supporting list.
        spkgInfo.set("dependencies", depList.toArray());
      }
    } else {
      throw new MasonError("No package installed by the name of: " + pkgName);
    }
  } catch e: MasonError {
    writeln(e.message());
  }
  return spkgInfo;
}

/* Returns spack package path for build information */
proc getSpkgPath(spec: string): string throws {
  const command = "spack location -i " + spec;
  const pkgPath = getSpackResult(command, quiet=true);
  if pkgPath == "" {
    throw new owned MasonError("Mason could not find " + spec);
  }
  return pkgPath.strip();
}

/* Find dependencies of package that are installed on machine */
proc getSpkgDependencies(spec: string): list(string) throws {
  const name = specName(spec);
  const command = "spack find -df --show-full-compiler " + spec;
  const pkgInfo = getSpackResult(command, quiet=true);
  var found = false;
  var dependencies: list(string);
  for item in pkgInfo.split() {
    log.debugf("Spack dependency %s\n", item);

    if item.rfind(name) != -1 {
      found = true;
      log.debugln("Found");
    } else if found {
      const dep = item.strip("^");
      log.debugf("Had found already, adding %s\n", dep);
      dependencies.pushBack(dep);
    }
  }
  if !found {
    throw new MasonError("Mason could not find dependency: " + spec);
  }
  return dependencies;
}


/* Get package name from spec */
private proc specName(spec: string): string throws {
  const fields = spec.split('%');
  const subfields = fields[0].split('@');
  const name = subfields[0];
  return name;
}


// ENGIN: not sure why we need this, this seems to be sensitive to changes to
// Spack syntax. All we need should be "spec"?
/* Resolve spec, pinning to the installed version and eliminating ranges */
private proc resolveSpec(spec: string): string throws {
  const command = "spack spec --no-install-status %s | head -n1".format(spec);
  const output = getSpackResult(command, quiet=true);

  if output == '' {
    throw new MasonError("Package not found: " + spec);
  }

  return output;
}


/* Install an external package */
proc installSpkg(args: [] string) throws {
  var parser = new argumentParser(helpHandler=new MasonInstallHelpHandler());

  var quietFlag = parser.addFlag(name="quiet", defaultValue=false);
  var specArg = parser.addArgument(name="SpackSpec", numArgs=0..);

  parser.parseArgs(args);

  var command = "spack install";
  var spec: string;

  if specArg.hasValue() {
    var specArr = specArg.values();
    if MASON_OFFLINE && ((+ reduce (specArr == "--update")) == 0) {
      writeln("Cannot install Spack packages when MASON_OFFLINE=true");
      return;
    }
    spec = " ".join(specArr);
  } else {
    masonInstallHelp();
    exit(1);
  }

  const status =
    runSpackCommand(" ".join(command, spec), quietFlag.valueAsBool());
  if status != 0 {
    throw new MasonError("Package could not be installed");
  }
}


/* Uninstall an external package */
proc uninstallSpkg(args: [] string) throws {

  var parser = new argumentParser(helpHandler=new MasonUninstallHelpHandler());

  var forceFlag = parser.addFlag(name="force", defaultValue=false);
  var allFlag = parser.addFlag(name="all", defaultValue=false);
  var depFlag = parser.addFlag(name="dependents", defaultValue=false);
  var pkgArg = parser.addArgument(name="package", numArgs=0..);

  parser.parseArgs(args);

  var pkgName: string;
  var command = "spack uninstall -y";
  var confirm: string;
  var uninstallArgs = "";

  if forceFlag.valueAsBool() then uninstallArgs += "--force ";
  if allFlag.valueAsBool() then uninstallArgs += "--all ";
  if depFlag.valueAsBool() then uninstallArgs += "--dependents ";
  if pkgArg.hasValue() {
    var pkgArr = pkgArg.values();
    pkgName = "".join(pkgArr);
  } else {
    masonUninstallHelp();
    exit(1);
  }

  writeln("Are you sure you want to uninstall " + pkgName +"? [y/n]");
  read(confirm);
  if confirm != "y" {
    writeln("Aborting...");
    exit(0);
  }

  const status = runSpackCommand(" ".join(command, uninstallArgs, pkgName));
  if status != 0 {
    throw new owned MasonError("Package could not be uninstalled");
  }
}
