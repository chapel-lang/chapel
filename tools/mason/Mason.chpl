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
 *
 * Author: Sam Partee
 * Github: @Spartee
 */


/*

The Design of Mason
===================

  Mason is a command line tool for building chapel programs to provide users
  with a consistent way of building applications and libraries. Mason uses
  a four state pipeline to go from start to finish in a project. The
  four states are listed below.


  Four States:

  1) Project Code: ``yourProject/src/yourProject.chpl``
           This is the source code of the project the user creates using mason.
  2) Manifest File: ``Mason.toml``
           Toml file containing metadata and dependencies
           Builds dependency directed acyclic graph (DAG) to be
           serialized into lock file
  3) Lock File: ``Mason.lock``
           Contains necessary build information
           Serialized directed acyclic graph of the dependencies build options
           from the manifest
  4) Dependency Code: ``$MASON_HOME/src``
           Local dependencies downloaded by mason after the user lists them in
           a project manifest.

Full documentation is located in the chapel release in
$CHPL_HOME/doc/rst/tools/mason/mason.rst

*/
module Mason {
  use ArgumentParser;
  use FileSystem;
  use Map;
  use MasonBuild;
  use MasonDoc;
  use MasonEnv;
  use MasonExternal;
  use MasonHelp;
  use MasonInit;
  use MasonModify;
  use MasonNew;
  use MasonPublish;
  use MasonRun;
  use MasonSearch;
  use MasonSystem;
  use MasonTest;
  use MasonUpdate;
  use MasonUtils;
  use MasonModules;

  import MasonLogger;
  use List;

  proc main(args: [] string) throws {

    var parser = new argumentParser(helpHandler = new MasonHelpHandler());

    var subCmds = new map(string, shared Argument);

    // define all the supported subcommand strings here
    var cmds = ["add","build","clean","doc","env","external","init","publish",
                "new","rm","run","search","system","test","update",
                "help","version","modules"];
    for cmd in cmds {
      subCmds.add(cmd,parser.addSubCommand(cmd));
    }

    var versionFlag = parser.addFlag(name="versionFlag",
                                    opts=["-V","--version"],
                                    defaultValue=false);

    var noColorFlag = parser.addFlag(name="noColorFlag",
                                    opts=["--no-color"],
                                    defaultValue=false);

    parser.parseArgs(args);

    // TODO: Can printVersion take an exit code?
    if versionFlag.valueAsBool() {
      printVersion();
      return 0;
    }

    MasonLogger.setUseColorOutput(!noColorFlag.valueAsBool());

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
    var retCode = 0;
    // pass the arguments to the appropriate subcommand
    try! {
      select (usedCmd) {
        when "add" do masonModify(cmdArgs);
        when "build" do masonBuild(cmdArgs);
        when "clean" do masonClean(cmdArgs);
        when "doc" do masonDoc(cmdArgs);
        when "env" do masonEnv(cmdArgs);
        when "external" do masonExternal(cmdArgs);
        when "help" do masonHelp();
        when "init" do masonInit(cmdArgs);
        when "new" do masonNew(cmdArgs);
        when "publish" do masonPublish(cmdArgs);
        when "rm" do masonModify(cmdArgs);
        when "run" do masonRun(cmdArgs);
        when "search" do retCode = masonSearch(cmdArgs);
        when "system" do retCode = masonSystem(cmdArgs);
        when "test" do masonTest(cmdArgs);
        when "update" do masonUpdate(cmdArgs);
        when "modules" do masonModules(cmdArgs);
        when "version" do printVersion();
        otherwise {
          writeln("No subcommand provided");
          masonHelp();
          retCode = 1;
        }
      }
    } catch ex: MasonError {
      stderr.writeln(ex.message());
      retCode = 1;
    }
    return retCode;
  }


  proc masonClean(args) throws {
    var parser = new argumentParser(helpHandler=new MasonCleanHelpHandler());

    parser.parseArgs(args);
    const cwd = here.cwd();

    const projectHome = getProjectHome(cwd);
    runCommand("rm -rf " + projectHome + "/target");
  }


  proc printVersion() {
    writeln("mason " + MASON_VERSION);
  }
}
