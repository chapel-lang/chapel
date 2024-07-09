import re
import subprocess as sp
import typing
import os
import glob
import sys
from package_name import short_version

basename, chapel_version, pkg_version, os_name, arch = sys.argv[1:6]
short_chapel_version = short_version(chapel_version)

get_path_pat = re.compile(r"(?:RUNPATH|RPATH)=(.*)")
version_path_pat = re.compile(f"^/home/user/chapel-{chapel_version}/(.*)$")


def get_path(s: str) -> typing.List[str]:
    m = re.search(get_path_pat, s)
    if m:
        paths = m.group(1).split(":")
        paths = [p.strip() for p in paths if p.strip() != ""]
        return paths
    return []


files = ["chpl", "chpldoc", "mason"]
files = [os.path.join("/usr/bin", f) for f in files]
files = [f for f in files if os.path.exists(f)]

# find all .so in the old lib path
for f in glob.glob(
    f"/usr/lib/chapel/{short_chapel_version}/**/*.so", recursive=True
):
    files.append(f)

new_prefix = f"/usr/lib/chapel/{short_chapel_version}"
for f in files:
    # check if the file has a runpath
    cp = sp.run(
        ["chrpath", "-l", f], encoding="utf-8", stdout=sp.PIPE, stderr=sp.STDOUT
    )
    if cp.returncode != 0:
        if "no rpath or runpath tag found" in cp.stdout:
            print(f"No rpath or runpath tag found in {f}")
            continue
        print(f"Error running chrpath on {f}")
        print(cp.stdout)
        exit(1)
    path = cp.stdout.strip()
    path = get_path(path)
    new_path = []
    # clean path
    for p in path:
        if p == "/usr/lib64":
            continue
        m = re.match(version_path_pat, p)
        if m:
            suffix = m.group(1)
            if suffix.startswith("lib/compiler/"):
                suffix = "compiler"
            new_path.append(f"{new_prefix}/{suffix}")
        else:
            new_path.append(p)

    if len(new_path) == 0:
        sp.check_call(["chrpath", "-d", f])
    else:
        # remove duplicates
        new_path = list(set(new_path))

        path = ":".join(new_path)
        sp.check_call(["chrpath", "-r", path, f])
    print("=" * 80)

# remove uses of /home/user/chapel-VERSION from chpl-deps
# sudo find /usr/lib/chapel -type f -exec bash -c "grep -i -a -o -l /home/user/chapel-VERSION/third-party/chpl-venv/build/build-venv/bin/python3 \"{}\"" \;
folder = f"/usr/lib/chapel/{short_chapel_version}/third-party/chpl-venv/install/chpldeps/bin"
if os.path.exists(folder):
    for f in os.listdir(folder):
        f = os.path.join(folder, f)
        if not os.path.isfile(f):
            continue
        with open(f, "r") as fd:
            lines = fd.readlines()
        with open(f, "w") as fd:
            for line in lines:
                line = line.replace(
                    f"/home/user/chapel-{chapel_version}/third-party/chpl-venv/build/build-venv/bin/python3",
                    "/usr/bin/python3",
                )
                fd.write(line)
        print(f"Fixed {f}")
        print("=" * 80)
