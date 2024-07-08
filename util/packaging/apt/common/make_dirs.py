import os
import sys
from string import Template
from package_name import package_name


class MyTemplate(Template):
    delimiter = "@@"


basename, chapel_version, pkg_version, os_name, arch = sys.argv[1:6]
deb_name = package_name(basename, chapel_version, pkg_version, os_name, arch)

dirs = [
    deb_name,
    f"{deb_name}/usr",
    f"{deb_name}/usr/bin",
    f"{deb_name}/usr/lib",
    f"{deb_name}/usr/share",
    f"{deb_name}/DEBIAN",
]
for d in dirs:
    os.makedirs(d, exist_ok=True)


substitutions = {
    "DEB_NAME": deb_name,
    "CHAPEL_VERSION": chapel_version,
    "TARGETARCH": arch,
}

template = ""
template_file = "control.template"
with open(template_file, "r") as f:
    template = f.read()

src = MyTemplate(template)
result = src.substitute(substitutions)

output_file = f"{deb_name}/DEBIAN/control"
with open(output_file, "w") as f:
    f.write(result)
