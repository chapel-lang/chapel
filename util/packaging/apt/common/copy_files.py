import sys
import os
import subprocess as sp
from package_name import package_name

basename, chapel_version, pkg_version, os_name, arch = sys.argv[1:6]
deb_name = package_name(basename, chapel_version, pkg_version, os_name, arch)

is_minimal = "-minimal" in basename

files = [
    "/usr/bin/chpl"
]
if not is_minimal:
    files.extend([
        "/usr/bin/chpldoc",
        "/usr/bin/mason",
        "/usr/bin/chplcheck",
        "/usr/bin/chpl-language-server",
        "/usr/bin/chpl-shim",
    ])
dirs = [
    "/usr/lib/chapel",
    "/usr/lib/cmake/chpl",
    "/usr/share/chapel",
]
for f in files:
    sp.check_call(["cp", f, f"{deb_name}{f}"])
for d in dirs:
    dirname = os.path.dirname(d)
    os.makedirs(f"{deb_name}{dirname}", exist_ok=True)
    sp.check_call(["cp", "-r", d, f"{deb_name}{d}"])
