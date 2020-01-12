/*
 * Copyright 2004-2020 Cray Inc.
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

// checks.h
#ifndef CHECKS_H
#define CHECKS_H

// These are the entry points for per-pass checks.
void check_parse();
void check_checkParsed();
void check_readExternC();
void check_expandExternArrayCalls();
void check_cleanup();
void check_scopeResolve();
void check_flattenClasses();
void check_docs();
void check_normalize();
void check_checkNormalized();
void check_buildDefaultFunctions();
void check_createTaskFunctions();
void check_resolve();
void check_resolveIntents();
void check_checkResolved();
void check_replaceArrayAccessesWithRefTemps();
void check_flattenFunctions();
void check_cullOverReferences();
void check_lowerErrorHandling();
void check_callDestructors();
void check_lowerIterators();
void check_parallel();
void check_prune();
void check_bulkCopyRecords();
void check_removeUnnecessaryAutoCopyCalls();
void check_inlineFunctions();
void check_scalarReplace();
void check_refPropagation();
void check_copyPropagation();
void check_deadCodeElimination();
void check_removeEmptyRecords();
void check_localizeGlobals();
void check_loopInvariantCodeMotion();
void check_prune2();
void check_returnStarTuplesByRefArgs();
void check_insertWideReferences();
void check_optimizeOnClauses();
void check_addInitCalls();
void check_insertLineNumbers();
void check_denormalize();
void check_codegen();
void check_makeBinary();

#endif
