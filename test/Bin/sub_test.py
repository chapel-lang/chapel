#!/usr/bin/env python
#
# Rewrite of sub_test by Sung-Eun Choi (sungeun@cray.com)
#  sub_test is used by start_test in the Chapel Testing system
#  August 2009
#
# This script can be overridden with a script by the same name
#  placed in the test directory.
#
# The use and behavior of the various environment variables,
#  settings, and files were copied straight from sub_test.  They
#  were added/modified to sub_test over the years, and their use
#  is inconsistent and a bit of a mess.  I like to think that this
#  is due to the fact that the original sub_test was written in csh,
#  which was probably pretty novel at the time but is quite limited
#  by today's standards.  In addition, I implemented the timeout
#  mechanism directly rather than calling out to the timedexec
#  (perl) script.
#
# For compatibility reasons, I have maintained the behavior of the
#  original sub_test.  Any new features (e.g., internal timeout
#  mechanism) or modified behaviors (e.g., multiple .compopts,
#  multiple .execopts, custom .good files) will not interfere with
#  the expected behavior of tests that do not use the features or
#  behaviors.
#
# ENVIRONMENT VARIABLES:
#
# CHPL_HOME: Grabbed from the environment or deduced based on the path to
#    the compiler.
# CHPL_TEST_VGRND_COMP: Use valgrind on the compiler
# CHPL_TEST_VBRND_EXE: Use valgrind on the test program
# CHPL_VALGRIND_OPTS: Options to valgrind
# CHPL_TET_INVOKE_DIR: Use this dir to invoke the tests (rather than ".")
# CHPL_TEST_FUTURES: 2 == test futures only
#                    1 == test futures and non-futures
#                    0 == test non-futures only
# CHPL_TEST_NOTESTS: Test the tests that are marked "notest" (see below)
# LAUNCHCMD: Uses this command to launch the test program
# CHPL_TEST_INTERP:
# CHPL_TEST_PERF: Run as a performance test (same as -performance flag)
# CHPL_TEST_PERF_DIR: Scratch directory for performance data
# CHPL_ONE_TEST: Name of the one test in this directory to run
# CHPL_TEST_SINGLES: If false, test the entire directory
# CHPL_COMM: Chapel communication layer
# CHPL_COMPONLY: Only build the test (same as -noexec flag)
#
#
# DIRECTORY-WIDE FILES:  These settings are for the entire directory and
#  in many cases can be overridden or augmented with test-specific settings.
#
# TIMEOUT: Timeout for compiler or test execution (default 300, 600 for valgrind)
# KILLTIMEOUT: Timeout before kill'ed processes are sent 'kill -9' (default 10)
# NOEXEC: Do not execute tests in this directory
# NOVGRBIN: Do not execute valgrind
# COMPSTDIN: Get stdin from this file (default /dev/null)
# COMPOPTS: Compiler flags
# LASTCOMPOPTS: Compiler flags to be put at the end of the command line
# EXECOPTS: Test program flags to be applied to the entire directory
# NUMLOCALES: Number of locales to use
# CATFILES: List of files whose contents are added to end of test output
# PREDIFF: Script to execute before diff'ing output (arguments: <test>,
#    executable>, <log>, <compiler executable>)
# PREEXEC: Script to execute before executing test program (arguments: <test
#    executable>, <log>, <compiler executable>)
#
#
# TEST-SPECIFIC FILES:  These setting override or augment the directory-wide
#  settings.  Unless otherwise specified, these files are named
#  <test executable>.suffix (where suffix is one of the following).
#
# .good: "Golden" output file (can have different basenames)
# .compopts: Additional compiler options
# .perfcompopts: Additional compiler options for performance testing
# .lastcompopts: Additional compiler options to be added at the end of the
#    command line
# .execopts: Additional test options
# .perfexecopts: Additional test options for performance testing
# .notest: Do not run this test
# .numlocales: Number of locales to use (overrides NUMLOCALES)
# .future: Future test
# .ifuture: Future test
# .noexec: Do not execute this test
# .skipif: Skip this test if certain environment conditions hold true
# .timeout: Test timeout (overrides TIMEOUT)
# .killtimeout: Kill timeout (overrides KILLTIMEOUT)
# .catfiles: Additional list of files whose contents are added to end of
#    test output
# .prediff: Additional script to execute before diff'ing output
# .preexec: Additional script to execute before executing test program
# .perfkeys: Existence indicates a performance test.  Contents specifies
#    performance "keys"


