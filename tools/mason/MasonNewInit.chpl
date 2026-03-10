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

module MasonNewInit {

  import IO;
  import IO.format;
  import HaltWrappers;
  import ArgumentParser.{argumentParser, Argument};
  import List.list;

  import MasonUtils;
  import MasonUtils.MasonError;
  import MasonLogger;
  import MasonHelp.{MasonNewHelpHandler, MasonInitHelpHandler};
  import Manifest.{manifestFile, packageType};

  import TemplateString.templateString;
  import Pathlib.path;

  private var log = new MasonLogger.logger("mason new/init");

  class MasonPackageTemplate {
    var manifest: manifestFile;
    var directory: path;

    proc init(in manifest: manifestFile, directory: path) {
      this.manifest = manifest;
      this.directory = directory;
    }

    /*
      Run basic checks before creating the project
    */
    proc checks(isNew: bool) throws {
      if !MasonUtils.isIdentifier(manifest.name) {
        throw new MasonError("Bad package name '" + manifest.name +
           "' - only Chapel identifiers are legal package names.\n" +
           "Use '--name <LegalName>' to specify a legal package name.");
      }

      if isNew && directory.isDir() {
        throw new MasonError("A directory named '" +
                              directory:string + "' already exists");
      }

      if !isNew && !directory.isDir() {
        throw new  MasonError("Directory does not exist: " + directory:string +
                              " Did you mean 'mason new' to create a " +
                              "new project from scratch?");
      }

      if !isNew {
        const isLightweight = manifest.pkgType == packageType.light;
        if (directory / "Mason.toml").isFile() {
          throw new MasonError(
            "Mason.toml already exists for current project. " +
            "Remove or rename the existing manifest file and rerun " +
            "`mason init` to initialize a new project.");
        } else if (directory / "src").isDir() && !isLightweight {
          throw new MasonError(
            "/src/ directory already exists for current project. " +
            "Remove or rename the /src/ directory and rerun " +
            "`mason init` to initialize a new project. " +
            "Alternatively, run `mason init --light` to add only a " +
            "manifest file.");
        }
      }
    }

    /*
      Main project builder
    */
    proc makeProject() throws {
      makeBasicToml();
      makeSourceFiles();
    }
    /*
      Create a git repo and related files (ie gitignore)
    */
    proc makeVCS(show: bool) throws {
      makeGit(show);
      makeGitignore();
    }

    proc makeSourceFiles() throws do HaltWrappers.pureVirtualMethodHalt();

    proc makeBasicToml() throws {
      const s = manifest.toToml();
      var writer = IO.openWriter((directory / "Mason.toml"):string);
      writer.write(s);
    }
    proc makeSrcDir() throws {
      (directory / "src").mkdir();
    }
    proc makeGit(show: bool) throws {
      if (directory / ".git").isDir() {
        throw new MasonError("'" + directory:string +
                      "' is already a git repository. " +
                      "Use `--no-vcs` to create a project " +
                      "without initializing a git repository.");
      }
      var cmd: list(string);
      cmd.pushBack(["git", "init"]);
      if !show then cmd.pushBack("-q");
      cmd.pushBack(["-b", "main"]);
      cmd.pushBack(directory:string);
      MasonUtils.runCommand(cmd.toArray());
    }
    proc makeGitignore() throws {
      const toIgnore = """
        target/
        Mason.lock
        doc/
      """.dedent().strip(trailing=false);

      const file = directory / ".gitignore";
      if file.isFile() {
        log.warnln(".gitignore already exists, " +
                  "skipping creation of .gitignore file");
        return;
      }

      var writer = IO.openWriter(file:string);
      writer.write(toIgnore);
    }

  }
  class MasonApplicationTemplate: MasonPackageTemplate {
    proc init(in manifest: manifestFile, directory: path) {
      super.init(manifest, directory);
    }
    override proc makeSourceFiles() throws {
      const srcDir = directory / "src";
      srcDir.mkdir();
      const appTemp: templateString = """
        /* Documentation for {{name}} */
        module {{name}} {
          proc main() {
            writeln("New mason package: {{name}}");
          }
        }
      """.dedent().strip(trailing=false);
      var s = appTemp(["name"=>manifest.name]);
      var writer =
        IO.openWriter((srcDir / "%s.chpl".format(manifest.name)):string);
      writer.write(s);
    }
  }
  class MasonLibraryTemplate: MasonPackageTemplate {
    proc init(in manifest: manifestFile, directory: path) {
      super.init(manifest, directory);
    }

    override proc makeSourceFiles() throws {
      const srcDir = directory / "src";
      srcDir.mkdir();
      const libTemp: templateString = """
        /* Documentation for {{name}} */
        module {{name}} {
          // Your library here
        }
      """.dedent().strip(trailing=false);
      var s = libTemp(["name"=>manifest.name]);
      var writer =
        IO.openWriter((srcDir / "%s.chpl".format(manifest.name)):string);
      writer.write(s);
    }
  }
  class MasonLightTemplate: MasonPackageTemplate {
    proc init(in manifest: manifestFile, directory: path) {
      super.init(manifest, directory);
    }

