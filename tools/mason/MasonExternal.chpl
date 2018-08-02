/*
 * Copyright 2004-2018 Cray Inc.
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


use MasonUtils;
use FileSystem;
use MasonHelp;
use MasonEnv;
use Path;
use TOML;

proc masonExternal(args: [] string) {
  try! {
    if args.size < 3 {
      masonExternalHelp();
      exit(0);
    }
    else if args[2] == "--setup" {
      setupSpack();
      exit(0);
    }
    if spackInstalled() {
      select (args[2]) {
        when 'search' do searchSpkgs(args);
        when 'compiler' do compiler(args);
        when 'install' do installSpkg(args);
        when 'uninstall' do installSpkg(args);
        when '--help' do masonExternalHelp();
        when '-h' do masonExternalHelp();
        when '--installed' do spkgOnSystem(args);
        when '--info' do spkgInfo(args);
        when '--arch' do printArch();
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


private proc spackInstalled() throws {
  const status = runWithStatus("spack");
  if status != 0 {
    throw new MasonError("To use `mason external` call mason external --setup");
  }
  return true;
}

private proc setupSpack() throws {
  const spackVers = "releases/v0.11.2";
  const destination = MASON_HOME + "/spack/";
  const clone = "git clone https://github.com/spack/spack " + destination;
  const checkout = "git checkout " + spackVers;
  const status = runWithStatus(clone);
  if status != 0 {
    throw new MasonError("Spack installation failed");
  }
  else {
    gitC(destination, checkout);
    setupSpackEnv();
  }
}


private proc setupSpackEnv() {
  const spackRoot = "'" + MASON_HOME + "/spack'";
  const setSpackRoot = "export SPACK_ROOT=" + spackRoot;
  const setPath = "export PATH=$SPACK_ROOT/bin:$PATH";
  const spackScript = ". $SPACK_ROOT/share/spack/setup-env.sh";
  var status = runCommand(setSpackRoot);
  status = runCommand(setPath);
  status = runCommand(spackScript);
  writeln("To avoid having to run --setup, put the following in your .bashrc,\n"
          + setSpackRoot + "\n"
          + setPath + "\n"
          + spackScript + "\n");
}

/* Queries spack for package existance */
proc spkgExists(spec: string) : bool {
  const command = "spack list " + spec;
  const status = runWithStatus(command);
  if status != 0 {
    return false;
  }
  return true;
}

/* lists available spack packages */
private proc listSpkgs() {
  const command = "spack list";
  const status = runCommand(command);
}

/* Queries spack for package existance */
//TODO: add --desc to search descriptions
private proc searchSpkgs(args: [?d] string) {
  if args.size < 4 {
    listSpkgs();
    exit(0);
  }
  else {
    if args[3] == "-h" || args[3] == "--help" {
      masonExternalSearchHelp();
      exit(0);
    }
    const pkgName = args[3];
    const command = "spack list " + pkgName;
    const status = runCommand(command);
  }
}

/* lists all installed spack packages for user */
private proc listInstalled() {
  const command = "spack find";
  const status = runCommand(command);
}

/* User facing function to show packages installed on
   system. Takes all spack arguments ex. -df <package> */
private proc spkgOnSystem(args: [?d] string) {
  if args.size < 3 {
    masonExternalHelp();
    exit(1);
  }
  else if args.size == 3 {
    listInstalled();
  }
  else {
    var command = "spack find";
    var packageWithArgs = " ".join(args[3..]);
    const status = runCommand(" ".join(command, packageWithArgs));
  }
}

/* User facing function to show info about a package */
private proc spkgInfo(args) {
  if args.size < 4 {
    masonExternalHelp();
    exit(1);
  }
  else if args[3].find('-') > 0 {
    masonExternalHelp();
    exit(1);
  }
  else {
    const command = "spack info";
    const pkgName = args[3];
    const status = runCommand(" ".join(command, pkgName));
  }
}

