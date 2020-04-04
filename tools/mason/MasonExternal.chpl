/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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

config const spackVersion = "releases/v0.11.2";

private use List;
private use Map;
use MasonUtils;
use FileSystem;
use MasonHelp;
use SpecParser;
use MasonEnv;
use Path;
use TOML;

proc masonExternal(args: [] string) {
  try! {
    if args.size < 3 {
      masonExternalHelp();
      exit(0);
    }
    else if args[2] == "-h" || args[2] == "--help" {
      masonExternalHelp();
      exit(0);
    }
    else if args[2] == "--setup" {
      if isDir(SPACK_ROOT) then
        throw new owned MasonError("Spack backend is already installed");
      else if MASON_OFFLINE then
        throw new owned MasonError('Cannot setup Spack when MASON_OFFLINE is set to true');
      else {
        setupSpack();
        exit(0);
      }
    }
    if spackInstalled() {
      select (args[2]) {
        when 'search' do searchSpkgs(args);
        when 'compiler' do compiler(args);
        when 'install' do installSpkg(args);
        when 'uninstall' do uninstallSpkg(args);
        when 'info' do spkgInfo(args);
        when 'find' do findSpkg(args);
        when '--spec' do specHelp();
        otherwise {
          writeln('error: no such subcommand');
          writeln('try mason external --help');
          exit(1);
        }
      }
    }
  }
  catch e: MasonError {
    writeln(e.message());
    exit(1);
  }
}

/* Prints Spack spec help menu */
private proc specHelp() {
  const command = "spack help --spec";
  const status = runSpackCommand(command);
}

private proc spackInstalled() throws {
  if !isDir(SPACK_ROOT) {
    throw new owned MasonError("To use `mason external` call `mason external --setup`");
  }
  return true;
}

/* Spack installed to MASON_HOME/spack */
proc setupSpack() throws {
  writeln("Installing Spack backend ...");
  const destination = MASON_HOME + "/spack/";
  const clone = "git clone -q https://github.com/spack/spack " + destination;
  const checkout = "git checkout -q " + spackVersion;
  const status = runWithStatus(clone);
  gitC(destination, checkout);
  if status != 0 {
    throw new owned MasonError("Spack installation failed");
  }
}


/* lists available spack packages */
private proc listSpkgs() {
  const command = "spack list";
  const status = runSpackCommand(command);
}

/* Queries spack for package existence */
private proc searchSpkgs(args: [?d] string) {
  if args.size < 4 {
    listSpkgs();
    exit(0);
  }
  else {
    var command = "spack list";
    var pkgName: string;
    if args[3].find('-') != -1 {
      for arg in args[3..] {
        if arg.find('h') != -1 {
          masonExternalSearchHelp();
          exit(0);
        }
      }
    }
    if args[3] == "-d" || args[3] == "--desc" {
      command = " ".join(command, "--search-description");
      pkgName = args[4];
    }
    else {
      pkgName = args[3];
    }
    command = " ".join(command, pkgName);
    const status = runSpackCommand(command);
  }
}

/* lists all installed spack packages for user */
private proc listInstalled() {
  const command = "spack find";
  const status = runSpackCommand(command);
}

/* User facing function to show packages installed on
   system. Takes all spack arguments ex. -df <package> */
private proc findSpkg(args: [?d] string) {
  if args.size == 3 {
    listInstalled();
    exit(0);
  }
  if args[3].find('-') != -1 {
    for arg in args[3..] {
      if arg == "-h" || arg == "--help" {
        masonExternalFindHelp();
        exit(0);
      }
    }
  }
  var command = "spack find";
  var packageWithArgs = " ".join(args[3..]);
  const status = runSpackCommand(" ".join(command, packageWithArgs));
}

