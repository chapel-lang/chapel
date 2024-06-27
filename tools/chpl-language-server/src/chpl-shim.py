#!/usr/bin/env python3
#
# Copyright 2024-2024 Hewlett Packard Enterprise Development LP
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

import subprocess
import sys
import argparse
import tempfile
import os
import json
from collections import defaultdict
from chapel import *
from chapel.core import *


def list_parsed_files(files, module_paths):
    ctx = Context()
    ctx.set_module_paths(module_paths, files)

    for file in files:
        asts = ctx.parse(file)

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

    # Create a temporary directory to add the 'chpl' wrapper script
    tempdir = tempfile.TemporaryDirectory()
    wrapper = os.path.join(tempdir.name, "chpl")
    os.symlink(__file__, wrapper)

    # Modify path to include the wrapper script at the front
    chpl = (
        subprocess.check_output(["which", "chpl"])
        .decode(sys.stdout.encoding)
        .strip()
    )
    newpath = os.pathsep.join([tempdir.name, os.environ["PATH"]])

    newenv = os.environ.copy()
    newenv["CHPL_SHIM_TARGET_PATH"] = tempdir.name
    newenv["CHPL_SHIM_REAL_COMPILER_PATH"] = chpl
    newenv["PATH"] = newpath

    # Invoke the script I was asked to invoke
    subprocess.run(sys.argv[1:], env=newenv)

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
            json.dump(commands, f)


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
        if arg.endswith(".chpl"):
            files.append(arg)

    files = [os.path.abspath(file) for file in files if file.endswith(".chpl")]
    files += list_parsed_files(files, args.module_dirs)
    files = list(set(files))

    commands = {}
    for file in files:
        invocation = " ".join(sys.argv).replace(__file__, real_compiler)
        commands[file] = {
            #   "invocation": invocation,
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
    if sys.argv[0].endswith("chpl-shim.py"):
        run_toplevel()
    else:
        run_chpl_shim()
