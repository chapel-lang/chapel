/*
 * Copyright 2024 Hewlett Packard Enterprise Development LP 
 * Other additional copyright holders may be indicated within.
 *  
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *      
 * You may obtain a copy of the License at
 *      
 *     http://www.apache.org/licenses/LICENSE-2.0
 *        
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */       
            
#ifndef CHPL_FRAMEWORK_CHECK_BUILD_H
#define CHPL_FRAMEWORK_CHECK_BUILD_H

#include "llvm/Config/llvm-config.h"

/*

This file exists to check that the current configuration used by the header
files matches the configuration from when the .cpp files in the library were
built. Include this header file from one .cpp file in an application.  That will
cause it to run the check at runtime and print an error out to stderr if there
is a problem.

*/

// should not be necessary to call this directly, but any calls
// to it should pass no arguments
bool validateCompiledCorrectly(
      int headersHaveLlvm=
#ifdef HAVE_LLVM
        1,
#else
        0,
#endif
       int headersLlvmMajor=LLVM_VERSION_MAJOR
      );

// Initialize a global by running validateCompiledCorrectly()
// so that it is run automatically if this header file is included.
static bool compiledCorrectly=validateCompiledCorrectly();

#endif