/* Queries system to see if package is installed on system */
private proc spkgInstalled(spec: string) {
  const command = "spack find -df " + spec;
  const pkgInfo = runCommand(command, quiet=true);
  var found = false;
  var dependencies: [1..0] string; // a list of pkg dependencies
  for item in pkgInfo.split() {  
    if item == spec {
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
  const status = runCommand(command);
 }

/* Finds available compilers */
private proc findCompilers() {
  const command = "spack compiler find";
  const status = runCommand(command);
}

/* Opens the compiler configuration file in $EDITOR */
private proc editCompilers() {
  const command = "spack config edit compilers";
  const status = runCommand(command);
}

/* Print system arch info */
private proc printArch() {
  const command = "spack arch";
  const status = runCommand(command);
}


/* Given a toml of external dependencies returns
   the dependencies in a toml */
proc getExternalPackages(exDeps: unmanaged Toml) {

  var exDom: domain(string);
  var exDepTree: [exDom] unmanaged Toml;

  for (name, spec) in zip(exDeps.D, exDeps.A) {
    try! {
      select spec.tag {
          when fieldToml do continue;
          otherwise {
            var dependencies = getSpkgDependencies(spec.s);
            const pkgInfo = getSpkgInfo(spec.s, dependencies);
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
proc getSpkgInfo(spec: string, dependencies: [?d] string) : unmanaged Toml throws {

  // put above try b/c compiler comlains about return value
  var depList: [1..0] unmanaged Toml;
  var spkgDom: domain(string);
  var spkgToml: [spkgDom] unmanaged Toml;
  var spkgInfo: unmanaged Toml = spkgToml;

  try {

    // TODO: create a parser for returning what the user has inputted in
    // terms of name, version, compiler etc..
    var split = spec.split("@");
    var pkgName = split[1];
    var versplit = split[2].split("%");
    var version = versplit[1];
    var compiler = versplit[2];

    if spkgInstalled(spec) {
      const spkgPath = getSpkgPath(spec);
      const libs = joinPath(spkgPath, "lib");
      const include = joinPath(spkgPath, "include");
      const other = joinPath(spkgPath, "other");

      if isDir(other) {
        spkgInfo["other"] = other;
      }
      spkgInfo["name"] = pkgName;
      spkgInfo["version"] = version;
      spkgInfo["compiler"] = compiler;
      spkgInfo["libs"] = libs;
      spkgInfo["include"] = include;

      while dependencies.domain.size > 0 {
        var dep = dependencies[dependencies.domain.first];
        var depSpec = dep.split("@");
        var name = depSpec[1];

        // put dep into current packages dep list
        depList.push_back(new unmanaged Toml(name));

        // get dependencies of dep
        var depsOfDep = getSpkgDependencies(dep);

        // get a toml that contains the dependency info and put it
        // in a subtable of the current dependencies table
        spkgInfo[name] = getSpkgInfo(dep, depsOfDep);

        // remove dep for recursion
        dependencies.remove(dependencies.domain.first);
      }
      if depList.domain.size > 0 {
        spkgInfo["dependencies"] = depList;
      }
    }
    else {
      throw new MasonError("No package installed by the name of: " + pkgName);
    }
  }
  catch e: MasonError {
    writeln(e.message());
  }
  return spkgInfo;
}

/* Returns spack package path for build information */
proc getSpkgPath(spec: string) throws {
  const command = "spack location -i " + spec;
  const pkgPath = runCommand(command, quiet=true);
  if pkgPath == "" {
    throw new MasonError("Mason could not find " + spec);
  }
  return pkgPath.strip();
}

proc getSpkgDependencies(spec: string) throws {
  const command = "spack find -df " + spec;
  const pkgInfo = runCommand(command, quiet=true);
  var found = false;
  var dependencies: [1..0] string; // a list of pkg dependencies
  for item in pkgInfo.split() {
    
    if item == spec {
      found = true;
    }
    else if found == true {
      const dep = item.strip("^");
      dependencies.push_back(dep); // format: pkg@version%compiler
    }
  }
  if !found {
    throw new MasonError("Mason could not find dependency: " + spec);
  }
  return dependencies;
}


/* Un/Install an external package */
proc installSpkg(args: [?d] string) throws {
  if args.size < 4 {
    masonUnInstallHelp();
    exit(1);
  }
  else {
    var command: string;
    if args[2] == "install" {
      command = "spack install";
    }
    else if args[2] == "uninstall" {
      command = "spack uninstall -y";
      var confirm: string;
      writeln("Are you sure you want to uninstall " + args[3] +"? [y/n]");
      read(confirm);
      if confirm != "y" {
        writeln("Aborting...");
        exit(0);
      }
    }
    else {
      masonUnInstallHelp();
      exit(1);
    }
    const pkgName = args[3];
    var toInstall = pkgName;
    var compiler = "";
    var version = "";
    

    // pkg and version
    if args.size == 5 {
      version = args[5];
      toInstall = "".join(pkgName, "@", version);
    }
    // pkg, version, and compiler
    else if args.size == 6 {
      version = args[5];
      compiler = args[6];
      toInstall = "".join(pkgName, "@", version, "%", compiler);
    }
    const status = runWithStatus(" ".join(command, toInstall));
    if status != 0 {
      throw new MasonError("Package could not be installed");
    }
  }
}
