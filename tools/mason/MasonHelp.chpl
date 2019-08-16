/*
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


/* A help module for the mason package manager */

use Help;
use MasonUtils;


proc masonHelp() {
  writeln("Chapel's package manager");
  writeln();
  writeln('Usage:');
  writeln('    mason <command> [<args>...]');
  writeln('    mason [options]');
  writeln();
  writeln('Options:');
  writeln('    -h, --help          Display this message');
  writeln('    -V, --version       Print version info and exit');
  writeln('    --list              List installed commands');
  writeln();
  writeln('Some common mason commands are (see all commands with --list):');
  writeln('    new         Create a new mason project');
  writeln('    add         Add a dependency to Mason.toml');
  writeln('    rm          Remove a dependency from Mason.toml');
  writeln('    update      Update/Generate Mason.lock');
  writeln('    build       Compile the current project');
  writeln('    run         Build and execute src/<project name>.chpl');
  writeln('    search      Search the registry for packages');
  writeln('    env         Print environment variables recognized by mason');
  writeln('    clean       Remove the target directory');
  writeln('    doc         Build this project\'s documentation');
  writeln('    system      Integrate with system packages found via pkg-config');
  writeln('    test        Compile and run tests found in /test');
  writeln('    external    Integrate external dependencies into mason packages');
  writeln('    publish     Publish package to mason-registry');
}

proc masonList() {
  writeln('Installed Mason Commands:');
  writeln('      add                ');
  writeln('      rm                 ');
  writeln('      new                ');
  writeln('      update             ');
  writeln('      build              ');
  writeln('      run                ');
  writeln('      test               ');
  writeln('      external           ');
  writeln('      search             ');
  writeln('      env                ');
  writeln('      clean              ');
  writeln('      doc                ');
  writeln('      help               ');
  writeln('      version            ');
  writeln('      system             ');
  writeln('      publish            ');
}


proc masonRunHelp() {
  writeln('Run the compiled project and output to standard output');
  writeln();
  writeln('Usage:');
  writeln('   mason run [options]');
  writeln();
  writeln('Options:');
  writeln('    -h, --help                   Display this message');
  writeln('        --build                  Compile before running binary');
  writeln('        --show                   Increase verbosity');
  writeln('        --example <example>      Run an example');
  writeln();
  writeln('When --example is thrown without an example, all available examples will be listed');
  writeln();
  writeln('When no options are provided, the following will take place:');
  writeln('   - Execute binary from mason project if target/ is present');
  writeln('   - If no target directory, build and run is Mason.toml is present');
  writeln();
  writeln('Runtime arguments can be included after mason arguments.');
  writeln('To ensure that runtime arguments and mason arguments to not conflict, separate them');
  writeln('with a single dash(`-`). For example');
  writeln('   e.g. mason run --build - --runtimeArg=true');
}

proc masonBuildHelp() {
  writeln('Compile a local package and all of its dependencies');
  writeln();
  writeln('Usage:');
  writeln('    mason build [options]');
  writeln();
  writeln('Options:');
  writeln('    -h, --help                   Display this message');
  writeln('        --show                   Increase verbosity');
  writeln('        --release                Compile to target/release with optimizations (--fast)');
  writeln('        --force                  Force Mason to build the project');
  writeln('        --example <example>      Build an example from the example/ directory');
  writeln('        --no-update              Do not update the mason registry before building');
  writeln();
  writeln('When --example is thrown without an example, all examples will be built');
  writeln('When no options are provided, the following will take place:');
  writeln('   - Build from mason project if Mason.lock present');
  writeln();
  writeln('Compilation flags and arguments can be included after mason arguments.');
  writeln('To ensure compilation flags and mason arguments to not conflict, separate them with a');
  writeln('single dash(`-`). For example');
  writeln('   e.g. mason build --force - --savec tmpdir');
}

