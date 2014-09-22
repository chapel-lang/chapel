/*
 * Copyright 2004-2014 Cray Inc.
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

#ifndef _chpl_rt_comp_detect_h_
#define _chpl_rt_comp_detect_h_

#define RT_COMP_UNKNOWN 0
#define RT_COMP_CRAY 1
#define RT_COMP_INTEL 2
#define RT_COMP_CLANG 4
#define RT_COMP_GCC 8
#define RT_COMP_PGI 16

#if defined(_CRAYC)
#define RT_COMP_CC RT_COMP_CRAY
#elif defined(__INTEL_COMPILER)
#define RT_COMP_CC RT_COMP_INTEL
#elif defined(__clang__)
#define RT_COMP_CC RT_COMP_CLANG
#elif defined(__PGI)
#define RT_COMP_CC RT_COMP_PGI
#elif defined(__GNUC__)
#define RT_COMP_CC RT_COMP_GCC
#else
#define RT_COMP_CC RT_COMP_UNKNOWN
#endif

#endif // _chpl_rt_comp_detect_h_
