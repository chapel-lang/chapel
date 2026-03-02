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


/* A help module for the mason package manager */

use ArgumentParser;
use Help;
use MasonUtils;


class MasonHelpHandler : HelpHandler {
  override proc printHelp() {
    masonHelp();
  }
}

class MasonRunHelpHandler : HelpHandler {
  override proc printHelp() {
    masonRunHelp();
  }
}

class MasonBuildHelpHandler : HelpHandler {
  override proc printHelp() {
    masonBuildHelp();
  }
}

class MasonNewHelpHandler : HelpHandler {
  override proc printHelp() {
    masonNewHelp();
  }
}

class MasonInitHelpHandler : HelpHandler {
  override proc printHelp() {
    masonInitHelp();
  }
}

class MasonSearchHelpHandler : HelpHandler {
  override proc printHelp() {
    masonSearchHelp();
  }
}

class MasonModifyHelpHandler : HelpHandler {
  override proc printHelp() {
    masonModifyHelp();
  }
}

class MasonUpdateHelpHandler : HelpHandler {
  override proc printHelp() {
    masonUpdateHelp();
  }
}

class MasonEnvHelpHandler : HelpHandler {
  override proc printHelp() {
    masonEnvHelp();
  }
}

class MasonExternalHelpHandler : HelpHandler {
  override proc printHelp() {
    masonExternalHelp();
  }
}

class MasonExternalFindHelpHandler : HelpHandler {
  override proc printHelp() {
    masonExternalFindHelp();
  }
}

class MasonExternalInfoHelpHandler : HelpHandler {
  override proc printHelp() {
    masonExternalInfoHelp();
  }
}

class MasonExternalSearchHelpHandler : HelpHandler {
  override proc printHelp() {
    masonExternalSearchHelp();
  }
}

class MasonInstallHelpHandler : HelpHandler {
  override proc printHelp() {
    masonInstallHelp();
  }
}

class MasonUninstallHelpHandler : HelpHandler {
  override proc printHelp() {
    masonUninstallHelp();
  }
}

class MasonCompilerHelpHandler : HelpHandler {
  override proc printHelp() {
    masonCompilerHelp();
  }
}

class MasonTestHelpHandler : HelpHandler {
  override proc printHelp() {
    masonTestHelp();
  }
}

class MasonSystemHelpHandler : HelpHandler {
  override proc printHelp() {
    masonSystemHelp();
  }
}

class MasonSystemSearchHelpHandler : HelpHandler {
  override proc printHelp() {
    masonSystemSearchHelp();
  }
}

class MasonSystemPcHelpHandler : HelpHandler {
  override proc printHelp() {
    masonSystemPcHelp();
  }
}

class MasonCleanHelpHandler : HelpHandler {
  override proc printHelp() {
    masonCleanHelp();
  }
}

class MasonPublishHelpHandler : HelpHandler {
  override proc printHelp() {
    masonPublishHelp();
  }
}

class MasonDocHelpHandler : HelpHandler {
  override proc printHelp() {
    masonDocHelp();
  }
}

class MasonModulesHelpHandler : HelpHandler {
  override proc printHelp() {
    masonModulesHelp();
  }
}

proc masonHelp() {
  const s = """
  Chapel's package manager

  Usage:
      mason <command> [<args>...]
      mason [options]

  Options:
      -h, --help          Display this message
      -V, --version       Print version info and exit
          --no-color      Do not format text printed on console

  Mason commands:
      new         Create a new mason project
      init        Initialize a mason project inside an existing directory
      add         Add a dependency to Mason.toml
      rm          Remove a dependency from Mason.toml
      update      Update/Generate Mason.lock
      build       Compile the current project
      run         Build and execute src/<project name>.chpl
      search      Search the registry for packages
      env         Print environment variables recognized by mason
      clean       Remove the target directory
      doc         Build this project\'s documentation
      system      Integrate with system packages found via pkg-config
      test        Compile and run tests found in /test
      external    Integrate external dependencies into mason packages
      publish     Publish package to mason-registry
      modules     Print flags for including mason dependencies from TOML file
  """.dedent().strip();
  writeln(s);
}

