/*
 * Copyright 2004-2015 Cray Inc.
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

use Sort;

//
// This module defines several iterators that are designed to help
// support reasoning about directory and file contents.  At present,
// all are serial and single-locale, though the ultimate intention
// is to also support parallel and multi-locale options.  
//
// These interfaces should be considered experimental and subject to
// change for the 1.10 release.  For that reason, send in feedback if
// you would like to see changes or additional arguments/features.
// And, as always, send in bug reports if you think we got something
// wrong.
//