/* Entry point into the various info subcommands */
private proc spkgInfo(args: [?d] string) {
  var option = "--help";
  if args.size < 4 {
    masonExternalInfoHelp();
    exit(1);
  }
  else {
    option = args[3];
  }
  select option {
      when "--arch" do printArch();
      when "--help" do masonExternalInfoHelp();
      when "-h" do masonExternalInfoHelp();
      otherwise {
        var status = runSpackCommand("spack info " + option);
      }
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
private proc compiler(args: [?d] string) {
  var option = "list";
  if args.size > 3 {
    option = args[3];
  }
  select option {
      when "--list" do listCompilers();
      when "--find" do findCompilers();
      when "--edit" do editCompilers();
      otherwise do masonCompilerHelp();
    }
}

/* lists available compilers on system */
private proc listCompilers() {
  const command = "spack compilers";
  const status = runSpackCommand(command);
 }

/* Finds available compilers */
private proc findCompilers() {
  const command = "spack compiler find";
  const status = runSpackCommand(command);
}

/* Opens the compiler configuration file in $EDITOR */
private proc editCompilers() {
  const command = "spack config edit compilers";
  const status = runSpackCommand(command);
}


/* Given a toml of external dependencies returns
   the dependencies in a toml in lock file format */
proc getExternalPackages(exDeps: unmanaged Toml) /* [domain(string)] unmanaged Toml? */ {

  var exDom: domain(string);
  var exDepTree: [exDom] unmanaged Toml?;

  for (name, spc) in exDeps.A.items() {
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

            const resolvedSpec = resolveSpec(fullSpec);

            var dependencies = getSpkgDependencies(resolvedSpec);
            const pkgInfo = getSpkgInfo(resolvedSpec, dependencies);

            if !exDom.contains(name) then
              exDom += name;

            exDepTree[name] = pkgInfo;
          }
        }
    }
    catch e: MasonError {
      writeln(e.message());
      exit(1);
    }
  }
  return exDepTree;
}


/* Retrieves build information for MasonUpdate */
proc getSpkgInfo(spec: string, ref dependencies: list(string)): unmanaged Toml throws {

  var depList: list(unmanaged Toml);
  var spkgDom: domain(string);
  var spkgToml: [spkgDom] unmanaged Toml?;
  var spkgInfo = new unmanaged Toml(spkgToml);

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

      while dependencies.size > 0 {
        var dep = dependencies[1];
        var depSpec = dep.split("@", 1);
        var name = depSpec[0];

        // put dep into current packages dep list
        depList.append(new unmanaged Toml(name));

        // get dependencies of dep
        var depsOfDep = getSpkgDependencies(dep);

        // get a toml that contains the dependency info and put it
        // in a subtable of the current dependencies table
        spkgInfo.set(name, getSpkgInfo(dep, depsOfDep));

        // remove dep for recursion
        dependencies.pop(1);
      }
      if depList.size > 0 {
        // Temporarily use toArray here to avoid supporting list.
        spkgInfo.set("dependencies", depList.toArray());
      }
    }
    else {
      throw new owned MasonError("No package installed by the name of: " + pkgName);
    }
  }
  catch e: MasonError {
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

    if item.rfind(name) != -1 {
      found = true;
    }
    else if found {
      const dep = item.strip("^");
      dependencies.append(dep);
    }
  }
  if !found {
    throw new owned MasonError("Mason could not find dependency: " + spec);
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


/* Resolve spec, pinning to the installed version and eliminating ranges */
private proc resolveSpec(spec: string): string throws {
  const command = "spack spec %s".format(spec);
  const output = getSpackResult(command, quiet=true);
  var lines = output.split('\n');

  // Package on 7th line
  var ret = lines[6].strip();

  if ret == '' {
    throw new owned MasonError("Package not found: " + spec);
  }

  return ret;
}


/* Install an external package */
proc installSpkg(args: [?d] string) throws {
  if hasOptions(args, '-h', '--help') {
    masonInstallHelp();
    exit(1);
  }

  if MASON_OFFLINE && args.count('--update') == 0 {
    writeln('Cannot install Spack packages when MASON_OFFLINE=true');
    return;
  }

  if args.size < 4 {
    masonInstallHelp();
    exit(1);
  }
  else {
    var command = "spack install";
    var spec: string;
    if args[3] == "-h" || args[3] == "--help" {
      masonInstallHelp();
      exit(1);
    }
    else {
      spec = " ".join(args[3..]);
    }

    const status = runSpackCommand(" ".join(command, spec));
    if status != 0 {
      throw new owned MasonError("Package could not be installed");
    }
  }
}


/* Uninstall an external package */
proc uninstallSpkg(args: [?d] string) throws {
  if args.size < 4 {
    masonUninstallHelp();
    exit(1);
  }
  else {
    var pkgName: string;
    var command = "spack uninstall -y";
    var confirm: string;
    var uninstallArgs = "";
    if args[3] == "-h" || args[3] == "--help" {
      masonUninstallHelp();
      exit(1);
    }
    else if args[3].startsWith("-") > 0 {
      for arg in args[3..] {
        if arg.startsWith("-") {
          uninstallArgs = " ".join(uninstallArgs, arg);
        }
        else {
          pkgName = "".join(pkgName, arg);
        }
      }
    }
    else {
      pkgName = "".join(args[3..]);
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
}