proc masonRunHelp() {
  const s = """
  Run the compiled project and output to standard output

  Usage:
     mason run [options]

  Options:
      -h, --help                   Display this message
          --build                  Compile before running binary
          --release                Run the release version of the binary
          --show                   Increase verbosity
          --example <example>      Run an example

  When --example is thrown without an example, all available examples
  will be listed

  Runtime arguments can be included after mason arguments.
  You must separate runtime arguments and mason arguments with a
  double dash (`--`). All arguments after the double dash are passed
  directly to the executable.
  For example
     mason run --build -- --runtimeArg=true
  """.dedent().strip();
  writeln(s);
}

proc masonBuildHelp() {
  const s = """
  Compile a local package and all of its dependencies

  Usage:
      mason build [options]

  Options:
      -h, --help                   Display this message
          --show                   Increase verbosity
          --release                Compile to target/release with optimizations
                                   (--fast)
          --force                  Force Mason to build the project
          --example <example>      Build an example from the example/ directory
          --[no-]update            [Do not] update the mason registry before
                                   building

  When --example is thrown without an example, all available examples
  will be built

  Compilation flags and arguments can be included after mason arguments.
  You must separate compilation flags and mason arguments with a
  double dash (`--`). All arguments after the double dash are passed
  directly to the compiler.
  For example
     mason build --force -- --savec tmpdir
  """.dedent().strip();
  writeln(s);
}

proc masonNewHelp() {
  const s = """
  Creates a new Mason project in a new directory with the given project name.

  Usage:
      mason new [options] <project name>

  Options:
      -h, --help                   Display this message
          --show                   Increase verbosity
          --no-vcs                 Do not initialize a git repository
          --app                    Create a Mason "application"
          --lib                    Create a Mason "library"
          --light                  Create a Mason "lightweight" project
          --name <legalName>       Specify package name different from directory
                                   name
  """.dedent().strip();
  writeln(s);
}

proc masonInitHelp() {
  const s = """
  Initializes a Mason project inside a given directory or path.

  Usage:
      mason init [options] <directory name>
      mason init [options]

  Options:
      -h, --help                   Display this message
          --show                   Increase verbosity
          --no-vcs                 Do not initialize a git repository
          --app                    Create a Mason "application"
          --lib                    Create a Mason "library"
          --light                  Create a Mason "lightweight" project
          --name <legalName>       Specify package name different from directory
                                   name
  """.dedent().strip();
  writeln(s);
}

proc masonSearchHelp() {

  const s = """
  Search the registry for a package

  Usage:
      mason search [options] <query>

  Options:
      -h, --help                  Display this message
          --show                  Display the manifest of a package
          --[no-]update           [Do not] update the mason registry before
                                  searching the registry

  When no query is provided, all packages in the registry will be listed. The
  query will be used in a case-insensitive search of all packages in the
  registry.

  Packages will be listed regardless of their chplVersion compatibility.
  """.dedent().strip();
  writeln(s);
}

proc masonModifyHelp() {
  const s = """
Modify a Mason package's Mason.toml

Usage:
    mason rm [options] package
    mason add [options] package@version

Options:
    -h, --help                  Display this message
        --external              Add/Remove dependency from external dependencies
        --system                Add/Remove dependency from system dependencies

Not listing an option will add/remove the dependency from the Mason
[dependencies] table
Versions are necessary for adding dependencies, but not for removing
dependencies
Manually edit the Mason.toml if multiple versions of the same package are listed
Package names and versions are not validated upon adding
  """.dedent().strip();
  writeln(s);
}

proc masonUpdateHelp() {
  const s = """
  Update registries and generate Mason.lock

  Usage:
      mason update [options]

  Options:
      -h, --help                  Display this message
          --[no-]update           [Do not] update the mason registry before
                                  generating the lock file
          --force                 Force an update to the cached mason registry
  """.dedent().strip();
  writeln(s);
}

proc masonEnvHelp() {
  const s = """
  Print environment variables recognized by mason

  Usage:
      mason env [options]

  Options:
      -h, --help                  Display this message

  Environment variables set by the user will be printed with an
  asterisk at the end of the line.
  """.dedent().strip();
  writeln(s);
}

proc masonExternalHelp() {
  const s = """
  Use, install and search for external packages to build Mason packages with

  Usage:
      mason external [options] [<args>...]
      mason external [options]

  Options:
      search                      Search for a specific external package
      compiler                    List and search for compilers on system
      install                     Install an external package
      uninstall                   Uninstall an external package
      info                        Show information about an external package
      find                        Find information about installed external
                                  packages
      -V, --version               Display Spack version
      -h, --help                  Display this message
          --setup                 Download and install Spack backend
          --spec                  Display Spack specification help

  Please see Mason documentation for more instructions on using external
  packages
  """.dedent().strip();
  writeln(s);
}

