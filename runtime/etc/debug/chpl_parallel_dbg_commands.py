#
# Copyright 2025-2025 Hewlett Packard Enterprise Development LP
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

import shlex
import lldb
import os

def parse_command(command):
    command = shlex.split(command.strip())
    return command


def run_command(debugger: lldb.SBDebugger, command: str):
    interp = debugger.GetCommandInterpreter()
    result = lldb.SBCommandReturnObject()
    interp.HandleCommand(command, result)
    if not result.Succeeded():
        raise RuntimeError(f"Command failed: {result.GetError()}")
    return result.GetOutput()



"""
command script import chpldbg.py
command script add -f chpldbg.on on
command script add -f chpldbg.connect connect

lldb -o "command script import chpldbg_cmds.py" -o "command script add -f chpldbg.on on" -o "command script add -f chpldbg.connect connect"
"""
def on(debugger: lldb.SBDebugger, command, result, internal_dict):
    # get a list of targets
    current_locale = debugger.GetSelectedTarget()


    locales = []
    command = parse_command(command)
    if len(command) == 0:
        print("Usage: on [all|<locale-id>[,..<locale-id>]] ...")
        return
    elif command[0] == "all":
        for i in range(debugger.GetNumTargets()):
            target = debugger.GetTargetAtIndex(i)
            if target.IsValid():
                locales.append(target)
    else:
        locale_ids = command[0].split(",")
        for locale_id in locale_ids:
            try:
                locale_id = int(locale_id)
            except ValueError:
                print(f"Invalid locale id: {locale_id}")
                return
            if locale_id < 0 or locale_id >= debugger.GetNumTargets():
                print(f"Invalid locale id: {locale_id}")
                return
            target = debugger.GetTargetAtIndex(locale_id)
            if target.IsValid():
                locales.append(target)

    if len(locales) == 0:
        print("No valid locales found")
        return

    if len(command) > 1:
        # TODO: on locale <command> doesn't work properly yet
        print("Running command on multiple locales is not fully supported yet")
        return

        prev = debugger.GetSelectedTarget()
        for t in locales:
            debugger.SetSelectedTarget(t)
            c = ' '.join(command[1:])
            print("Running command on locale %d: %s" % (t.GetProcess().GetProcessID(), c))
            try:
                run_command(debugger, c)
            except RuntimeError as e:
                print(f"Error running command on locale {t.GetProcess().GetProcessID()}: {e}")
        if prev.IsValid():
            debugger.SetSelectedTarget(prev)
    else:
        if len(locales) != 1:
            print("Multiple locales selected, please specify a command to run")
            return
        target = locales[0]
        debugger.SetSelectedTarget(target)

def connect(debugger: lldb.SBDebugger, command, result, internal_dict):
    command = parse_command(command)
    if len(command) != 2:
        print("Usage: connect /path/to/chapel/binary NUM_LOCALES")
        return

    binary = command[0]
    try:
        num_locales = int(command[1])
    except ValueError:
        print("NUM_LOCALES must be an integer")
        return
    if num_locales < 1:
        print("NUM_LOCALES must be greater than 0")
        return

    # clear all targets
    for i in range(debugger.GetNumTargets()):
        target = debugger.GetTargetAtIndex(i)
        debugger.DeleteTarget(target)

    # create targets for each locale
    BASE_PORT = int(os.environ.get("CHPL_RT_DEBUG_SERVER_BASE_PORT", "5000"))
    targets = []
    for i in range(num_locales):
        target = debugger.CreateTargetWithFileAndArch(binary, lldb.LLDB_ARCH_DEFAULT)
        if not target.IsValid():
            print(f"Failed to create target for locale {i}")
            return
        targets.append(target)
        debugger.SetSelectedTarget(target)
        # debugger.HandleCommand(f"process connect connect://127.0.0.1:{BASE_PORT + i}")
        debugger.HandleCommand(f"gdb-remote {BASE_PORT + i}")


def __lldb_init_module(debugger, internal_dict):
    print("Chapel parallel debugger commands loaded")
    directory = os.path.dirname(os.path.abspath(__file__))
    pretty_printer_file = os.path.join(directory, "chpl_lldb_pretty_print.py")
    lldb_commands_file = os.path.join(directory, "lldb.commands")
    debugger.HandleCommand(f'command source "{lldb_commands_file}"')
    debugger.HandleCommand(f'command script import "{pretty_printer_file}"')
    debugger.HandleCommand("command script add -f chpl_parallel_dbg_commands.on on")
    debugger.HandleCommand("command script add -f chpl_parallel_dbg_commands.connect connect")

