#!/usr/bin/env python3

import os
import re

directories = ["current", "../vagrant/current"]

def gather_provision_script_cmds(path):
    cmds = [ ]
    with open(path) as file:
        for line in file:
            line = line.strip()
            if line.startswith("#!"):
                pass # ignore shebang line
            elif line:
                cmds.append(line)
    return cmds

def title(name):
    name = name.capitalize()
    # fix capitalization
    if name == "Freebsd":
        name = "FreeBSD"
    if name == "Centos":
        name = "CentOS"
    if name == "Opensuse":
        name = "OpenSuse"
    # handle Debian nicknames
    if name == "Buster":
        name = '10 "Buster"'
    if name == "Bullseye":
        name = '11 "Bullseye"'
    if name == "Bookworm":
        name = '12 "Bookworm"'
    # handle Ubuntu nicknames
    if name == "Bionic":
        name = '18.04 "Bionic Beaver"'
    if name == "Focal":
        name = '20.04 "Focal Fossa"'
    if name == "Jammy":
        name = '22.04 "Jammy Jellyfish"'
    if name == "Impish":
        name = '21.10 "Impish Indri"'
    if name == "Kinetic":
        name = '22.10 "Kinetic Kudu"'
    return name

def fixname(subdir):
    name = os.path.basename(subdir)
    # remove -cloud-base from e.g. fedora-32-cloud-base
    if name.endswith("-cloud-base"):
        name = name[:name.find("-cloud-base")]
    # remove -STABLE from e.g. freebsd-FreeBSD-12.2-STABLE
    if name.endswith("-STABLE"):
        name = name[:name.find("-STABLE")]
    # remove first freebsd in freebsd-FreeBSD-12.2-STABLE
    if name.startswith("freebsd-FreeBSD-"):
        name = name[len("freebsd-"):]
    # remove 64 in ubuntu-impish64
    if name.endswith("64"):
        name = name[:name.find("64")]

    parts = name.split("-")
    adj = [ ]
    for part in parts:
        if part.endswith("linux"):
            # e.g. rockylinux -> Rocky Linux
            tmp = part[:part.find("linux")]
            adj.append(title(tmp))
            adj.append("Linux")
        elif re.search('\d$', part):
            sections = re.split('([0-9.]+)', part)
            for s in sections:
                s = s.strip()
                if s:
                    adj.append(title(s))
        else:
            adj.append(title(part))

    return " ".join(adj)

def extract_sdef_commands(sdef):
    cmds = [ ]
    with open(sdef) as file:
        inPost = False
        for line in file:
            if line.startswith("%post"):
                inPost = True
            elif line.startswith("%"):
                inPost = False
            elif inPost:
                line = line.strip()
                if line.startswith("/provision-scripts/"):
                    spath = line[1:] # remove leading /
                    subcmds = gather_provision_script_cmds(spath)
                    cmds.extend(subcmds)
                elif line:
                    cmds.append(line)
    return cmds

def extract_vfile_commands(vfile):
    cmds = [ ]
    with open(vfile) as file:
        for line in file:
            line = line.strip()
            if 'provision-scripts' in line:
                parts = line.split('"')
                for part in parts:
                    if 'provision-scripts' in part:
                        spath = part[part.find('provision-scripts'):]
                        if ('git-clone-chapel.sh' in spath or
                            'gmake-chapel-quick.sh' in spath or
                            'make-chapel-quick.sh' in spath):
                            pass # skip these
                        else:
                            subcmds = gather_provision_script_cmds(spath)
                            cmds.extend(subcmds)
    return cmds

subdirs = [ ]
for d in directories:
    for subdir in os.listdir(d):
        subpath = os.path.join(d, subdir)
        subdirs.append(subpath)
subdirs.sort(key=os.path.basename)

for subpath in subdirs:
    if "nollvm" in subpath:
        continue # skip these configurations
    if "homebrew" in subpath:
        continue # skip these configurations (this script would need to be improved)
    if "generic-x32-debian11" in subpath:
        continue # skip this one, redudant with other debian ones

    cmds = [ ]
    if os.path.isdir(subpath):
        name = fixname(subpath)
        print("  * " + name + '::')
        sdef = os.path.join(subpath, "singularity.def")
        vfile = os.path.join(subpath, "Vagrantfile")
        if os.path.exists(sdef):
            cmds = extract_sdef_commands(sdef)
        elif os.path.exists(vfile):
            cmds = extract_vfile_commands(vfile)
        else:
            print("NO CMDS FILE FOUND!")

    for cmd in cmds:
        if cmd.startswith("#"):
            pass # ignore comments
        else:
            words = cmd.split()
            adj = [ ]
            for word in words:
                if word == "-y" or word == "--yes" or word == "--noconfirm":
                    pass # filter these out
                elif word == "/home/vagrant/.bashrc":
                    adj.append("~/.bashrc")
                else:
                    adj.append(word)
            print("      sudo " + " ".join(adj))
    print()