proc masonExternalFindHelp() {
  const s = """
Find external packages on your system and get information about them

Usage:
    mason external find [options]
    mason external find [options] <package>

    <package>: a Spack spec expression indicating the package to find

Options:
    -h, --help                  Display this message

Display Options:
    -s, --short                 Show only specs (default)
    -p, --paths                 Show paths to package install directories
    -d, --deps                  Show full dependency DAG of installed packages
    -l, --long                  Show dependency hashes as well as versions
    -L, --very-long             Show full dependency hashes as well as versions
    -t TAGS, --tags TAGS        Filter a package query by tags
        --show-flags            Show spec compiler flags
        --show-full-compiler    Show full compiler specs
        --variants              Show variants in output (can be long)
    -e, --explicit              Show only specs that were installed explicitly
    -E, --implicit              Show only specs that were installed as
                                dependencies
    -u, --unknown               Show only specs Spack does not have a package
                                for
    -m, --missing               Show missing dependencies as well as installed
                                specs
    -M, --only-missing          Show only missing dependencies
    -N, --namespace             Show fully qualified package names

When no package is listed, all installed external packages will be listed.
  """.dedent().strip();
  writeln(s);
}

proc masonExternalInfoHelp() {
  const s = """
  Get information about external packages and system architecture

  Usage:
      mason external info [options] <package>

      <package>: a Spack spec expression indicating the package to retrieve
                 information on

  Options:
      -h, --help                  Display this message
          --arch                  Print architecture information about current
                                  system
  """.dedent().strip();
  writeln(s);
}

proc masonExternalSearchHelp() {
  const s = """
  Search for external packages

  Usage:
      mason external search [options] <search string>

  Options:
      -h, --help                  Display this message
      -d, --desc                  Parse descriptions of package to include more
                                  search results
  """.dedent().strip();
  writeln(s);
}

proc masonInstallHelp() {
  const s = """
Install external packages onto your system

Usage:
    mason external install [options] <package>

    <package>: a Spack spec expression indicating the package to install

Options:
    -h, --help                     Display this message
    --only {package,dependencies}  Select the mode of installation. The default
                                   is to install the package along with all its
                                   dependencies. Alternatively one can decide to
                                   install only the package or only the
                                   dependencies
    --jobs JOBS                    Explicitly set number of make jobs. Default
                                   is #cpus
    --overwrite                    Reinstall an existing spec, even if it has
                                   dependents
    --keep-prefix                  Don't remove the install prefix if
                                   installation fails
    --keep-stage                   Don't remove the build stage if installation
                                   succeeds
    --restage                      If a partial install is detected, delete
                                   prior state
    --use-cache                    Check for pre-built packages in mirrors
    --show-log-on-error            Print full build log to stderr if build fails
    --source                       Install source files in prefix
    --no-checksum                  Do not check packages against checksum
    --fake                         Fake install for debug purposes.
    --file                         Install from file. Read specs to install from
                                   .yaml
    --clean                        Sanitize the environment from variables that
                                   can affect how packages find libraries or
                                   headers
    --dirty                        Maintain the current environment without
                                   trying to
                                   sanitize it
    --test {root,all}              If 'root' is chosen, run package tests during
                                   installation for top-level packages (but skip
                                   tests for dependencies). If 'all' is chosen,
                                   run package tests during installation for all
                                   packages. If neither are chosen, don't run
                                   tests for any packages.
    --run-tests                    Run package tests during installation
                                   (same as --test=all)
    --log-format {junit}           Format to be used for log files
    --log-file LOG_FILE            Filename for the log file. If not passed a
                                   default will be used
    --yes-to-all                   Assume 'yes' is the answer to every
                                   confirmation request

External Mason packages can be installed as follows:
    mason external install <full Spack spec expression>
  """.dedent().strip();
  writeln(s);
}

