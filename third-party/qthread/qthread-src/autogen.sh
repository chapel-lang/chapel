#!/bin/bash
#
# Authors: Brian W. Barrett <bwbarre@sandia.gov>,
#          Kyle B. Wheeler <kbwheel@sandia.gov>
#

echo "Generating configure files..."

if [ "$ACLOCAL" ] ; then
	# do nothing
	echo I saw ACLOCAL defined to be $ACLOCAL
elif type aclocal &>/dev/null ; then
	export ACLOCAL=`type -P aclocal`
else
	echo "ERROR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
	echo "I need aclocal in order to generate the configure script and makefiles. I couldn't find it in your PATH, though. Perhaps you need to set the ACLOCAL environment variable to point toward a custom installation?"
	exit -1
fi

if [ "$LIBTOOLIZE" ] ; then
	# accept anything that's been pre-configured
	echo LIBTOOLIZE=$LIBTOOLIZE
elif type glibtoolize &>/dev/null ; then
	# prefer glibtoolize over libtoolize
	export LIBTOOLIZE=`type -p glibtoolize`
	echo LIBTOOLIZE=$LIBTOOLIZE
elif type libtoolize &>/dev/null ; then
	export LIBTOOLIZE=`type -p libtoolize`
	echo LIBTOOLIZE=$LIBTOOLIZE
else
	echo "ERROR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
	echo "I need libtoolize in order to generate the configure script and makefiles. I couldn't find either libtoolize or glibtoolize in your PATH. Perhaps you need to set the LIBTOOLIZE environment variable to point toward a custom installation?"
	exit -1
fi

libtool_version=( $($LIBTOOLIZE --version) )
libtool_version=( ${libtool_version[3]//./ } )
libtool_version=( ${libtool_version[*]/[a-z]/} )
if [ ${libtool_version[0]} -lt 2 ] ; then
	if [ ${libtool_version[0]} -eq 1 ] ; then
		if [ ${libtool_version[1]} -lt 5 ] ; then
			echo "Your version of libtoolize ($LIBTOOLIZE) is too old!"
			echo "We have tested with libtool 1.5.22."
			echo -n "You have: "
			$LIBTOOLIZE --version | head -n 1
			exit -1
		elif [ ${libtool_version[1]} -lt 9 ] ; then
			echo "You have an awfully old version of libtool. (${libtool_version[0]}.${libtool_version[1]}.${libtool_version[2]})"
			echo "We prefer to use LT_INIT, which came out with libtool 1.9b,"
			echo "way back in 2004. Maybe you should consider upgrading."
		fi
	else
		echo "Your version of libtoolize ($LIBTOOLIZE) is too old!"
		echo "We have tested with libtool 1.5.22."
		echo -n "You have: "
		$LIBTOOLIZE --version | head -n 1
		exit -1
	fi
fi

if [ "${LIBTOOLIZE}" = "${LIBTOOLIZE#/}" ] ; then
    libtool_fullpath=$(type -p "${LIBTOOLIZE}" )
else
    libtool_fullpath="$LIBTOOLIZE"
fi
if [ "${ACLOCAL}" = "${ACLOCAL#/}" ] ; then
    aclocal_fullpath=$(type -p "${ACLOCAL}" )
else
    aclocal_fullpath="$ACLOCAL"
fi
if [ "${libtool_fullpath%bin/*}" != "${aclocal_fullpath%bin/*}" ] ; then
    echo "Libtoolize and aclocal are not in the same directory, so I'm guessing ACLOCAL needs to be modified to include libtoolize's directory in its search path."
    if [ -d "${libtool_fullpath%/bin/*}/share/aclocal" ] ; then
        ACLOCAL_INCLUDE="${libtool_fullpath%/bin/*}/share/aclocal"
        echo 'That aclocal directory looks like "'$ACLOCAL_INCLUDE'"'
    else
        echo "I would do it for you, but I can't find a likely candidate".
    fi
else
    echo "Since LIBTOOLIZE and ACLOCAL are in the same directory, I'm going to assume they know about each other."
fi

if [ "$AUTOMAKE" ] ; then
	# accept anything that's been pre-configured
	echo Using AUTOMAKE=$AUTOMAKE
elif type automake &>/dev/null ; then
	export AUTOMAKE=`type -p automake`
else
	echo "ERROR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
	echo "I need automake in order to generate the configure script and makefiles. I couldn't find it in your PATH, though. Perhaps you need to set the AUTOMAKE environment variable to point toward a custom installation?"
	exit -1
fi

if [ "$AUTOCONF" ] ; then
	# accept anything that's been pre-configured
	echo Using AUTOCONF=$AUTOCONF
elif type autoconf &>/dev/null ; then
	export AUTOCONF=`type -p autoconf`
else
	echo "ERROR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
	echo "I need autoconf in order to generate the configure script and makefiles. I couldn't find it in your PATH, though. Perhaps you need to set the AUTOCONF environment variable to point toward a custom installation?"
	exit -1
fi

if [ "$AUTORECONF" ] ; then
	# accept anything that's been pre-configured
	echo Using AUTORECONF=$AUTORECONF
elif type autoreconf &>/dev/null ; then
	export AUTORECONF=`type -p autoreconf`
else
	echo "ERROR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
	echo "I need autoreconf in order to generate the configure script and makefiles. I couldn't find it in your PATH, though. Perhaps you need to set the AUTORECONF environment variable to point toward a custom installation?"
	exit -1
fi

if [ "$ACLOCAL_INCLUDE" ] ; then
	echo 'ACLOCAL="'$ACLOCAL -I$ACLOCAL_INCLUDE'"'
	export ACLOCAL="$ACLOCAL -I$ACLOCAL_INCLUDE"
fi

# If this directory isn't removed, the configure script may not have the right
# dynamically-generated version number
if [ -d autom4te.cache ] ; then
	rm -rf autom4te.cache
fi

version=$(awk '{if(NR==1)print$2;else exit}' ./NEWS)
if [[ ${version%b} != ${version} ]] && type git &>/dev/null && [[ $SKIPVGEN != 1 ]] ; then
	echo "Counting entries in git's log to come up with a revision number..."
	#git log -1 --pretty=format:%h | tee .autogen_git_output
    git log --pretty=oneline | wc -l | tee .autogen_git_output
	#svn stat -u README | tee .autogen_svn_output
	rev=$version-$(awk '{print $1}' .autogen_git_output)
	if [ "$rev" ] ; then
		echo -n $rev > .autogen-version
	else
		echo $version | tr -d '\012' > .autogen-version
	fi
else
	echo $version | tr -d '\012' > .autogen-version
fi

$AUTORECONF --install --symlink --warnings=gnu,obsolete,override,portability,no-obsolete "$@" && \
  echo "Preparing was successful if there were no error messages above." && \
  exit 0

echo "It appears that configure file generation failed.  Sorry :(."
exit -1
