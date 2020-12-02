#!/usr/bin/env python3

# Python test script to verify the llvm debug info
# is generated properly during Chapel compilation

import os, subprocess, sys
import shutil

chpl_home = sys.argv[1]

chplenv = subprocess.check_output([chpl_home + "/util/printchplenv",
                                   "--all", "--internal", "--simple"])

CHPL_LLVM=None
CHPL_HOST_BIN_SUBDIR=None
CHPL_LLVM_UNIQ_CFG_PATH=None

for line in chplenv.splitlines():
  line_str = str(line, encoding='utf-8', errors='surrogateescape')
  kv = line_str.split('=', 2)
  key = kv[0]
  val = kv[1]
  if key == 'CHPL_HOST_BIN_SUBDIR':
    CHPL_HOST_BIN_SUBDIR= val
  if key == 'CHPL_LLVM_UNIQ_CFG_PATH':
    CHPL_LLVM_UNIQ_CFG_PATH= val
  if key == 'CHPL_LLVM':
    CHPL_LLVM= val

build_options = '--baseline --llvm -g'
source_path = os.getcwd() #same as target path
source = source_path + os.sep + 'llvmDebug_test.chpl'
target = source_path + os.sep + 'llvmDebug_test'
# Build Chapel Test Program
Command_build = chpl_home + '/bin/' + CHPL_HOST_BIN_SUBDIR + '/chpl ' + build_options + ' ' + source + ' -o ' + target
if os.system(Command_build) == 0:
    print ('Build Succeeded')
else:
    print ('Build Failed')
    os._exit(1) #exit without raising an exception

# Determine syntax from LLVM version
llvm_vers_maj = 0
with open(chpl_home + '/third-party/llvm/LLVM_VERSION', 'r') as f:
    llvm_vers_maj = int(f.readline().split('.')[0])
if llvm_vers_maj < 6:
    debug_option = ' -debug-dump=str '
else:
    debug_option = ' -debug-str '

llvm_dwarfdump = None
if CHPL_LLVM == "bundled":
    llvm_dwarfdump = (chpl_home + '/third-party/llvm/install/' +
                      CHPL_LLVM_UNIQ_CFG_PATH + '/bin/llvm-dwarfdump')
else:
    llvm_dwarfdump = "llvm-dwarfdump-" + str(llvm_vers_maj)
    if not shutil.which(llvm_dwarfdump):
        llvm_dwarfdump = "llvm-dwarfdump"
        if not shutil.which(llvm_dwarfdump):
            print ('Could not find llvm-dwarfdump')
            os._exit(1) #exit without raising an exception

# Check Debug Info Existence
Command_check = llvm_dwarfdump + debug_option + target

output_bytes = subprocess.check_output(Command_check, shell=True)
output = str(output_bytes, encoding='utf-8', errors='surrogateescape')
# Verify the module
if 'My_foo' in output:
    print ('checking module --PASS')
else:
    print ('checking module --FAIL')

# Verify the functions
#if 'My_sayhello' in output:
#    if 'My_factorial' in output:
#        if 'main' in output:
#            print ('checking functions --PASS')
#else:
#    print ('checking functions --FAIL')

# Verify the struct types
if ('My_Actor' in output) and ('My_name' in output) and ('My_age' in output):
    print ('checking types --PASS')
else:
    print ('checking types --FAIL')

# Verify the global variables
#if 'My_message' in output:
#    if 'My_gv_t' in output:
#        if 'My_gv_b' in output:
#            if 'My_globalActor' in output:
#                print ('checking global variables --PASS')
#else:
#    print ('checking global variables --FAIL')

# Verify the local variables
#if 'My_anything' in output:
#    if 'My_localActor' in output:
#        if 'My_Number' in output:
#            print ('checking local variables --PASS')
#else:
#    print ('checking local variables --FAIL')

# Verify the formal arguments
#if 'My_level' in output:
#    if 'My_x' in output:
#        print ('checking formal arguments --PASS')
#else:
#    print ('checking formal arguments --FAIL')
#
print ('DONE with debug checking')

