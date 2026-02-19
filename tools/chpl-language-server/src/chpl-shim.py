#!/usr/bin/env python3
#
# Copyright 2024-2026 Hewlett Packard Enterprise Development LP
# Other additional copyright holders may be indicated within.
#
# The entirety of this work is licensed under the Apache License,
# Version 2.0 (the "License"); you may not use this file except
# in compliance with the License.
#
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

import argparse
import getopt
import json
import os
import re
import subprocess
import sys
import tempfile
from collections import defaultdict

from chapel import *
from chapel.core import *


def list_parsed_files(files, module_paths):
    ctx = Context()
    ctx.set_module_paths(module_paths, files)

    # parse all files before triggering resolution
    all_asts = {file: ctx.parse(file) for file in files}
    for asts in all_asts.values():
        # Trigger scope resolution to make sure other files are parsed.
        # XXX: hack
        for ast, _ in each_matching(asts, Identifier):
            ast.to_node()
    return [
        parsed_path
        for parsed_path in ctx.introspect_parsed_files()
        if not ctx.is_bundled_path(parsed_path)
    ]


def run_toplevel():
    # Top-level invocation; behave as the driver script.

    # parse manually using getopt so users have to put chpl-shim options first
    # getopt will stop on the first non-option argument, i.e. chpl or mason or similar
    generate_vscode_tasks = False
    flags, args = getopt.getopt(sys.argv[1:], "", ["vscode"])
    for flag, _ in flags:
        if flag in ["--vscode"]:
            generate_vscode_tasks = True

    # TODO: this only works when chpl is not passed as a path and we rely on
    # the PATH to find it. i.e. `./bin/chpl-shim ./bin/chpl` will not work.

    newenv = os.environ.copy()

    # Create a temporary directory to add the 'chpl' wrapper script
    tempdir = tempfile.TemporaryDirectory()
    wrapper = os.path.join(tempdir.name, "chpl")
    os.symlink(__file__, wrapper)

    if "CHPL_SHIM_TARGET_PATH" not in newenv:
        newenv["CHPL_SHIM_TARGET_PATH"] = tempdir.name
    if "CHPL_SHIM_REAL_COMPILER_PATH" not in newenv:
        chpl = (
            subprocess.check_output(["which", "chpl"])
            .decode(sys.stdout.encoding)
            .strip()
        )
        newenv["CHPL_SHIM_REAL_COMPILER_PATH"] = chpl

    # Modify path to include the wrapper script at the front
    newpath = os.pathsep.join([tempdir.name, os.environ["PATH"]])
    newenv["PATH"] = newpath

    # Invoke the script I was asked to invoke
    subprocess.run(args, env=newenv)

    # At this point, our subprocessses have finished and created flags
    # in the tempdir. Iterate all the files in the dir and merge the results.
    commands = defaultdict(list)

    for file in os.listdir(tempdir.name):
        file = os.path.join(tempdir.name, file)
        if os.path.isfile(file) and file.endswith(".json"):
            with open(file) as f:
                from_file = json.load(f)
                for key, value in from_file.items():
                    commands[key].append(value)

    # Skip if we would overwrite an existing file with empty commands
    skip_cls = (
        os.path.exists(".cls-commands.json") and len(commands.keys()) == 0
    )
    if not skip_cls:
        with open(".cls-commands.json", "w") as f:
            json.dump(commands, f, indent=2)

    # generate vscode tasks if requested and there is actually something to do
    if generate_vscode_tasks and args:
        # generate a 'type: process' for the command line passed to chpl-shim
        # i.e. `chpl-shim make`
        #
        # TODO: and generate a 'type: chpl' for the extracted invocation,
        # if it exists i.e. commands.get('invocation', None)
        # currently we don't generate that since it seems redundant

        command = args[0]
        remaining_args = args[1:]

        def get_env_affecting_chapel():
            """Similar logic to what's in the vscode extension"""
            base_globs = [
                r"CHPL_.*",
                r"CHPL_RT_.*",
                r"FI_.*",
                r"GASNET_.*",
                r"SLURM_.*",
                r"PBS_.*",
                r"QTHREAD_.*",
                r"QT_.*",
                r"ASAN_OPTIONS",
                r"PATH",
            ]
            base_globs = [re.compile(glob) for glob in base_globs]
            env = {
                k: v
                for k, v in os.environ.items()
                if any(g.match(k) for g in base_globs)
            }
            return env

        cli_task = {
            "label": " ".join(args),
            "type": "process",
            "command": command,
            "args": remaining_args,
            "problemMatcher": ["$chpl-compiler"],
            "options": {
                "cwd": os.getcwd(),
                "env": get_env_affecting_chapel(),
            },
        }

        # look for a .vscode/tasks.json file
        vscode_tasks_path = os.path.join(os.getcwd(), ".vscode", "tasks.json")
        if os.path.exists(vscode_tasks_path):
            with open(vscode_tasks_path, "r") as f:
                tasks = json.load(f)
        else:
            tasks = {"version": "2.0.0", "tasks": []}

        replaced = False
        for idx, t in enumerate(tasks["tasks"]):
            if t["label"] == cli_task["label"]:
                tasks["tasks"][idx] = cli_task
                replaced = True
                break
        if not replaced:
            tasks["tasks"].append(cli_task)

        with open(vscode_tasks_path, "w") as f:
            json.dump(tasks, f, indent=2)


