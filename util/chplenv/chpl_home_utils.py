""" Access to home variables """

import optparse
import os
import re
import sys

import chpl_bin_subdir, chpl_python_version, overrides
from utils import memoize


@memoize
def get_chpl_home():
    return overrides.get_chpl_home(overrides)

@memoize
def get_chpl_runtime_incl():
    prefix = get_prefix_install_prefix()
    if prefix is None:
        default = os.path.join(get_chpl_home(), 'runtime', 'include')
    else:
        chpl_vers = get_chpl_version_from_install()
        assert chpl_vers is not None
        default = os.path.join(prefix, 'lib', 'chapel', chpl_vers, 'runtime', 'include')
    chpl_runtime_incl = overrides.get('CHPL_RUNTIME_INCL', default)
    return chpl_runtime_incl

@memoize
def get_chpl_runtime_lib():
    prefix = get_prefix_install_prefix()
    if prefix is None:
        default = os.path.join(get_chpl_home(), 'lib')
    else:
        chpl_vers = get_chpl_version_from_install()
        assert chpl_vers is not None
        default = os.path.join(prefix, 'lib', 'chapel', chpl_vers, 'runtime', 'lib')
    chpl_runtime_lib = overrides.get('CHPL_RUNTIME_LIB', default)
    return chpl_runtime_lib

@memoize
def get_chpl_third_party():
    prefix = get_prefix_install_prefix()
    if prefix is None:
        default = os.path.join(get_chpl_home(), 'third-party')
    else:
        chpl_vers = get_chpl_version_from_install()
        assert chpl_vers is not None
        default = os.path.join(prefix, 'lib', 'chapel', chpl_vers, 'third-party')
    chpl_third_party = overrides.get('CHPL_THIRD_PARTY', default)
    return chpl_third_party

install_path_regex = re.compile(
    '^(.*){}share{}chapel{}(\\d+\\.\\d+)$'.format(os.path.sep,
                                                  os.path.sep,
                                                  os.path.sep))

@memoize
def get_chpl_version_from_install():
    if get_prefix_install_prefix():
        chpl_home = get_chpl_home()
        m = install_path_regex.match(chpl_home)
        # if we are in an installed directory, this should always match
        assert m is not None
        return m.group(2)
    return None


@memoize
def get_prefix_install_prefix():
    """
    If Chapel is installed as a prefix install (with `./configure --prefix` etc)
    then this function returns the prefix where it is installed.
    If it's not installed or installed as a `--chpl-home` install, it returns `None`
    """
    chpl_home = get_chpl_home()

    frontend_path = os.path.join(chpl_home, 'frontend')
    doc_path = os.path.join(chpl_home, 'doc')
    has_frontend = os.path.exists(frontend_path) and os.path.isdir(frontend_path)
    has_doc = os.path.exists(doc_path) and os.path.isdir(doc_path)
    is_source_dir = has_frontend and has_doc
    if is_source_dir:
        return None

    bin_path = os.path.join(chpl_home, 'bin')
    lib_path = os.path.join(chpl_home, 'lib')
    has_bin = os.path.exists(bin_path) and os.path.isdir(bin_path)
    has_lib = os.path.exists(lib_path) and os.path.isdir(lib_path)
    is_chpl_home_install = has_bin and has_lib
    if is_chpl_home_install:
        return None

    # if we reach this point, we can be fairly confident that this is a prefix-install
    # the regex should always match in this case
    m = install_path_regex.match(chpl_home)
    if m is None:
        return None

    prefix = m.group(1)
    if prefix == "":
        prefix = os.path.sep
    return prefix

# Fix paths in the passed string according to the provided list of pairs
# in tofix
def add_vars_to_paths_helper(s, tofix):
    lines = s.splitlines()
    output = [ ]

    for line in lines:
        # Find things that look like absolute paths
        # Note that this needs to handle e.g. -I/some/directory
        # (and not think it's a relative path starting with I)

        pattern = r'/[^ ]+'

        fixed = line

        for m in re.findall(pattern, line):
            origpath = m
            path = os.path.realpath(origpath)

            if os.path.isfile(path):
              # Leave out the filename, since we're only trying to replace
              # directories. This prevents clang++ from being replaced
              # with e.g. clang-6.0 when clang++ is a symbolic link.
              origpath = os.path.dirname(origpath)
              path = os.path.dirname(path)

            for kv in tofix:
              key = kv[0]
              val = kv[1]

              if path.startswith(val):
                rel = os.path.relpath(path, val)
                fixed = fixed.replace(origpath, key + "/" + rel)
                break

        output.append(fixed)

    return "\n".join(output)

# Fix paths in the passed string to use $CHPL_HOME, $CHPL_THIRD_PARTY, etc
def add_vars_to_paths(s):
    tofix = [ ]

    tofix.append( ['$CHPL_RUNTIME_LIB', get_chpl_runtime_lib()] )
    tofix.append( ['$CHPL_RUNTIME_INCL', get_chpl_runtime_incl()] )
    tofix.append( ['$CHPL_THIRD_PARTY', get_chpl_third_party()] )
    tofix.append( ['$CHPL_HOME', get_chpl_home()] )

    return add_vars_to_paths_helper(s, tofix)

# Get the chpl-venv install directory:
# $CHPL_HOME/third-party/chpl-venv/install/chpldeps
@memoize
def get_chpldeps(chapel_py=False):
    base = os.path.join(get_chpl_third_party(), "chpl-venv", "install")
    if not chapel_py:
        chpl_venv = os.path.join(base, "chpldeps")
    else:
        # for 3.10+, we can use chpl-frontend-py-deps
        # for other versions, we have to use chpl-frontend-py-deps-pyX.Y
        name = "chpl-frontend-py-deps"
        minor_version = sys.version_info.minor
        if minor_version < 10:
            name += "-py" + str(minor_version)
        chpl_venv = os.path.join(base, name)
    return chpl_venv

@memoize
def using_chapel_module():
    chpl_home = overrides.get('CHPL_HOME', None)
    chpl_module_home = os.environ.get('CHPL_MODULE_HOME', None)
    if chpl_home and chpl_module_home:
        return os.path.normpath(chpl_home) == os.path.normpath(chpl_module_home)
    return False


def _main():
    parser = optparse.OptionParser()
    parser.add_option('--home', action='store_const',
                      dest='func', const=get_chpl_home)
    parser.add_option('--third-party', action='store_const',
                      dest='func', const=get_chpl_third_party)
    parser.add_option('--chpldeps', action='store_const',
                      dest='func', const=get_chpldeps)
    parser.add_option(
        "--chpldeps-chapel-py",
        action="store_const",
        dest="func",
        const=lambda: get_chpldeps(True),
    )
    parser.add_option('--using-module', action='store_const',
                      dest='func', const=using_chapel_module)
    (options, args) = parser.parse_args()

    if options.func:
        sys.stdout.write("{0}\n".format(options.func()))


if __name__ == '__main__':
    _main()
