import sys
import argparse


def package_name(basename, chapel_version, pkg_version, os_name, arch):
    deb_name = f"{basename}-{chapel_version}-{pkg_version}.{os_name}.{arch}"
    return deb_name


def short_version(chapel_version):
    """
    Given MAJOR.MINOR.PATCH return MAJOR.MINOR
    """
    return chapel_version.rsplit(".", 1)[0]


if __name__ == "__main__":

    parser = argparse.ArgumentParser(description="Generate package name")
    parser.add_argument("--package-name", action="store_true")
    parser.add_argument("--short-version", action="store_true")
    args, raw_args = parser.parse_known_intermixed_args(sys.argv)

    basename, chapel_version, pkg_version, os_name, arch = raw_args[1:]

    if args.short_version:
        print(short_version(chapel_version))
    elif args.package_name:
        print(
            package_name(basename, chapel_version, pkg_version, os_name, arch)
        )
    else:
        # same as args.package_name
        print(
            package_name(basename, chapel_version, pkg_version, os_name, arch)
        )
