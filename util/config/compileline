#!/usr/bin/env python

import optparse
import os
import sys

script_dir = os.path.dirname(os.path.realpath(__file__))
chpl_home_dir = os.path.dirname(os.path.dirname(script_dir))
chplenv_dir = os.path.join(chpl_home_dir, "util", "chplenv")
sys.path.insert(0, os.path.abspath(chplenv_dir))

import chpl_compiler
import chpl_llvm
import chpl_make
import utils

def myrun(cmd, arg=None):

    use_cmd = list(cmd)
    if arg:
        use_cmd += [arg]

    # Run $use_cmd but filter out e.g. make[2]: lines
    # (about which directory we are in)
    ret = ""
    output = utils.run_command(use_cmd)
    lines = [line for line in output.splitlines(True)
                  if not line.startswith("make")]
    ret = ''.join(lines)

    return ret

def mysystem(cmd, arg=None):

    s = myrun(cmd, arg)
    s = s.strip()
    s += "\n"
    sys.stdout.write(s)

def parseArguments():
    parser = optparse.OptionParser(usage="usage: %prog [ENVVAR=VALUE] [flags]")
    parser.add_option("--home", const="home",
                      dest="actions", action='append_const',
                      help="print out CHPL_HOME")
    parser.add_option("--make", const="make",
                      dest="actions", action='append_const',
                      help="print out the make command")
    parser.add_option("--llvm", const="llvm",
                      dest="actions", action='append_const',
                      help="specify that the rest of the arguments are for a "
                           "compilation working with the LLVM backend. This "
                           "argument causes this script to set "
                           "CHPL_TARGET_COMPILER=clang-included")
    parser.add_option("--compile", "--compile-cc", const="compilecc",
                      dest="actions", action='append_const',
                      help="print a C compiler invocation that can use "
                           "the Chapel runtime")
    parser.add_option("--compile-c++", const="compilecxx",
                      dest="actions", action='append_const',
                      help="print a C++ compiler invocation that can use "
                           "the Chapel runtime")
    parser.add_option("--compiler", const="compiler",
                      dest="actions", action='append_const',
                      help="print the C compiler used")
    parser.add_option("--cflags", const="cflags",
                      dest="actions", action='append_const',
                      help="print C compilation flags that Chapel would use")
    parser.add_option("--c++flags", const="cxxflags",
                      dest="actions", action='append_const',
                      help="print C++ compilation flags that Chapel would use")
    parser.add_option("--includes-and-defines", const="includesanddefines",
                      dest="actions", action='append_const',
                      help="Print -D and -I compilation flags to include "
                           "the Chapel runtime")
    parser.add_option("--linkershared", const="linkershared",
                      dest="actions", action='append_const',
                      help="print out the command for linking with shared")
    parser.add_option("--libraries", const="libraries",
                      dest="actions", action='append_const',
                      help="print out -L and -l options to link with "
                           "the Chapel runtime")
    parser.add_option("--linker", const="linker",
                      dest="actions", action='append_const',
                      help="print the linker used")
    parser.add_option("--main.o", const="maino",
                      dest="actions", action='append_const',
                      help="print out the path to the main.o file")
    parser.add_option("--llvm-install-dir", const="llvminstalldir",
                      dest="actions", action='append_const',
                      help="print path to included LLVM installation")
    parser.add_option("--clang", const="clangcc",
                      dest="actions", action='append_const',
                      help="print which clang C compiler is configured")
    parser.add_option("--clang++", const="clangcxx",
                      dest="actions", action='append_const',
                      help="print which clang C++ compiler is configured")
    parser.add_option("--clang-sysroot-arguments", const="clangsysroot",
                      dest="actions", action='append_const',
                      help="print out any saved clang arguments that "
                           "specify the system root")
    parser.add_option("--multilocale-lib-deps", const="multilocale-lib-deps",
                      dest="actions", action='append_const',
                      help="print out libraries required to link "
                           "multi-locale libraries")

    (options, args) = parser.parse_args()

    # Handle VAR=VAL environment variable setting
    for arg in args:
        kv = arg.split("=", 2)
        if len(kv) == 2:
            os.environ[kv[0]] = os.environ[kv[1]]

    if options.actions:
        return options.actions
    else:
        return []


