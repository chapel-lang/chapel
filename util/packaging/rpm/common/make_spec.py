import sys
from string import Template


class MyTemplate(Template):
    delimiter = "@@"


basename, chapel_version, pkg_version, os_name, arch = sys.argv[1:6]

substitutions = {
    "BASENAME": basename,
    "CHAPEL_VERSION": chapel_version,
    "PACKAGE_VERSION": pkg_version,
}

template = ""
template_file = "spec.template"
with open(template_file, "r") as f:
    template = f.read()

src = MyTemplate(template)
result = src.substitute(substitutions)

output_file = f"{basename}.spec"
with open(output_file, "w") as f:
    f.write(result)
