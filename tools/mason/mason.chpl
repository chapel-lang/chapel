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
 *
 * Author: Sam Partee
 * Github: @Spartee
 */


use MasonUtils;
use MasonHelp;
use MasonEnv;
use MasonNew;
use MasonBuild;
use MasonUpdate;
use MasonSearch;
use FileSystem;

/*

The Design of Mason
===================

  Mason is a command line tool for building chapel programs to provide users with
       a consistent way of building applications and libraries. Mason uses
       a four state pipeline to go from start to finish in a project. The
       four states are listed below.


  Four States:

  1) Project Code: ``yourProject/src/yourProject.chpl``
           This is the source code of the project the user creates using mason.
  2) Manifest File: ``Mason.toml``
           Toml file containing metadata and dependencies
           Builds dependency directed acyclic graph (DAG) to be
           serialized into lock file
  3) Lock File:  ``Mason.lock``
           Contains necessary build information
           Serialized directed acyclic graph of the dependencies build options
           from the manifest
  4) Dependency Code:  ``$MASON_HOME/src``
           Local dependencies downloaded by mason after the user lists them in
           a project manifest.

Full documentation is located in the chapel release in $CHPL_HOME/doc/rst/tools/mason/mason.rst

*/



proc main(args: [] string) {
  if args.size < 2 {
    masonHelp();
    exit();
  }

  select (args[1]) {
    when 'new' do masonNew(args);
    when 'build' do masonBuild(args);
    when 'update' do UpdateLock(args);
    when 'run' do masonRun(args);
    when 'search' do masonSearch(args);
    when 'env' do masonEnv(args);
    when 'doc' do masonDoc(args);
    when 'clean' do masonClean();
    when '--list' do masonList();
    when '-h' do masonHelp();
    when '--help' do masonHelp();
    when '-V' do printVersion();
    when '--version' do printVersion();
    otherwise {
      writeln('error: no such subcommand');
      writeln('try mason --help');
      exit();
    }
  }
}




proc masonRun(args) {
  var toRun = basename(getEnv('PWD'));
  var show = false;
  var execopts: [1..0] string;
  if args.size > 2 {
    for arg in args[2..] {
      if arg == '-h' || arg == '--help' {
        masonRunHelp();
        exit();
      }
      else if arg == '--build' {
        masonBuild(['mason', 'build']);
      }
      else if arg == '--show' {
        show = true;
      }
      else {
        execopts.push_back(arg);
      }
    }
  }
  // Find the Binary and execute
  if isDir('target') {
    var execs = ' '.join(execopts);
    var command = "target/debug/" + toRun + ' ' + execs;
    if isDir('target/release') then
      command = "target/release/" + toRun + ' ' + execs;

    if show then
      writeln("Executing binary: " + command);

    if isFile("Mason.lock") then  // If built
      runCommand(command);
    else if isFile("Mason.toml") { // If not built
      masonBuild(args);
      runCommand(command);
    }
    else writeln("call mason run from the top level of your projects directory");
  }
  else {
    writeln("Mason cannot find the compiled program");
    exit();
  }
}


proc masonClean() {
  runCommand('rm -rf target');
}


proc masonDoc(args) {
  var toDoc = basename(getEnv('PWD'));
  var project = toDoc + '.chpl';
  if isDir('src/') {
    if isFile('src/' + project) {
      var command = 'chpldoc src/' + project;
      writeln(command);
      runCommand(command);
    }
  }
  else {
    writeln('Mason could not find the project to document!');
    runCommand('chpldoc');
  }
}

proc printVersion() {
  writeln('mason 0.1.0');
}

