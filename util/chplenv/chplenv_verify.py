"""
Verifies that the current environment is set up correctly for building Chapel, and that once built the compiler will function correctly.
"""

import functools
from utils import try_run_command, memoize
import tempfile
import shutil
import sys
import os
import shlex


def log(*args, **kwargs):
    kwargs = {**kwargs, "file": sys.stderr}
    verbose = False
    if "verbose" in kwargs:
        verbose = kwargs["verbose"]
        del kwargs["verbose"]
    if verbose:
        print(*args, **kwargs)


class VerificationPass:
    def __init__(self, chplenv, verbose=False):
        self.chplenv = chplenv
        self.verbose = verbose
        self.msg = None

    def log(self, *args, **kwargs):
        log("  ", *args, **kwargs, verbose=self.verbose)

    def verify(self):
        return False

    def skipif(self):
        """
        Returns True if this pass should be skipped, False otherwise
        """
        return False

    def explain(self):
        return self.msg or "Unknown error"


class FileCleanupManager:
    def __init__(self, *files):
        self.files = list(files)

    def track(self, f):
        self.files.append(f)

    def cleanup(self):
        for f in self.files:
            shutil.rmtree(f)

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        self.cleanup()


class TestCompile(VerificationPass):
    """
    Abstract base class for testing compiling programs
    """

    def _suffix(self):
        """
        Returns the suffix of the file to be created

        Extended by subclasses
        """
        return ""

    def _lang(self):
        """
        Returns the language of the file to be created

        Extended by subclasses
        """
        return ""

    def _compiler_env_var(self):
        """
        Returns the compiler environment variable to be used

        Extended by subclasses
        """
        return ""

    def _compiler(self):
        """
        Returns the compiler to be used, uses _compiler_env_var by default

        Extended by subclasses
        """
        cc_var = self._compiler_env_var()
        if not cc_var:
            return []
        cc = self.chplenv.get(cc_var)
        if not cc:
            self.msg = "{0} is missing".format(cc_var)
            return []
        return shlex.split(cc)

    def _compiler_args(self):
        """
        Returns the compiler arguments to be used

        Extended by subclasses
        """
        return []

    def _program(self):
        """
        Returns the program to be compiled

        Extended by subclasses
        """
        return ""

    def verify(self):
        tmpdir = tempfile.mkdtemp()

        hello_file = tempfile.mkstemp(
            dir=tmpdir, suffix=self._suffix(), text=True
        )
        with open(hello_file[1], "w") as f:
            f.write(self._program())

        with FileCleanupManager(tmpdir) as fcm:
            cmd = (
                self._compiler()
                + [hello_file[1], "-o", f"{tmpdir}/a.out"]
                + self._compiler_args()
            )
            self.log("Trying to compile with '{}'".format(" ".join(cmd)))
            succ, ret, out, _ = try_run_command(cmd, combine_output=True)
            if not succ or ret != 0:
                out = out if out is not None else "unknown error"
                self.msg = "Failed to compile program: {}".format(out)
                return False

            # Run the program
            cmd = [f"{tmpdir}/a.out"]
            self.log("Trying to run with '{}'".format(" ".join(cmd)))
            succ, ret, out, _ = try_run_command(cmd, combine_output=True)
            if not succ or ret != 0:
                out = out if out is not None else "unknown error"
                self.msg = "Failed to run program: {}".format(out)
                return False

        return True

    def explain(self):
        if not self.msg:
            compiler = self._compiler()
            if not compiler:
                self.msg = "No compiler found"
            self.msg = "{} compiler '{}' cannot compile programs".format(
                self._lang(), compiler[0]
            )
        return super().explain()


class TestHostCompile(TestCompile):
    """
    Try and compile a hello world program using CHPL_HOST_CXX
    """

    def skipif(self):
        if os.getenv("CHPLENV_SKIP_HOST", None) is not None:
            return True
        return super().skipif()

    def _suffix(self):
        return ".cpp"

    def _lang(self):
        return "C++"

    def _compiler_env_var(self):
        return "CHPL_HOST_CXX"

    def _program(self):
        return """
#include <iostream>
int main() {
  std::cout << "Hello, World!" << std::endl;
  return 0;
}
"""

def missing_llvm(chplenv):
    """
    Check if LLVM is missing or not
    if CHPL_LLVM is bundled, return True if its has not been built
    if CHPL_LLVM is none and CHPL_LLVM_SUPPORT is bundled, return True if has not been built
    """
    if (chplenv.get("CHPL_LLVM") == "bundled" or
        (chplenv.get("CHPL_LLVM") == "none" and
        chplenv.get("CHPL_LLVM_SUPPORT") == "bundled")):
        llvm_config = chplenv.get("CHPL_LLVM_CONFIG")
        if not llvm_config or not os.path.exists(llvm_config):
            return True
    return False