def main():

    global chpl_home_dir
    check_file = os.path.relpath(__file__, chpl_home_dir)
    check_path = os.path.join(chpl_home_dir, check_file)
    if not os.path.isfile(check_path):
        sys.stderr.write("Warning: check {0} not found\n".format(check_path))

    if "CHPL_HOME" in os.environ:
        if os.path.abspath(os.environ["CHPL_HOME"]) != chpl_home_dir:
            # to be sure, check that the inode numbers of our check file match
            env_check = os.path.join(os.environ["CHPL_HOME"], check_file)
            rel_check = os.path.join(chpl_home_dir, check_file)
            if os.path.samefile(env_check, rel_check):
                # No warning, it's OK, they are the same file.
                pass
            else:
                sys.stderr.write("Warning: check {0} not found\n".format(check_path))
                sys.stderr.write("Warning: Mismatched CHPL_HOME; got {0} but expected {1}\n".format(os.path.abspath(os.environ["CHPL_HOME"]), chpl_home_dir))

        chpl_home_dir = os.environ["CHPL_HOME"] # use enviro var spelling of it
    else:
        os.environ["CHPL_HOME"] = chpl_home_dir

    os.environ["CHPL_MAKE_HOME"] = chpl_home_dir
    if "CHPL_RUNTIME_LIB" in os.environ:
        os.environ["CHPL_MAKE_RUNTIME_LIB"] = os.environ["CHPL_RUNTIME_LIB"]

    if "CHPL_RUNTIME_INCL" in os.environ:
        os.environ["CHPL_MAKE_RUNTIME_INCL"] = os.environ["CHPL_RUNTIME_INCL"]

    if "CHPL_THIRD_PARTY" in os.environ:
        os.environ["CHPL_MAKE_THIRD_PARTY"] = os.environ["CHPL_THIRD_PARTY"]

    make = chpl_make.get()

    orig_make = make
    # Do not print directory changes.
    make = [make, "--no-print-directory"]

    # Make reasonable defaults for environment settings
    os.environ["COMP_GEN_WARN"] = "0"
    os.environ["COMP_GEN_DEBUG"] = "0"
    os.environ["COMP_GEN_OPT"] = "0"
    os.environ["COMP_GEN_SPECIALIZE"] = "0"
    os.environ["COMP_GEN_IEEE_FLOAT"] = "1"

    actions = parseArguments()

    make_helper = make + ["-f", chpl_home_dir + "/runtime/etc/Makefile.include"]

    for a in actions:
        if a == "home":
            sys.stdout.write("{0}\n".format(chpl_home_dir))
        elif a == "make":
            sys.stdout.write("{0}\n".format(orig_make))
        elif a == "llvm":
            os.environ["CHPL_LLVM_CODEGEN"] = "llvm"
            orig_target_compiler = chpl_compiler.get('target', llvm_mode='orig')
            target_compiler = chpl_compiler.get('target', llvm_mode='llvm')
            os.environ["CHPL_ORIG_TARGET_COMPILER"] = orig_target_compiler
            os.environ["CHPL_TARGET_COMPILER"] = target_compiler

            llvm = ""
            if "CHPL_LLVM" in os.environ:
                llvm = os.environ["CHPL_LLVM"]
            if "CHPL_MAKE_LLVM" in os.environ:
                llvm = os.environ["CHPL_MAKE_LLVM"]
            if llvm == "":
                llvm = chpl_llvm.get()
            if llvm == "none":
                sys.stderr.write("Cannot get --llvm configuration with CHPL_LLVM=none\n")
                sys.exit(1)
        elif a == "compilecc":
            mysystem(make_helper, "printcompileline")
        elif a == "compilecxx":
            mysystem(make_helper, "printcxxcompileline")
        elif a == "compiler":
            mysystem(make_helper, "printccompiler")
        elif a == "cflags":
            mysystem(make_helper, "printcflags")
        elif a == "cxxflags":
            mysystem(make_helper, "printcxxflags")
        elif a == "includesanddefines":
            mysystem(make_helper, "printincludesanddefines")
        elif a == "libraries":
            mysystem(make_helper, "printlibraries")
        elif a == "linker":
            mysystem(make_helper, "printlinker")
        elif a == "linkershared":
            mysystem(make_helper, "printlinkershared")
        elif a == "maino":
            mysystem(make_helper, "printmaino")
        elif a == "llvminstalldir":
            mysystem(make_helper, "printllvminstall")
        elif a == "clangcc":
            mysystem(make_helper, "printclangcc")
        elif a == "clangcxx":
            mysystem(make_helper, "printclangcxx")
        elif a == "clangsysroot":
            mysystem(make_helper, "printclangcxx")
            llvminstall = myrun(make_helper, "printllvminstall")
            llvminstall = llvminstall.strip()
            fname = os.path.join(llvminstall, "configured-clang-sysroot-arguments")
            if os.path.isfile(fname):
                with open(fname) as f:
                    for line in f:
                        sys.stdout.write(line)
        elif a == "launcherlibdir":
          mysystem(make_helper, "printlauncherlibdir")
        elif a == "multilocale-lib-deps":
          mysystem(make_helper, "printmultilocalelibdeps")

if __name__ == '__main__':
    main()