import sys, os, subprocess, string, signal
import select, fcntl
import fnmatch, time
import re
import shlex

#
# Time out class:  Read from a stream until time out
#  A little ugly but sending SIGALRM (or any other signal) to Python
#   can be unreliable (will not respond if holding certain locks).
#
class ReadTimeoutException(Exception): pass

def SetNonBlock(stream):
    flags = fcntl.fcntl(stream.fileno(), fcntl.F_GETFL)
    flags |= os.O_NONBLOCK
    fcntl.fcntl(stream.fileno(), fcntl.F_SETFL, flags)

def SuckOutputWithTimeout(stream, timeout):
    SetNonBlock(stream)
    buffer = ''
    end_time = time.time() + timeout
    while True:
        now = time.time()
        if end_time <= now:
            # Maybe return partial result instead?
            raise ReadTimeoutException('Teh tiem iz out!');
        ready_set = select.select([stream], [], [], end_time - now)[0]
        if stream in ready_set:
            bytes = stream.read()
            if len(bytes) == 0:
                break           # EOF
            buffer += bytes     # Inefficient way to accumulate bytes.
            # len(ready_set) == 0 is also an indication of timeout. However,
            # if we relied on that, we would require no data ready in order
            # to timeout  which doesn't seem quite right either.
    return buffer


#
# Auxilliary functions
#

# Escape all special characters
def ShellEscape(str):
    return re.sub(r'([\\!@#$%^&*()?\'"|<>[\]{} ])', r'\\\1', str)

# return True if f has .chpl extension
def is_chpl_source(f):
    if ((f.count('.') !=0) and (len(f) > 6) and (len(f)-f.find('.chpl')==5)):
        return True
    else:
        return False

# read file with comments
def read_file_with_comments(f):
    # sys.stdout.write('Opening: %s\n'%(f))
    myfile = open(f, 'r')
    mylines = myfile.readlines()
    myfile.close()
    mylist=list()
    for line in mylines:
        line=line.strip()
        # ignore blank lines
        if not line.strip(): continue
        # ignore comments
        if line.lstrip()[0] == '#': continue
        mylist.append(line)
    return mylist

