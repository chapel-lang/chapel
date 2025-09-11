import os, subprocess as sp, sys
import re


target = sys.argv[1]
output_file = sys.argv[2]

chplenv_lines = (
    sp.check_output(
        [
            os.environ["CHPL_HOME"] + "/util/printchplenv",
            "--all",
            "--internal",
            "--simple",
        ]
    )
    .decode()
    .splitlines()
)


chplenv = {k: v for k, v in [line.split("=", 1) for line in chplenv_lines]}
llvm_bin = (
    sp.check_output([chplenv["CHPL_LLVM_CONFIG"], "--bindir"]).decode().strip()
)
llvm_dwarfdump = llvm_bin + "/llvm-dwarfdump"

dwarfDumpTarget = target

# On OSX we should have built a '.dSYM' archive.
if chplenv["CHPL_TARGET_PLATFORM"] == "darwin":
    dwarfDumpTarget += ".dSYM"
if chplenv["CHPL_LAUNCHER"] != "none":
    dwarfDumpTarget += "_real"

def verify():

    if chplenv["CHPL_TARGET_PLATFORM"] != "darwin":
        # TODO: skip the verify check on non-macOS platforms for now
        # on non-macOS platforms we may have linked against system
        # libraries ike glibc with debug info and it causes --verify to hang
        return None

    try:
        sp.check_call([llvm_dwarfdump, "--debug-info", "--verify", dwarfDumpTarget], stdout=sp.PIPE, stderr=sp.STDOUT)
    except sp.CalledProcessError as e:
        return e.output.decode().strip()
    return None

def dump_symbol(name):
    try:
        cmd = [llvm_dwarfdump, dwarfDumpTarget, "--debug-info", "--name", name, "--show-children", "--diff"]
        output = sp.check_output(cmd).decode().strip()
        # Remove the first line which is just the filename
        output = output.splitlines(keepends=True)[1:]
        # remove any line with DW_AT_location
        output = [line for line in output if "DW_AT_location" not in line]

        # replace anything that looks like CHPL_HOME with CHPL_HOME
        chpl_home = os.environ.get("CHPL_HOME", "")
        output = [re.sub(re.escape(chpl_home), "$CHPL_HOME", line) for line in output]

        # DW_TAG_enumeration_type can print in any order, so sanitize
        # DW_AT_const_value and DW_AT_name lines to a deterministic order
        if any("DW_TAG_enumeration_type" in line for line in output):
            output = [re.sub(r"\d+", "ENUM_VAL", line) if "DW_AT_const_value" in line else line for line in output]
            output = [re.sub(r'\(".*"\)', '("ENUM_NAME")', line) if "DW_AT_name" in line else line for line in output]

        output = "".join(output).strip()
        return output
    except sp.CalledProcessError as e:
        return f"Error dumping symbol {name}: {e.output.decode().strip()}"
    except Exception as e:
        return f"Error dumping symbol {name}: {str(e)}"


def test_symbols(expected_symbols):
    res = []
    for name, expected in expected_symbols.items():
        output = dump_symbol(name)

        if expected.strip() not in output:
            res.append(f"Symbol {name} did not match expected output")
            res.append("Expected:")
            res.append(expected.strip())
            res.append("Got:")
            res.append(output)
            res.append("="*40)
    return res

def run(expected_symbols):
    with open(output_file, "a") as f:
        print("Running DWARF verification...", file=f)
        verify_errors = verify()
        if verify_errors:
            print("DWARF verification failed:", file=f)
            print(verify_errors, file=f)
            print("FAIL", file=f)
            sys.exit(1)
        else:
            print("Success!!", file=f)
        print("Running DWARF dump checks...", file=f)
        errors = test_symbols(expected_symbols)
        if errors:
            for err in errors:
                print(err, file=f)
            print("FAIL", file=f)
        else:
            print("Success!!", file=f)
