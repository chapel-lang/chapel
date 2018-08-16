#!/usr/bin/env python3
import base64
import requests
from pprint import pprint
import re

# get authentication token for GitHub
secret = open("secret-token.txt", "r+")
info = secret.readline().split(",")
user = info[0].strip()
token = info[1].strip()
secret.close()

f = open("../rst/tools/mason/package-list.rst", "w+")

f.write(".. _package-list:\n\n")

f.write("============\n")
f.write("Package List\n")
f.write("============\n\n")

f.write("The following is a list of packages currently available in Mason through the mason-registry.\n\n")

base_url = "https://api.github.com/repos/chapel-lang/mason-registry/contents/Bricks?ref=master"
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

        for toml_file in content:
            download = requests.get(toml_file["download_url"], auth=(user, token))
            toml = download.text
            for line in re.findall("(\w+) = ([^\s]+)", toml):
                if line[0] == "author":
                    package["author"] = line[1].strip('"')
                elif line[0] == "source":
                    package["source"] = line[1].strip('"')
                elif line[0] == "chplVersion":
                    package["chpl_version"] = line[1].strip('"')
                elif line[0] == "version":
                    package["versions"].append(line[1].strip('"'))


        # put packages in top lvl package dict
        packages[package["name"]] = package


    # write package-list.rst
    for name, pack in packages.items():
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

else:
    print('Error requesting content from GitHub')
    print('Could not write package-list.rst')

# close package-list.rst
f.close()