    override proc makeSourceFiles() throws {
      // lightweight packages do not create a src/ directory or main source file
    }

    @chplcheck.ignore("UnusedFormal")
    override proc makeVCS(show: bool) throws {
      // TODO: this warning doesn't seem very useful since the default is always
      // --vcs
      // log.warnln("Lightweight projects don't create git repositories");
    }
  }


  private proc getTemplate(in manifest: manifestFile,
                          directory: path): owned MasonPackageTemplate throws {
    use packageType;
    select manifest.pkgType {
      when application do
        return new MasonApplicationTemplate(manifest, directory);
      when library do return new MasonLibraryTemplate(manifest, directory);
      when light do return new MasonLightTemplate(manifest, directory);
      otherwise do
        HaltWrappers.exhaustiveSelectHalt("invalid packageType");
    }
  }

  private proc determinePackageType(appFlag, libFlag, lightFlag) throws {
    var isApplication = appFlag.valueAsBool();
    var isLibrary = libFlag.valueAsBool();
    var isLightweight = lightFlag.valueAsBool();
    if isApplication + isLibrary + isLightweight > 1 then
      throw new MasonError("A mason package cannot be of multiple types");

    if isApplication then
      return packageType.application;
    else if isLibrary then
      return packageType.library;
    else if isLightweight then
      return packageType.light;
    else
      return packageType.default;

  }

  private proc determinePackageNameAndPath(
    isNew: bool,
    nameOpt: Argument,
    legalNameOpt: Argument
  ): (string, path) throws {
    var name = if nameOpt.hasValue() then nameOpt.value().strip() else "";
    var directory: path;
    if isNew && name == "" then
      throw new MasonError("A package name must be specified");
    if !isNew && name == "" {
      name = basename(here.cwd());
      directory = ".":path;
    } else {
      directory = name:path;
    }
    if legalNameOpt.hasValue() {
      name = legalNameOpt.value().strip();
    }

    return (name, directory);
  }

  private proc addSharedFlags(ref parser) throws {
    var noVcsFlag = parser.addFlag(name="vcs",
                                  opts=["--no-vcs"],
                                  defaultValue=false);
    var legalNameOpt = parser.addOption(name="legalname",
                                        opts=["--name"]);

    var showFlag = parser.addFlag(name="show", defaultValue=false);
    var appFlag = parser.addFlag(name="app", defaultValue=false);
    var libFlag = parser.addFlag(name="lib", defaultValue=false);
    var lightFlag = parser.addFlag(name="light", defaultValue=false);

    // var exampleFlag = parser.addFlag(name="example",
    //                                   opts=["--no-examples"],
    //                                   defaultValue=true);
    // var testFlag = parser.addFlag(name="test",
    //                               opts=["--no-tests"],
    //                               defaultValue=true);

    return (noVcsFlag, legalNameOpt, showFlag, appFlag, libFlag, lightFlag);
  }

  /*
    Creates a new library project at a given directory
      mason new <projectName/directoryName>
  */
  proc masonNew(args: [] string) throws {
    var parser = new argumentParser(helpHandler=new MasonNewHelpHandler());
    var projectNameArg = parser.addArgument(name="project name", numArgs=1);
    var (noVcsFlag,
         legalNameOpt,
         showFlag,
         appFlag,
         libFlag,
         lightFlag) = addSharedFlags(parser);
    parser.parseArgs(args);

    var show = showFlag.valueAsBool();
    var noVcs = noVcsFlag.valueAsBool();

    var (name, directory) =
      determinePackageNameAndPath(isNew=true,
                                  nameOpt=projectNameArg,
                                  legalNameOpt=legalNameOpt);

    var pkgType = determinePackageType(appFlag, libFlag, lightFlag);
    var newPkgManifest = manifestFile.defaultNewPkg(name, pkgType);
    var template = getTemplate(newPkgManifest, directory);

    template.checks(isNew=true);
    directory.mkdir();
    if !noVcs then
      template.makeVCS(show);
    template.makeProject();
  }

  /*
    Initializes a library project in a project directory
      'mason init <dirName/path>''
      or 'mason init (inside project directory)''
  */
  proc masonInit(args: [] string) throws {
    var parser = new argumentParser(helpHandler=new MasonInitHelpHandler());
    var dirArg = parser.addArgument(name="directory", numArgs=0..1);
    var (noVcsFlag,
         legalNameOpt,
         showFlag,
         appFlag,
         libFlag,
         lightFlag) = addSharedFlags(parser);
    parser.parseArgs(args);

    var show = showFlag.valueAsBool();
    var noVcs = noVcsFlag.valueAsBool();

    var (name, directory) =
      determinePackageNameAndPath(isNew=false,
                                  nameOpt=dirArg,
                                  legalNameOpt=legalNameOpt);

    var pkgType = determinePackageType(appFlag, libFlag, lightFlag);
    var newPkgManifest = manifestFile.defaultNewPkg(name, pkgType);
    var template = getTemplate(newPkgManifest, directory);

    template.checks(isNew=false);
    if !noVcs then
      template.makeVCS(show);
    template.makeProject();
  }
}