proc masonUninstallHelp() {
  const s = """
  Uninstall external packages on your system

  Usage:
      mason external uninstall [options] <package>

      <package>: a Spack spec expression indicating the package to install

  Options:
      -h, --help                  Display this message
          --force                 Remove regardless of dependents
          --all                   USE CAREFULLY. remove ALL installed packages
                                  that match supplied spec
          --dependents            Also uninstall any dependent packages

  External Mason packages can be uninstalled as follows:
      mason external uninstall <full Spack spec expression>
  """.dedent().strip();
  writeln(s);
}

proc masonCompilerHelp() {
  const s = """
  Find and view compilers on your system

  Usage:
      mason external compiler [options]

  Options:
      -h, --help                  Display this message
          --find                  Find compilers on your system
          --edit                  Open the compilers configuration file in
                                  $EDITOR
          --list                  List the compilers on your system
  """.dedent().strip();
  writeln(s);
}

proc masonTestHelp() {
  const s = """
mason test works inside and outside of mason packages.
Inside a mason package: run test files found in test/
Outside of a mason package: run test files found in the provided path
                            (defaults to '.').

Usage:
    mason test [options] <path>

Options:
    -h, --help                  Display this message
        --show                  Direct output of tests to stdout
        --no-run                Compile tests without running them
        --keep-binary           Doesn't delete the binaries after running
        --recursive             Descend recursively into subdirectories of
                                given directories
        --parallel              Run tests in parallel (sequential by default)
        --[no]-update           [Do not] update the mason-registry when testing
        --setComm               Set the CHPL_COMM value for running the tests
                                e.g. none, gasnet, ofi
        --filter=FILTER         Filter the tests to run in each file by a
                                regular expression

Test configuration is up to the user
Tests pass if they exit with status code 0
  """.dedent().strip();
  writeln(s);
}

proc masonSystemHelp() {
  const s = """
  Integrate a Mason package with system packages found via pkg-config

  Usage:
      mason system <command> [<args>...]
      mason system [options]

  Options:
      pc                          Print a system package's .pc file
      search                      Search all packages available on the system
      -h, --help                  Display this message

  For more information on using system dependencies see the Mason documentation
  """.dedent().strip();
  writeln(s);
}

proc masonSystemSearchHelp() {
  const s = """
  Search for packages on system found via pkg-config.
  Packages are searched by name via the passed query.

  Usage:
      mason system search [options] <query>
      mason system search [options]

  Options:
      -h, --help                  Display this message
          --no-show-desc          Only display package name
          --desc                  Search package descriptions as well as names
  """.dedent().strip();
  writeln(s);
}

proc masonSystemPcHelp() {
  const s = """
  Print a package's .pc file (pkg-config file)

  Usage:
      mason system pc [options] <package name>

  Options:
      -h, --help                  Display this message

  The pc command sometimes has trouble finding a .pc file if the file is named
  something other than '<package name>.pc'. If this is the case,
  try searching for the package with 'mason system search'
  """.dedent().strip();
  writeln(s);
}

proc masonCleanHelp() {
  const s = """
  Cleans the target directory of the mason directory

  Usage:
      mason clean [options]

  Options:
      -h, --help                  Display this message
  """.dedent().strip();
  writeln(s);
}


proc masonPublishHelp() {
  const s = """
Publish a package to the mason-registry repository

Usage:
    mason publish [options] <registry>

Options:
    <registry>                   Positional argument indicates the target
                                 registry.
                                 Defaults to chapel-lang/mason-registry
    -h, --help                   Display this message
    -c, --create-registry        Creates a local registry at path
    --dry-run                    Check to see if package is ready to be
                                 published
    --check                      Runs check to see if package can be published
                                 successfully to <registry>
    --ci-check                   Same as --check, except omits git origin checks
    --[no-]update                [Do not] Prevent registries from being updated
                                 when a package is published
    --refresh-licenses           Force-update the list of valid license names
                                 and immediately exit without publishing

Publishing requires the mason-registry to be forked and the package to have a
remote origin.
  """.dedent().strip();
  writeln(s);
}


proc masonDocHelp() {
  const s = """
  Generate documentation for a mason package using chpldoc

  Usage:
      mason doc [options]

  Options:
      -h, --help                  Display this message

  Will generate documentation when ran inside a mason package.
  Requires that chpldoc is set up in order to work.
  For instructions on setting up chpldoc, please view its documentation.
  """.dedent().strip();
  writeln(s);
}

proc masonModulesHelp() {
  const s = """
  Print flags to include modules from a toml file

  Usage:
      mason modules
  """.dedent().strip();
  writeln(s);
}
