/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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

COMPILER_GLOBAL(bool, "boundsChecking", boundsChecking)
COMPILER_GLOBAL(bool, "castChecking", castChecking)
COMPILER_GLOBAL(bool, "chpl_constArgChecking", constArgChecking)
COMPILER_GLOBAL(bool, "chpl_checkNilDereferences", nilDerefChecking)
COMPILER_GLOBAL(bool, "chpl_overloadSetsChecks", overloadSetsChecking)
COMPILER_GLOBAL(bool, "chpl_checkDivByZero", divByZeroChecking)
COMPILER_GLOBAL(bool, "CHPL_CACHE_REMOTE", cacheRemote)
COMPILER_GLOBAL(bool, "_privatization", privatization)
COMPILER_GLOBAL(bool, "_local", local)
COMPILER_GLOBAL(bool, "chpl_warnUnstable", warnUnstable)