# diff 2 files
def diff_files(f1, f2):
    sys.stdout.write('[Executing diff %s %s]\n'%(f1, f2))
    p = subprocess.Popen(['diff',f1,f2],
                         stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    myoutput = p.communicate()[0] # grab stdout to avoid potential deadlock
    if p.returncode != 0:
        sys.stdout.write(myoutput)
    return p.returncode

# kill process
def kill_proc(p, timeout):
    k = subprocess.Popen(['kill',str(p.pid)])
    k.wait()
    now = time.time()
    end_time = now + timeout # give it a little time
    while end_time > now:
        if p.poll():
            return
        now = time.time()
    # use the big hammer (and don't bother waiting)
    subprocess.Popen(['kill','-9', str(p.pid)])
    return


#
# Start of sub_test proper
#

if len(sys.argv)!=2:
    print 'usage: sub_test.py compiler'
    sys.exit(0)

# Find the base installation
compiler=sys.argv[1]
if not os.access(compiler,os.R_OK|os.X_OK):
    sys.stdout.write('[Cannot execute compiler \'%s\']\n'%(compiler))
    sys.exit(-1)

path_to_compiler=os.path.abspath(os.path.dirname(compiler))
# Assume chpl binary is 2 directory levels down in the base installation
(chpl_base, tmp) = os.path.split(path_to_compiler)
(chpl_base, tmp) = os.path.split(chpl_base)
chpl_base=os.path.normpath(chpl_base)
# sys.stdout.write('CHPL_BASE='+chpl_base+'\n')

# If $CHPL_HOME is not set, use the base installation of the compiler
chpl_home=os.getenv('CHPL_HOME', chpl_base);
chpl_home=os.path.normpath(chpl_home)
# sys.stdout.write('CHPL_HOME='+chpl_home+'\n');

# Find the test directory
testdir=chpl_home+'/test'
if os.path.isdir(testdir)==0:
    testdir=chpl_home+'/examples'
    if os.path.isdir(testdir)==0:
        sys.stdout.write('Error: Cannot find '+chpl_home+'/test or '+chpl_home+'/examples'+'\n')
        sys.exit(0)
# sys.stdout.write('testdir='+testdir+'\n');

# Use timedexec
# As much as I hate calling out to another script for the time out stuff,
#  subprocess doesn't quite cut is for this kind of stuff
useTimedExec=True
if useTimedExec:
    timedexec=testdir+'/Bin/timedexec'
    if not os.access(timedexec,os.R_OK|os.X_OK):
        sys.stdout.write('[Cannot execute timedexec script \"%s\"]\n'%(timedexec))
        sys.exit(-1)
# sys.stdout.write('timedexec='+timedexec+'\n');

# HW platform
platform=subprocess.Popen([chpl_base+'/util/platform.pl', '--target'], stdout=subprocess.PIPE).communicate()[0]
platform.strip()
# sys.stdout.write('platform='+platform+'\n')

# Machine name we are running on
machine=os.uname()[1].split('.', 1)[0]
# sys.stdout.write('machine='+machine+'\n')

# Get global timeout
if os.access('./TIMEOUT',os.R_OK):
    gt=subprocess.Popen(['cat', './TIMEOUT'], stdout=subprocess.PIPE).communicate()[0]
    globalTimeout=string.atoi(gt)
elif os.getenv('CHPL_TEST_VGRND_COMP')=='on':
    globalTimeout=900
else:
    globalTimeout=300
# sys.stdout.write('globalTimeout=%d\n'%(globalTimeout))

# Get global timeout for kill
if os.access('./KILLTIMEOUT',os.R_OK):
    gt=subprocess.Popen(['cat', './KILLTIMEOUT'], stdout=subprocess.PIPE).communicate()[0]
    globalKillTimeout=string.atoi(gt)
else:
    globalKillTimeout=10
# sys.stdout.write('globalKillTimeout=%d\n'%(globalKillTimeout))

# Get the current directory
localdir = string.replace(os.getcwd(), testdir, '.')

if localdir!='./':
    # strip off the leading './'
    localdir = string.lstrip(localdir, '.')
    localdir = string.lstrip(localdir, '/')
# sys.stdout.write('localdir=%s\n'%(localdir))

#
# Test options for all tests in this directory
#
if os.access('./NOEXEC',os.R_OK):
    execute=False
else:
    execute=True
# sys.stdout.write('execute=%d\n'%(execute))

if os.access('./NOVGRBIN',os.R_OK):
    vgrbin=False
else:
    vgrbin=True
# sys.stdout.write('vgrbin=%d\n'%(vgrbin))

if os.access('./COMPSTDIN',os.R_OK):
    compstdin='./COMPSTDIN'
else:
    compstdin='/dev/null'
# sys.stdout.write('compstdin=%s\n'%(compstdin))

globalLastcompopts=list();
if os.access('./LASTCOMPOPTS',os.R_OK):
    globalLastcompopts+=subprocess.Popen(['cat', './LASTCOMPOPTS'], stdout=subprocess.PIPE).communicate()[0].strip().split()
# sys.stdout.write('globalLastcompopts=%s\n'%(globalLastcompopts))

if os.access('./NUMLOCALES',os.R_OK):
    globalNumlocales=int(subprocess.Popen(['cat', './NUMLOCALES'], stdout=subprocess.PIPE).communicate()[0])
    # globalNumlocales.strip(globalNumlocales)
else:
    globalNumlocales=int(os.getenv('NUMLOCALES'))
# sys.stdout.write('globalNumlocales=%s\n'%(globalNumlocales))

if os.access('./CATFILES',os.R_OK):
    globalCatfiles=subprocess.Popen(['cat', './CATFILES'], stdout=subprocess.PIPE).communicate()[0]
    globalCatfiles.strip(globalCatfiles)
else:
    globalCatfiles=None
# sys.stdout.write('globalCatfiles=%s\n'%(globalCatfiles))


#
# valgrind stuff
#
chpl_valgrind_opts=os.getenv('CHPL_VALGRIND_OPTS', '--tool=memcheck')
# sys.stdout.write('chpl_valgrind_opts=%s\n'%(chpl_valgrind_opts))

if os.getenv('CHPL_TEST_VGRND_COMP')=='on':
    valgrindcomp = 'valgrind'
    valgrindcompopts=chpl_valgrind_opts.split()
    valgrindcompopts+=['--gen-suppressions=all']
    valgrindcompopts+=['--suppressions=%s/compiler/etc/valgrind.suppressions'%(chpl_home)]
    valgrindcompopts+=['-q']
else:
    valgrindcomp = None
    valgrindcompopts = None
# sys.stdout.write('valgrindcomp=%s %s\n'%(valgrindcomp, valgrindcompopts))

if (os.getenv('CHPL_TEST_VGRND_EXE')=='on' and vgrbin):
    valgrindbin = 'valgrind'
    valgrindbinopts = chpl_valgrind_opts.split()+['-q']
else:
    valgrindbin = None
    valgrindbinopts = None
# sys.stdout.write('valgrindbin=%s %s\n'%(valgrindbin, valgrindbinopts))


#
# Misc set up
#

testfutures=string.atoi(os.getenv('CHPL_TEST_FUTURES','0'))
# sys.stdout.write('testfutures=%s\n'%(testfutures))

testnotests=os.getenv('CHPL_TEST_NOTESTS')
# sys.stdout.write('testnotests=%s\n'%(testnotests))

launchcmd=os.getenv('LAUNCHCMD')
# sys.stdout.write('launchcmd=%s\n'%(launchcmd))

if os.getenv('CHPL_TEST_INTERP')=='on':
    execute=False
    futureSuffix='.ifuture'
else:
    futureSuffix='.future'
# sys.stdout.write('futureSuffix=%s\n'%(futureSuffix))

if os.getenv('CHPL_TEST_PERF')!=None:
    compoptssuffix='.perfcompopts'
    execoptssuffix='.perfexecopts'
    perftest=True
    perfdir=os.getenv('CHPL_TEST_PERF_DIR', './perfdat/'+machine)
else:
    compoptssuffix='.compopts'
    execoptssuffix='.execopts'
    perftest=False
# sys.stdout.write('perftest=%d\n'%(perftest))


#
# Global COMPOPTS
#
if os.access('./COMPOPTS',os.R_OK):
    tgco=read_file_with_comments('./COMPOPTS')
    globalCompopts = shlex.split(tgco[0])
else:
    globalCompopts=list()
envCompopts = os.getenv('COMPOPTS')
if envCompopts != None:
    globalCompopts += shlex.split(envCompopts)
# sys.stdout.write('globalCompopts=%s\n'%(globalCompopts))

#
# Global EXECOPTS
#
globalExecopts=list()
if os.access('./EXECOPTS',os.R_OK):
    tgeo=read_file_with_comments('./EXECOPTS')
    globalExecopts= shlex.split(tgeo[0])
else:
    globalExecopts=list()
envExecopts = os.getenv('EXECOPTS')
if envExecopts != None:
    globalExecopts += shlex.split(envExecopts)
# sys.stdout.write('globalExecopts=%s\n'%(globalExecopts))

#
# Global PREDIFF & PREEXEC
#
if os.access('./PREDIFF',os.R_OK|os.X_OK):
    globalPrediff='./PREDIFF'
else:
    globalPrediff=None
# sys.stdout.write('globalPrediff=%s\n'%(globalPrediff))
if os.access('./PREEXEC',os.R_OK|os.X_OK):
    globalPreexec='./PREEXEC'
else:
    globalPreexec=None

#
# Start running tests
#
sys.stdout.write('[Starting subtest - %s]\n'%(time.strftime('%a %b %d %H:%M:%S %Z %Y', time.localtime())))
sys.stdout.write('[compiler: \'%s\']\n'%(compiler))

dirlist=os.listdir(testdir+'/'+localdir)

onetestsrc = os.getenv('CHPL_ONETEST')
if onetestsrc==None:
    testsrc=filter(is_chpl_source, dirlist)
else:
    testsrc=list()
    testsrc.append(onetestsrc)
# sys.stdout.write('testsrc=%s\n'%(testsrc))

for testname in testsrc:
    sys.stdout.flush()

    # print testname
    execname=testname[:len(testname)-5]
    # print execname

    # Test specific settings
    catfiles = globalCatfiles
    numlocales = globalNumlocales
    lastcompopts = list()
    if globalLastcompopts:
        lastcompopts += globalLastcompopts
    # sys.stdout.write("lastcompopts=%s\n"%(lastcompopts))

    # Get the list of files starting with 'execname.'
    execname_files = fnmatch.filter(dirlist, execname+'.*')
    # print execname_files, dirlist

    if (perftest and (execname_files.count(execname+'.perfkeys')==0)):
        sys.stdout.write('[Skipping noperf test: %s/%s]\n'%(localdir,execname))
        continue # on to next test

    timeout = globalTimeout
    killtimeout = globalKillTimeout
    futuretest=''
    executebin=execute
    testfuturesfile=False
    noexecfile=False
    execoptsfile=False
    prediff=None
    preexec=None
    # Deal with these files
    do_not_test=False
    for f in execname_files:
        (root, suffix) = os.path.splitext(f)
        # sys.stdout.write("**** %s ****\n"%(f))

        # Deal with these later
        if (suffix == '.good' or
            suffix=='compopts' or suffix=='perfcompopts' or
            suffix=='execopts' or suffix=='perfexecopts'):
            continue # on to next file

        elif (suffix=='.notest' and (os.access(f, os.R_OK) and
                                     testnotests=='0')):
            sys.stdout.write('[Skipping notest test: %s/%s]\n'%(localdir,execname))
            do_not_test=True
            break

        elif (suffix=='.skipif' and (os.access(f, os.R_OK) and
               (os.getenv('CHPL_TEST_SINGLES')=='0'))):
            skiptest=subprocess.Popen([testdir+'/Bin/testEnv.pl', './'+f], stdout=subprocess.PIPE).communicate()[0]
            if int(skiptest):
                sys.stdout.write('[Skipping %s based on .skipif environment settings]\n'%(f))
                do_not_test=True
                break

        elif (suffix=='.timeout' and os.access(f, os.R_OK)):
            timeout=string.atoi(subprocess.Popen(['cat', f], stdout=subprocess.PIPE).communicate()[0])

        elif (suffix=='.killtimeout' and os.access(f, os.R_OK)):
            killtimeout=string.atoi(subprocess.Popen(['cat', f], stdout=subprocess.PIPE).communicate()[0])

        elif (suffix=='.catfiles' and os.access(f, os.R_OK)):
            execcatfiles=subprocess.Popen(['cat', f], stdout=subprocess.PIPE).communicate()[0].strip()
            if catfiles:
                catfiles+=execcatfiles
            else:
                catfiles=execcatfiles

        elif (suffix=='.lastcompopts' and os.access(f, os.R_OK)):
            lastcompopts+=subprocess.Popen(['cat', f], stdout=subprocess.PIPE).communicate()[0].strip().split()
            # sys.stdout.write("lastcompopts=%s\n"%(lastcompopts))

        elif (suffix=='.numlocales' and os.access(f, os.R_OK)):
            numlocales=int(subprocess.Popen(['cat', f], stdout=subprocess.PIPE).communicate()[0])

        elif suffix==futureSuffix and os.access(f, os.R_OK):
            futuretest='Future ('+subprocess.Popen(['head', '-n', '1', './'+execname+futureSuffix], stdout=subprocess.PIPE).communicate()[0].strip()+') '

        elif (suffix=='.noexec' and os.access(f, os.R_OK)):
            noexecfile=True
            executebin=False

        elif (suffix=='.prediff' and os.access(f, os.R_OK|os.X_OK)):
            prediff=f

        elif (suffix=='.preexec' and os.access(f, os.R_OK|os.X_OK)):
            preexec=f

        if suffix=='.future':
            testfuturesfile=True
            if testfutures==0:
                sys.stdout.write('[Skipping future test: %s/%s]\n'%(localdir,execname))
                do_not_test=True

        if do_not_test:
            break

    del execname_files

    # Skip to the next test
    if do_not_test:
        continue # on to next test

    # Skip non-future tests if specified
    if (testfuturesfile==False and testfutures==2):
        sys.stdout.write('[Skipping non-future test: %s/%s]\n'%(localdir,execname))
        continue # on to next test

    # Set numlocales
    if (numlocales == 0) or (os.getenv('CHPL_COMM', 'none')=='none'):
        numlocexecopts = None
    else:
        numlocexecopts = ' -nl '+str(numlocales)

    # Get list of test specific compiler options
    if os.access(execname+compoptssuffix, os.R_OK):
        compoptslist = read_file_with_comments(execname+compoptssuffix)
    else:
        compoptslist = list(' ')

    # Get list of test specific exec options
    if os.access(execname+execoptssuffix, os.R_OK):
        execoptsfile=True
        execoptslist = read_file_with_comments(execname+execoptssuffix)
    else:
        execoptslist = list(' ')

    if (os.getenv('CHPL_TEST_INTERP')=='on' and
        (noexecfile or testfuturesfile or execoptsfile)):
        sys.stdout.write('[Skipping interpretation of : %s/%s]\n'%(localdir/execname))
        continue # on to next test


    # For all compopts + execopts combos..
    compoptsnum = 0
    for compopts in compoptslist:
        sys.stdout.flush()

        if len(compoptslist) == 1:
            complog=execname+'.comp.out.tmp'
        else:
            compoptsnum += 1
            complog = execname+'.'+str(compoptsnum)+'.comp.out.tmp'

        #
        # Build the test program
        #
        args=['-o']+[execname]+globalCompopts+shlex.split(compopts)+[testname]
        if lastcompopts:
            args += lastcompopts
        # sys.stdout.write("args=%s\n"%(args))

        if valgrindcomp:
            cmd = valgrindcomp
            args = valgrindcompopts+[compiler]+args
        else:
            cmd = compiler

        #
        # Compile (with timeout)
        #
        sys.stdout.write('[Executing %s'%(cmd))
        if args:
            sys.stdout.write(' %s'%(' '.join(args)))
        sys.stdout.write('< %s\']\n'%(compstdin))
        if useTimedExec:
            wholecmd = cmd+' '+' '.join(map(ShellEscape, args))+' < '+compstdin
            p = subprocess.Popen([timedexec, str(timeout), wholecmd],
                                 stdout=subprocess.PIPE,
                                 stderr=subprocess.STDOUT)
            output = p.communicate()[0]
            status = p.returncode

            if status == 222:
                sys.stdout.write('%s[Error: Timed out compilation for %s/%s'%
                                 (futuretest, localdir, execname))
                if len(compoptslist) > 1:
                    sys.stdout.write('(%s %s)'%(' '.join(globalCompopts),compopts))
                sys.stdout.write(']\n')
                continue # on to next compopts
            
        else:
            my_stdin=file(compstdin, 'r')
            p = subprocess.Popen([cmd]+args, stdin=my_stdin,
                                 stdout=subprocess.PIPE,
                                 stderr=subprocess.STDOUT)
            try:
                output = SuckOutputWithTimeout(p.stdout, timeout)
            except ReadTimeoutException:
                sys.stdout.write('%s[Error: Timed out compilation for %s/%s'%
                                 (futuretest, localdir, execname))
                if len(compoptslist) > 1:
                    sys.stdout.write('(%s %s)'%(' '.join(globalCompopts),compopts))
                sys.stdout.write(']\n')
                kill_proc(p, killtimeout)
                continue # on to next compopts

            status = p.returncode

        if (status!=0 or not executebin):
            # Compare compiler output with expected program output
            if catfiles:
                sys.stdout.write('[Concatenating extra files: %s]\n'%
                                 (execname+'.catfiles'))
                output+=subprocess.Popen(['cat', catfiles],
                                         stdout=subprocess.PIPE).communicate()[0]

            # Sadly these scripts require an actual file
            complogfile=file(complog, 'w')
            complogfile.write('%s'%(output))
            complogfile.close()

            if globalPrediff:
                sys.stdout.write('Executing ./PREDIFF\n')
                subprocess.Popen(['./PREDIFF',
                                  execname,complog,compiler]).wait()

            if prediff:
                subprocess.Popen(['./'+execname+'.prediff',
                                  execname,complog,compiler]).wait()

            sys.stdout.write('output=%s\n'%(output))

            # Find the default 'golden' output file
            checkfile = execname+'.'+machine+'.good'
            if not os.path.isfile(checkfile):
                checkfile=execname+'.comm-'+os.getenv('CHPL_COMM','none')+'.good'
                if not os.path.isfile(checkfile):
                    checkfile=execname+platform+'.good'
                    if not os.path.isfile(checkfile):
                        checkfile=execname+'.good'
            # sys.stdout.write('default checkfile=%s\n'%(checkfile))
                
            if not os.access(checkfile, os.R_OK):
                sys.stdout.write('[Error cannot locate compiler output comparison file %s]\n'%(checkfile))
                sys.stdout.write('[Compiler output was as follows:]\n')
                subprocess.Popen(['cat', complog]).wait()
                continue # on to next compopts

            result = diff_files(checkfile, complog)
            if result==0:
                os.unlink(complog)
                sys.stdout.write('%s[Success '%(futuretest))
            else:
                sys.stdout.write('%s[Error '%(futuretest))
            sys.stdout.write('matching compiler output for %s/%s'%
                                 (localdir, execname))
            if len(compoptslist) > 1:
                sys.stdout.write('(%s %s)'%(' '.join(globalCompopts),compopts))
            sys.stdout.write(']\n')

            continue # on to next compopts
        else:
            compoutput = output # save for diff


        #
        # Compile successful
        #
        sys.stdout.write('[Success compiling %s/%s]\n'%(localdir, execname))

        if os.getenv('CHPL_COMPONLY'):
            sys.stdout.write('[Note: Not executing or comparing the output due to -noexec flags]\n')
            continue # on to next compopts

        if os.access(execname+'.stdin', os.R_OK):
            redirectin = execname+'.stdin'
        else:
            redirectin = '/dev/null'

        # Execute the test for all requested execopts
        execoptsnum = 0
        for texecopts in execoptslist:
            sys.stdout.flush()
            onlyone = (len(compoptslist)==1) and (len(execoptslist)==1)
            tlist = texecopts.split('#')
            execopts = tlist[0].strip()
            if numlocexecopts != None:
                execopts += numlocexecopts;
            if len(tlist) > 1:
                # Ignore everything after the first token
                execcheckfile = tlist[1].strip().split()[0]
            else:
                execcheckfile = None
            del tlist

            if onlyone:
                execlog=execname+'.exec.out.tmp'
            else:
                execoptsnum += 1
                execlog = execname+'.'+str(compoptsnum)+'-'+str(execoptsnum)+'.exec.out.tmp'

            if globalPreexec:
                sys.stdout.write('[Executing PREEXEC]\n')
                subprocess.Popen(['./PREEXEC',
                                  execname,execlog,compiler]).wait()

            if preexec:
                sys.stdout.write('[Executing %s.preexec]\n'%(execname))
                subprocess.Popen([execname+'.preexec',
                                  execname,execlog,compiler]).wait()

            if not os.access(execname, os.R_OK|os.X_OK):
                sys.stdout.write('%s[Error could not locate executable %s for %s/%s'%
                                 (futuretest, execname, localdir, execname))
                if not onlyone:
                    sys.stdout.write(' (%d-%d)'%(compoptsnum, execoptsnum))
                sys.stdout.write(']\n')
                break; # on to next compopts

            args=list();
            if launchcmd:
                cmd=launchcmd
                args+=['./'+execname]
            elif valgrindbin:
                cmd=valgrindbin
                args+=valgrindbinopts+['./'+execname]
            else:
                cmd='./'+execname

            args+=globalExecopts
            args+=shlex.split(execopts)

            #
            # Run program (with timeout)
            #
            sys.stdout.write('[Executing program %s %s< %s\']\n'%
                             (cmd, ' '.join(args), redirectin))
            if useTimedExec:
                wholecmd = cmd+' '+' '.join(map(ShellEscape, args))+' < '+redirectin
                p = subprocess.Popen([timedexec, str(timeout), wholecmd],
                                     stdout=subprocess.PIPE,
                                     stderr=subprocess.STDOUT)
                output = p.communicate()[0]
                status = p.returncode

                if status == 222:
                    sys.stdout.write('%s[Error: Timed out executing program %s/%s'%
                                     (futuretest, localdir, execname))
                    if not onlyone:
                        sys.stdout.write(' %s %s (%s %s) (%d-%d)'%
                                         (' '.join(globalExecopts), execopts,
                                          ' '.join(globalCompopts),compopts,
                                          compoptsnum, execoptsnum))
                    sys.stdout.write(']\n')
                    continue # on to next execopts

            else:
                my_stdin=file(redirectin, 'r')
                p = subprocess.Popen([cmd]+args, stdin=my_stdin,
                                     stdout=subprocess.PIPE,
                                     stderr=subprocess.STDOUT)
                try:
                    output = SuckOutputWithTimeout(p.stdout, timeout)
                except ReadTimeoutException:
                    sys.stdout.write('%s[Error: Timed out executing program %s/%s'%
                                     (futuretest, localdir, execname))
                    if not onlyone:
                        sys.stdout.write(' %s %s (%s %s) (%d-%d)'%
                                         (' '.join(globalExecopts), execopts,
                                          ' '.join(globalCompopts),compopts,
                                          compoptsnum, execoptsnum))
                    sys.stdout.write(']\n')
                    kill_proc(p, killtimeout)
                    continue # on to next execopts

                status = p.returncode

            if catfiles:
                sys.stdout.write('[Concatenating extra files: %s]\n'%
                                 (execname+'.catfiles'))
                output+=subprocess.Popen(['cat']+catfiles.split(),
                                         stdout=subprocess.PIPE).communicate()[0]

            # Sadly these scripts require an actual file
            execlogfile=file(execlog, 'w')
            execlogfile.write('%s'%(compoutput))
            execlogfile.write('%s'%(output))
            execlogfile.close()

            if globalPrediff:
                sys.stdout.write('[Executing PREDIFF]\n')
                sys.stdout.write(subprocess.Popen(['./PREDIFF',
                                                   execname,execlog,compiler],
                                                  stdout=subprocess.PIPE).
                                 communicate()[0])

            if prediff:
                sys.stdout.write('[Executing %s.prediff]\n'%(execname))
                sys.stdout.write(subprocess.Popen(['./'+execname+'.prediff',
                                                   execname,execlog,compiler],
                                                  stdout=subprocess.PIPE).
                                 communicate()[0])

            if not perftest:
                if (execcheckfile == None):
                    # Look for the "golden" output
                    if onlyone:
                        ceident=''
                    else:
                        if len(compoptslist)==1:
                            ceident = '.0-'
                        else:
                            ceident = '.'+str(compoptsnum+1)+'-'
                        if len(execoptslist)==1:
                            ceindent += '0'
                        else:
                            ceident += str(execoptsnum)

                    execcheckfile = execname+'.'+machine+ceident+'.good'
                    if not os.path.isfile(execcheckfile):
                        execcheckfile=execname+'.comm-'+os.getenv('CHPL_COMM','none')+ceident+'.good'
                        if not os.path.isfile(execcheckfile):
                            execcheckfile=execname+platform+ceident+'.good'
                            if not os.path.isfile(execcheckfile):
                                execcheckfile=execname+ceident+'.good'
                                if not os.path.isfile(execcheckfile):
                                    execcheckfile=execname+'.good'

                if not os.access(execcheckfile, os.R_OK):
                    sys.stdout.write('[Error cannot locate program output comparison file %s]\n'%(execcheckfile))
                    sys.stdout.write('[Execution output was as follows:]\n')
                    sys.stdout.write(subprocess.Popen(['cat', execlog],
                                                      stdout=subprocess.PIPE).
                                     communicate()[0])

                    continue # on to next execopts

                result = diff_files(execcheckfile, execlog)
                if result==0:
                    os.unlink(execlog)
                    sys.stdout.write('%s[Success '%(futuretest))
                else:
                    sys.stdout.write('%s[Error '%(futuretest))
                sys.stdout.write('matching program output for %s/%s'%
                                 (localdir, execname))
                if result!=0 and not onlyone:
                    sys.stdout.write(' %s %s (%s %s) (%d-%d)'%
                                     (' '.join(globalExecopts), execopts,
                                      ' '.join(globalCompopts),compopts,
                                      compoptsnum, execoptsnum))
                sys.stdout.write(']\n')

            else:
                if not os.path.isdir(perfdir) and not os.path.isfile(perfdir):
                    os.makedirs(perfdir)
                if not os.access(perfdir, os.R_OK|os.X_OK):
                    sys.stdout.write('[Error creating performance test directory %s]\n'%(perfdir))
                    break # on to next compopts
                sys.stdout.write('[Executing checkKeys %s %s]\n'%(execname, perfdir))
                p = subprocess.Popen([testdir+'/Bin/computePerfStats.pl',
                                      execname, perfdir],
                                     stdout=subprocess.PIPE)
                sys.stdout.write('%s'%(p.communicate()[0]))

                status = p.returncode

                if status == 0:
                    sys.stdout.write('[Success')
                else:
                    sys.stdout.write('[Error')
                sys.stdout.write(' matching performance keys for %s/%s'%
                                 (localdir, execname))
                if status!=0 and not onlyone:
                    sys.stdout.write(' %s %s (%s %s) (%d-%d)'%
                                     (' '.join(globalExecopts), execopts,
                                      ' '.join(globalCompopts),compopts,
                                      compoptsnum, execoptsnum))
                sys.stdout.write(']\n')

        if os.path.isfile(execname):
            os.unlink(execname)
        if os.path.isfile(execname+'_real'):
            os.unlink(execname+'_real')

    del execoptslist
    del compoptslist
