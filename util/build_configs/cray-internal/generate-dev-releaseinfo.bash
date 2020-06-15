#!/bin/bash

# Generate a draft/placeholder release_info.txt file.
# Writes to stdout

set -e

cwd=$( cd $(dirname "${BASH_SOURCE[0]}" ) && pwd )
source $cwd/../functions.bash

# Accept command line parameters of the form NAME=Value. Or use environment variables.

while test $# -gt 0; do
    case $1 in ( *=* ) export "$1" ;; esac
    shift
done

# Cray-internal-specific shell variables

source $cwd/common.bash

# Generate the file

cat <<EOF
Now Available: HPE Chapel Compiler $major.$minor

This is a draft/placeholder file for development purposes only.

TODO:   Add "highlights of this release"
TODO:   Add the official release dates

    Hewlett Packard Enterprise (HPE) and the Chapel open-source
    community are pleased to announce the release of version
    $src_version of the Chapel compiler.  Chapel is a parallel
    programming language designed to improve the productivity of
    high-end computer users while also serving as a portable parallel
    programming model for use on multicore laptops, commodity
    clusters, and the cloud.

Chapel $major.$minor
===========

    Chapel is a productive parallel programming language whose design
    and development are being led by HPE in a collaborative
    manner with members of academia, computing labs, and industry.
    Chapel's overall goal is to improve programmer productivity on
    large-scale systems.  Its implementation is being pursued in an
    open-source manner at GitHub under the Apache 2.0 license.  We
    encourage feedback from users in order to help improve Chapel's
    utility, generality, and adoption.

    The highlights of this release are as follows:

    (TODO)

    And of course, this release contains a large number of error
    message improvements, bug fixes, and feature enhancements.  See
    \$CHPL_HOME/CHANGES.md for the full list.

    To get started with Chapel, load the 'chapel' module and refer to
    \$CHPL_HOME/doc/html/platforms/cray.html for Cray-specific
    documentation.  You may also want to refer to the general Chapel
    documentation hierarchy, described in \$CHPL_HOME/doc/README.rst
    or found online at: https://chapel-lang.org/docs/

    For general information on Chapel, please refer to the Chapel web
    page at: https://chapel-lang.org

    Interested users are encouraged to ask questions and join
    discussions in a variety of forums (Stack Overflow, Gitter,
    mailing lists, etc.) listed at https://chapel-lang.org/users.html.
    Questions about Chapel can be directed to the Chapel team at
    chapel_info@cray.com.  To file bugs or feature requests against
    Chapel, refer to \$CHPL_HOME/doc/html/usingchapel/bugs.html or
    https://chapel-lang.org/bugs.html.


How Do I Get It?
----------------

    Chapel $major.$minor is available for download at these URLs:

      http://crayport.cray.com/Pages/default.aspx
      https://github.com/chapel-lang/chapel/releases/tag/$src_version


Deliverables:
-------------

    The Chapel release packages contain the compiler executables,
    runtime libraries, example programs, and documentation for use on
    Cray XC(TM) systems.  Chapel can be built from source for the Cray
    CS(TM) system.  Additional information on Chapel can be found at:
    https://chapel-lang.org, and the open-source release can be found
    at: https://github.com/chapel-lang/chapel/releases/tag/$src_version


Schedule:
---------

    Official release date:     September n, 2018 (TODO)
    Chapel $major.$(( $minor + 1 )) release:             March, 2019 (TODO)


Licensing:
----------

    Chapel is licensed under the terms of the Apache 2.0 license.  See
    \$CHPL_HOME/LICENSE or https://chapel-lang.org/license.html for
    details.


Dependencies:
-------------

    Chapel $major.$minor is supported on the following Cray systems:
    * Cray Shasta systems with CLE based on SLES15 or later
    * Cray XC systems with CLE 5.2UP04 or later
    * Cray CS systems running Red Hat, CentOS, or SuSE Linux

    A back-end C compiler is required to generate an executable. Any
    recent version of PrgEnv-gnu/gcc or PrgEnv-intel/icc can be used.
    Using the module build of Chapel with PrgEnv-cray requires
    cce/8.6.3 or later (those wanting to use earlier versions of
    CLE or PrgEnv-cray can attempt to do so by building from source).


Installation instructions:
--------------------------

    Copy the Chapel RPM file to the current directory and execute
    the appropriate one of the following commands:

      #On Cray Shasta systems (x86_64):
      rpm -ivh chapel-$pkg_version-$rpm_release.x86_64.rpm

      #On Cray XC systems (x86_64):
      rpm -ivh chapel-$pkg_version-crayxc$rc_number.x86_64.rpm

      #On Cray XC systems (aarch64):
      rpm -ivh chapel-$pkg_version-crayxc$rc_number.aarch64.rpm

      #To make this the default version:
      /opt/cray/admin-pe/set_default_files/set_default_chapel_$pkg_version

    For Cray CS systems, download the source release tar file
      chapel-$src_version.tar.gz
    from Crayport or GitHub:
      https://github.com/chapel-lang/chapel/releases/tag/$src_version

    Unpack the tar file and follow the instructions in
    \$CHPL_HOME/doc/html/platforms/cray.html.


Getting started:
----------------

   Please see \$CHPL_HOME/doc/html/platforms/cray.html for quick start
   information and further details.

EOF