def run_chpl_shim():
    # Being invoked as a wrapper script
    real_compiler = os.environ["CHPL_SHIM_REAL_COMPILER_PATH"]
    tmpfile_path = os.environ["CHPL_SHIM_TARGET_PATH"]

    parser = argparse.ArgumentParser()
    parser.add_argument("filename", nargs="*")
    parser.add_argument(
        "-M", "--module-dir", action="append", dest="module_dirs", default=[]
    )

    args, _ = parser.parse_known_args()
    files = args.filename

    # Since we don't have a perfectly matching arg-parser, iterate argv again
    # to look for .chpl files
    for arg in sys.argv:
        if arg.endswith(".chpl") and arg not in files:
            files.append(arg)

    # if no .chpl files were specified, `chpl` may have been invoked
    # with just `--print-chpl-home` or `--version`, so we should not generate
    # a .cls-commands.json file.
    skip_cls_generation = len(files) == 0

    if not skip_cls_generation:
        files = [
            os.path.abspath(file) for file in files if file.endswith(".chpl")
        ]
        files += list_parsed_files(files, args.module_dirs)
        files = list(set(files))

        # Detect symlinks in the form '.chpl.chpl'. These are produced by
        # our current CMake support. Instead, resolve the symlink to the real
        # file to avoid confusion.
        for i in range(len(files)):
            file = files[i]
            if file.endswith(".chpl.chpl") and os.path.islink(file):
                real_file = os.path.realpath(file)
                assert real_file.endswith(".chpl")
                files[i] = real_file

        commands = {}
        invocation = " ".join(sys.argv).replace(__file__, real_compiler)
        commands["invocation"] = invocation
        for file in files:
            commands[file] = {
                "module_dirs": [
                    os.path.abspath(mod_dir) for mod_dir in args.module_dirs
                ],
                "files": files,
            }

        tmpfile = tempfile.NamedTemporaryFile(
            mode="w", delete=False, dir=tmpfile_path, suffix=".json"
        )

        # Convert to json
        with tmpfile as f:
            f.write(json.dumps(commands))

    subprocess.run([real_compiler] + sys.argv[1:])


if __name__ == "__main__":
    """
    The normal flow is as follows:
      1. The shim is invoked as the driver, where the rest of the arguments
         are the command that ends up building a Chapel program.
      2. Assuming that the comamnd refers to "just" 'chpl' (i.e. not a path to chpl),
         the shim creates a temporary directory, links itself as 'chpl' in that directory,
         and modifies the PATH to include that directory at the front.
         This way, when the command invokes 'chpl', it will invoke the shim again,
         but this time as the wrapper.
      3. When this happens, the shim receives all the arguments the command
         would've provided to the real 'chpl' compiler. The shim then extracts
         the .chpl files and other data needed for .cls-commands.json, writes
         that data to a temporary file, and then invokes the real 'chpl'
         compiler with the same arguments it received.
    """

    if sys.argv[0].endswith("chpl-shim.py"):
        run_toplevel()
    else:
        run_chpl_shim()