proc masonNewHelp() {
  writeln('Usage:');
  writeln('    mason new [options] <project name>');
  writeln();
  writeln('Options:');
  writeln('    -h, --help                   Display this message');
  writeln('        --show                   Increase verbosity');
  writeln('        --no-vcs                 Do not initialize a git repository');
}

proc masonSearchHelp() {
  const desc =
"When no query is provided, all packages in the registry will be listed. The\n" +
"query will be used in a case-insensitive search of all packages in the\n" +
"registry.\n" +
"\n" +
"Packages will be listed regardless of their chplVersion compatibility.";

  writeln("Search the registry for a package");
  writeln();
  writeln("Usage:");
  writeln("    mason search [options] <query>");
  if developerMode {
    writeln('        --debug                 Print debug information');
  }
  writeln();
  writeln("Options:");
  writeln("    -h, --help                  Display this message");
  writeln("    --show                      Display the manifest of a package");
  writeln();
  writeln(desc);
}

proc masonModifyHelp() {
  writeln("Modify a Mason package's Mason.toml");
  writeln();
  writeln("Usage:");
  writeln("    mason rm [options] package");
  writeln("    mason add [options] package@version");
  writeln();
  writeln("Options:");
  writeln("    -h, --help                  Display this message");
  writeln("        --external              Add/Remove dependency from external dependencies");
  writeln("        --system                Add/Remove dependency from system dependencies");
  writeln();
  writeln("Not listing an option will add/remove the dependency from the Mason [dependencies] table");
  writeln("Versions are necessary for adding dependencies, but not for removing dependencies");
  writeln("Manually edit the Mason.toml if multiple versions of the same package are listed");
  writeln("Package names and versions are not validated upon adding");
}

proc masonEnvHelp() {
  writeln("Print environment variables recognized by mason");
  writeln();
  writeln("Usage:");
  writeln("    mason env [options]");
  writeln();
  writeln("Options:");
  writeln("    -h, --help                  Display this message");
  writeln();
  writeln("Environment variables set by the user will be printed with an");
  writeln("asterisk at the end of the line.");
}

proc masonExternalHelp() {
  writeln("Use, install and search for external packages to build Mason packages with");
  writeln();
  writeln("Usage:");
  writeln('    mason external [options] [<args>...]');
  writeln('    mason external [options]');
  writeln();
  writeln("Options:");
  writeln("    search                      Search for a specific external package");
  writeln("    compiler                    List and search for compilers on system");
  writeln("    install                     Install an external package");
  writeln("    uninstall                   Uninstall an external package");
  writeln("    info                        Show information about an external package");
  writeln("    find                        Find information about installed external packages");
  writeln("    -h, --help                  Display this message");
  writeln("        --setup                 Download and install Spack backend");
  writeln("        --spec                  Display Spack specification help");
  writeln();
  writeln("Please see Mason documentation for more instructions on using external packages");
}

proc masonExternalFindHelp() {
  writeln("Find external packages on your system and get information about them");
  writeln();
  writeln("Usage:");
  writeln("    mason external find [options]");
  writeln("    mason external find [options] <package>");
  writeln();
  writeln("    <package>: a Spack spec expression indicating the package to find");
  writeln();
  writeln("Options:");
  writeln("    -h, --help                  Display this message");
  writeln();
  writeln("Display Options:");
  writeln("    -s, --short                 Show only specs (default)");
  writeln("    -p, --paths                 Show paths to package install directories");
  writeln("    -d, --deps                  Show full dependency DAG of installed packages");
  writeln("    -l, --long                  Show dependency hashes as well as versions");
  writeln("    -L, --very-long             Show full dependency hashes as well as versions");
  writeln("    -t TAGS, --tags TAGS        Filter a package query by tags");
  writeln("        --show-flags            Show spec compiler flags");
  writeln("        --show-full-compiler    Show full compiler specs");
  writeln("        --variants              Show variants in output (can be long)");
  writeln("    -e, --explicit              Show only specs that were installed explicitly");
  writeln("    -E, --implicit              Show only specs that were installed as dependencies");
  writeln("    -u, --unknown               Show only specs Spack does not have a package for");
  writeln("    -m, --missing               Show missing dependencies as well as installed specs");
  writeln("    -M, --only-missing          Show only missing dependencies");
  writeln("    -N, --namespace             Show fully qualified package names");
  writeln();
  writeln("When no package is listed, all installed external packages will be listed.");
}

