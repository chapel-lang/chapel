import requests
import toml

"""
This script requests the GitHub API for the Mason.toml's
found in the Mason Registry so that a package list can be
automatically generated.

An authentication token is needed from github in order to
make more than a few requests every hour.

Dependencies
  Python3,
  requests 2.19.1,
  toml 0.9.4
"""

def getAuthToken():
    """get authentication token for GitHub"""
    secret = open("secret-token.txt", "r+")
    info = secret.readline().split(",")
    user = info[0].strip()
    token = info[1].strip()
    secret.close()
    return user, token


def requestPackageInfo():
    """Contacts Github API to parse Mason.tomls of packages in Mason-Registry"""
    base_url = "https://api.github.com/repos/chapel-lang/mason-registry/contents/Bricks?ref=master"

    user, token = getAuthToken()
    req = requests.get(base_url, auth=(user, token))
    packages = {}
    if req.status_code == requests.codes.ok:
        registry = req.json()

        # get all packages currently in the registry
        for brick in registry:
            package = {}
            package["name"] = brick["name"]
            package["versions"] = []
            content = requests.get(brick["url"], auth=(user, token))
            content = content.json()

            # get contents of toml files and parse values
            for toml_file in content:
                download = requests.get(toml_file["download_url"],
                                        auth=(user, token))
                toml_file = toml.loads(download.text)
                package["author"] = toml_file["brick"]["author"]
                package["source"] = toml_file["brick"]["source"]
                package["chpl_version"] = toml_file["brick"]["chplVersion"]
                package["versions"].append(toml_file["brick"]["version"])

            # put packages in top lvl package dict
            packages[package["name"]] = package

    else:
        print('Error requesting content from GitHub')
        print('Could not write package-list.rst')

    return packages


def writePackageList(packages, path="../rst/tools/mason/package-list.rst"):
    """Writes out the package-list.rst for package documentation"""

    f = open(path, "w+")

    f.write(".. _package-list:\n\n")

    f.write("============\n")
    f.write("Package List\n")
    f.write("============\n\n")

    f.write("""The following is a list of packages currently
    available in Mason through the mason-registry.\n\n""")

    for name, pack in sorted(packages.items()):
        if not name.startswith("_"):
            f.write(name + "\n")
            f.write("~"*len(name) + "\n")
            f.write("Author: \n")
            f.write("     | " + pack["author"] + "\n")
            f.write("Repository: \n")
            f.write("     | " + pack["source"] + "\n")
            f.write("Chapel Version: \n")
            f.write("     | " + pack["chpl_version"] + "\n")
            f.write("Versions: \n")
            for version in pack["versions"]:
                f.write("     | " + version + "\n")
                f.write("\n")
                f.write("\n")

    # close package-list.rst
    f.close()

if __name__ == "__main__":
    package_list = requestPackageInfo()
    writePackageList(package_list)