class TestTargetCompile(TestCompile):

    def skipif(self):
        if self.chplenv.get("CHPL_TARGET_COMPILER") == "llvm" and missing_llvm(self.chplenv):
            return True
        return super().skipif()

class TestTargetCompileCC(TestTargetCompile):
    """
    Try and compile a hello world program using CHPL_TARGET_CC
    """

    def _suffix(self):
        return ".c"

    def _lang(self):
        return "C"

    def _compiler_env_var(self):
        return "CHPL_TARGET_CC"

    def _program(self):
        return """
#include <stdio.h>
int main() {
    printf("Hello, World!\\n");
    return 0;
}
"""


class TestTargetCompileCXX(TestTargetCompile):
    """
    Try and compile a hello world program using CHPL_TARGET_CXX
    """

    def _suffix(self):
        return ".cpp"

    def _lang(self):
        return "C++"

    def _compiler_env_var(self):
        return "CHPL_TARGET_CXX"

    def _program(self):
        return """
#include <iostream>
int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
"""


class TestHostCanFindLLVM(TestCompile):
    """
    Try and compile a hello world program using CHPL_HOST_CXX that includes a header from LLVM
    """

    def skipif(self):
        if os.getenv("CHPLENV_SKIP_HOST", None) is not None:
            return True
        if missing_llvm(self.chplenv):
            return True
        return super().skipif()

    def _suffix(self):
        return ".cpp"

    def _lang(self):
        return "C++"

    def _compiler_env_var(self):
        return "CHPL_HOST_CXX"

    def _compiler_args(self):
        comp_args = (
            self.chplenv.get("CHPL_HOST_BUNDLED_COMPILE_ARGS", "") + " " +
            self.chplenv.get("CHPL_HOST_SYSTEM_COMPILE_ARGS", "")
        )
        link_args = (
            self.chplenv.get("CHPL_HOST_BUNDLED_LINK_ARGS", "") + " " +
            self.chplenv.get("CHPL_HOST_SYSTEM_LINK_ARGS", "")
        )

        # strip out jemalloc to avoid linker warnings on some systems
        if "jemalloc" in link_args:
            link_args = link_args.replace("-ljemalloc", "")

        return (
            super()._compiler_args()
            + shlex.split(comp_args)
            + shlex.split(link_args)
        )

    def _program(self):
        llvm_version = self.chplenv.get("CHPL_LLVM_VERSION", "0")
        if self.chplenv.get("CHPL_LLVM") != "none":
            return f"""
#include "clang/Basic/Version.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Config/llvm-config.h"
#include <iostream>
#if LLVM_VERSION_MAJOR != {llvm_version}
#error Mismatched LLVM version
#endif
int main() {{
    std::cout << clang::getLLVMRevision() << std::endl;
    llvm::LLVMContext Context;
    llvm::Module M("mymod", Context);
    return 0;
}}
"""
        else:
            return f"""
#include "llvm/Support/raw_ostream.h"
#include "llvm/Config/llvm-config.h"
#include <iostream>
#if LLVM_VERSION_MAJOR != {llvm_version}
#error Mismatched LLVM version
#endif
int main() {{
    std::cout << "Hello, World!" << std::endl;
    llvm::outs() << "Hello, LLVM!\\n";
    return 0;
}}
"""


    def explain(self):
        compiler = self._compiler()
        if not compiler:
            return "No compiler found"
        return "{}{}{} cannot find LLVM headers".format(
            super().explain(), os.linesep, compiler[0])


passes = [
    ("TestHostCompile", TestHostCompile),
    ("TestTargetCompileCC", TestTargetCompileCC),
    ("TestTargetCompileCXX", TestTargetCompileCXX),
    ("TestHostCanFindLLVM", TestHostCanFindLLVM),
]


def verify(chplenv, verbose=False):

    mychplenv = {}
    # Remove leading/trailing whitespace from keys
    for k, v in chplenv.items():
        mychplenv[k.strip()] = v

    global passes
    for name, cls in passes:
        p = cls(mychplenv, verbose=verbose)
        if p.skipif():
            log(f"Skipping verification pass {name}", verbose=verbose)
        else:
            log(f"Running verification pass {name}", verbose=verbose)
            if not p.verify():
                return (False, p.explain())
    return (True, None)