proc masonExternalInfoHelp() {
  writeln("Get information about external packages and system architecture");
  writeln();
  writeln("Usage:");
  writeln("    mason external info [options] <package>");
  writeln();
  writeln("    <package>: a Spack spec expression indicating the package to retrieve information on");
  writeln();
  writeln("Options:");
  writeln("    -h, --help                  Display this message");
  writeln("        --arch                  Print architecture information about current system");
  writeln();
}

proc masonExternalSearchHelp() {
  writeln("Search for external packages");
  writeln();
  writeln("Usage:");
  writeln("    mason external search [options] <search string>");
  writeln();
  writeln("Options:");
  writeln("    -h, --help                  Display this message");
  writeln("    -d, --desc                  Parse descriptions of package to include more search results");
  writeln();
}

proc masonInstallHelp() {
  writeln("Install external packages onto your system");
  writeln();
  writeln("Usage:");
  writeln("    mason external install [options] <package> ");
  writeln();
  writeln("    <package>: a Spack spec expression indicating the package to install");
  writeln();
  writeln("Options:");
  writeln("    -h, --help                     Display this message");
  writeln("    --only {package,dependencies}  Select the mode of installation. the default is to");
  writeln("                                   install the package along with all its dependencies.");
  writeln("                                   alternatively one can decide to install only the");
  writeln("                                   package or only the dependencies");
  writeln("    --jobs JOBS                    Explicitly set number of make jobs. default is #cpus");
  writeln("    --overwrite                    reinstall an existing spec, even if it has dependents");
  writeln("    --keep-prefix                  Don't remove the install prefix if installation fails");
  writeln("    --keep-stage                   Don't remove the build stage if installation succeeds");
  writeln("    --restage                      If a partial install is detected, delete prior state");
  writeln("    --use-cache                    Check for pre-built packages in mirrors");
  writeln("    --show-log-on-error            Print full build log to stderr if build fails");
  writeln("    --source                       Install source files in prefix");
  writeln("    --no-checksum                  Do not check packages against checksum");
  // writeln("    -v, --verbose               Display verbose build output while installing"); #10622
  writeln("    --fake                         Fake install for debug purposes.");
  writeln("    --file                         Install from file. Read specs to install from .yaml");
  writeln("    --clean                        Sanitize the environment from variables that can");
  writeln("                                   affect how packages find libraries or headers");
  writeln("    --dirty                        Maintain the current environment without trying to");
  writeln("                                   sanitize it");
  writeln("    --test {root,all}              If 'root' is chosen, run package tests during");
  writeln("                                   installation for top-level packages (but skip tests");
  writeln("                                   for dependencies). if 'all' is chosen, run package");
  writeln("                                   tests during installation for all packages. If neither");
  writeln("                                   are chosen, don't run tests for any packages.");
  writeln("    --run-tests                    Run package tests during installation (same as --test=all)");
  writeln("    --log-format {junit}           Format to be used for log files");
  writeln("    --log-file LOG_FILE            Filename for the log file. if not passed a default will be used");
  writeln("    --yes-to-all                   Assume 'yes' is the answer to every confirmation request");
  writeln();
  writeln("External Mason packages can be installed as follows:");
  writeln("    mason external install <full Spack spec expression>");
  writeln();
}

proc masonUninstallHelp() {
  writeln("Uninstall external packages on your system");
  writeln();
  writeln("Usage:");
  writeln("    mason external uninstall [options] <package> ");
  writeln();
  writeln("    <package>: a Spack spec expression indicating the package to install");
  writeln();
  writeln("Options:");
  writeln("    -h, --help                  Display this message");
  writeln("        --force                 Remove regardless of dependents");
  writeln("        --all                   USE CAREFULLY. remove ALL installed packages that match supplied spec");
  writeln("        --dependents            Also uninstall any dependent packages");
  writeln();
  writeln("External Mason packages can be uninstalled as follows:");
  writeln("    mason external uninstall <full Spack spec expression>");
  writeln();
}

