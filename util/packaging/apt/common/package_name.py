import sys

def package_name(basename, chapel_version, pkg_version, os_name, arch):
    deb_name = f"{basename}-{chapel_version}-{pkg_version}.{os_name}.{arch}"
    return deb_name

def short_version(chapel_version):
    """
    Given MAJOR.MINOR.PATCH return MAJOR.MINOR
    """
    return chapel_version.rsplit(".", 1)[0]


if __name__ == '__main__':
    basename, chapel_version, pkg_version, os_name, arch = sys.argv[1:6]
    print(package_name(basename, chapel_version, pkg_version, os_name, arch))
