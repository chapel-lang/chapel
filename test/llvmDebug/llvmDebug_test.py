#!/usr/bin/env python

# Python test script to verify the llvm debug info
# is generated properly during Chapel compilation

import os, subprocess, sys

chpl_home = sys.argv[1]
platform = sys.argv[2]
compiler = sys.argv[3]

plat_comp = platform + '-' + compiler
llvm_tool_path = chpl_home + '/third-party/llvm/install/' + plat_comp + '/bin'
build_options = '--baseline --llvm -g'
source_path = os.getcwd() #same as target path
source = source_path + os.sep + 'llvmDebug_test.chpl'
target = source_path + os.sep + 'llvmDebug_test'

# Build Chapel Test Program
Command_build = chpl_home + '/bin/' + platform + '/chpl ' + build_options + ' ' + source + ' -o ' + target
if os.system(Command_build) == 0:
    print 'Build Succeeded'
else:
    print 'Build Failed'
    os._exit(1) #exit without raising an exception

# Determine syntax from LLVM version
with open(chpl_home + '/third-party/llvm/LLVM_VERSION', 'r') as f:
    llvm_version = float(f.readline())
if llvm_version < 6.0:
    debug_option = ' -debug-dump=str '
else:
    debug_option = ' -debug-str '

# Check Debug Info Existence
Command_check = llvm_tool_path + os.sep + 'llvm-dwarfdump' + debug_option + target

output = subprocess.check_output(Command_check, shell=True)
# Verify the module
if 'My_foo' in output:
    print 'checking module --PASS'
else:
    print 'checking module --FAIL'

# Verify the functions
#if 'My_sayhello' in output:
#    if 'My_factorial' in output:
#        if 'main' in output:
#            print 'checking functions --PASS'
#else:
#    print 'checking functions --FAIL'

# Verify the struct types
if ('My_Actor' in output) and ('My_name' in output) and ('My_age' in output):
    print 'checking types --PASS'
else:
    print 'checking types --FAIL'

# Verify the global variables
#if 'My_message' in output:
#    if 'My_gv_t' in output:
#        if 'My_gv_b' in output:
#            if 'My_globalActor' in output:
#                print 'checking global variables --PASS'
#else:
#    print 'checking global variables --FAIL'

# Verify the local variables
#if 'My_anything' in output:
#    if 'My_localActor' in output:
#        if 'My_Number' in output:
#            print 'checking local variables --PASS'
#else:
#    print 'checking local variables --FAIL'

# Verify the formal arguments
#if 'My_level' in output:
#    if 'My_x' in output:
#        print 'checking formal arguments --PASS'
#else:
#    print 'checking formal arguments --FAIL'
#
print 'DONE with debug checking'

