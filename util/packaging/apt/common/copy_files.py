import sys
import subprocess as sp
from package_name import package_name

basename, chapel_version, pkg_version, os_name, arch = sys.argv[1:6]
deb_name = package_name(basename, chapel_version, pkg_version, os_name, arch)

files = [
    "/usr/bin/chpl",
    "/usr/bin/chpldoc",
    "/usr/bin/mason",
    "/usr/bin/chplcheck",
    "/usr/bin/chpl-language-server",
]
dirs = [
    "/usr/lib/chapel",
    "/usr/share/chapel",
]
for f in files:
    sp.check_call(["cp", f, f"{deb_name}{f}"])
for d in dirs:
    sp.check_call(["cp", "-r", d, f"{deb_name}{d}"])