proc masonCompilerHelp() {
  writeln("Find and view compilers on your system");
  writeln();
  writeln("Usage:");
  writeln("    mason external compiler [options]");
  writeln();
  writeln("Options:");
  writeln("    -h, --help                  Display this message");
  writeln("        --find                  Find compilers on your system");
  writeln("        --edit                  Open the compilers configuration file in $EDITOR");
  writeln("        --list                  List the compilers on your system");
  writeln();
 }

proc masonTestHelp() {
  writeln("Run test files located within target/debug/test");
  writeln();
  writeln("Usage:");
  writeln("    mason test [options]");
  writeln();
  writeln("Options:");
  writeln("    -h, --help                  Display this message");
  writeln("        --show                  Direct output of tests to stdout");
  writeln("        --no-run                Compile tests without running them");
  writeln("        --parallel              Run tests in parallel(sequential by default)");
  writeln();
  writeln("Test configuration is up to the user");
  writeln("Tests pass if they exit with status code 0");
}

proc masonSystemHelp() {
  writeln("Integrate a Mason package with system packages found via pkg-config");
  writeln();
  writeln("Usage:");
  writeln('    mason system [options] [<args>...]');
  writeln("    mason system [options]");
  writeln();
  writeln("Options:");
  writeln("    pc                          Print a system package's .pc file");
  writeln("    search                      Search all packages available on the system");
  writeln("    -h, --help                  Display this message");
  writeln();
  writeln("The pc command sometimes has trouble finding a .pc file if the file is named ");
  writeln("something other than <package name>.pc  Use -i to ensure package exists");
  writeln("For more information on using system dependencies see Mason documentation");
}

proc masonSystemSearchHelp() {
  writeln("Search for packages on system found via pkg-config");
  writeln();
  writeln("Usage:");
  writeln("    mason search [options]");
  writeln();
  writeln("Options:");
  writeln("    -h, --help                  Display this message");
  writeln("        --no-show-desc          Only display package name");
  writeln("        --desc                  Parse descriptions of package to include more search results");
  writeln();
}

proc masonSystemPcHelp() {
  writeln("Print a package's .pc file (pkg-config file)");
  writeln();
  writeln("Usage:");
  writeln("    mason pc [options]");
  writeln();
  writeln("Options:");
  writeln("    -h, --help                  Display this message");
  writeln();
}

proc masonCleanHelp() {
  writeln("Cleans the target directory of the mason directory");
  writeln();
  writeln("Usage:");
  writeln("    mason clean [options]");
  writeln();
  writeln("Options:");
  writeln("    -h, --help                  Display this message");
  writeln();
}


proc masonPublishHelp(){
  writeln("Publish a package to the mason-registry repository");
  writeln();
  writeln("Usage:");
  writeln("    mason publish [options] <registry>");
  writeln();
  writeln("Options:");
  writeln("    -h, --help                   Display this message");
  writeln('    --dry-run                    Check to see if package is ready to be published');
  writeln('    <registry>                   Positional argument indicates the target registry. Defaults to chapel-lang/mason-registry');
  writeln();
  writeln('Publishing requires the mason-registry to be forked and the package to have a remote origin.');
}


proc masonDocHelp() {
  writeln("Generate documentation for a mason package using chpldoc");
  writeln();
  writeln('Usage:');
  writeln('    mason doc [options]');
  writeln();
  writeln('Options:');
  writeln("    -h, --help                  Display this message");
  writeln();
  writeln("Will generate documentation when ran inside a mason package.");
  writeln("Requires that chpldoc is set up in order to work.");
  writeln("For instructions on setting up chpldoc, please view its documentation.");
  writeln();
}
