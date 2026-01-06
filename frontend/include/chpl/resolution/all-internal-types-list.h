/*
 * Copyright 2026 Hewlett Packard Enterprise Development LP
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

#ifndef START_INTERNAL_MODULE
#define START_INTERNAL_MODULE(modname__)
#endif

#ifndef END_INTERNAL_MODULE
#define END_INTERNAL_MODULE(modname__)
#endif

#ifndef INTERNAL_TYPE
#define INTERNAL_TYPE(typename__, definition__)
#endif

START_INTERNAL_MODULE(ChapelArray)
  INTERNAL_TYPE(_array, "record _array {}")
END_INTERNAL_MODULE(ChapelArray)

START_INTERNAL_MODULE(Bytes)
  INTERNAL_TYPE(_bytes, "record _bytes {}")
END_INTERNAL_MODULE(Bytes)

START_INTERNAL_MODULE(ChapelBase)
  INTERNAL_TYPE(iterKind, "enum iterKind { leader, follower, standalone }")
  INTERNAL_TYPE(_ddata, "record _ddata {}")
END_INTERNAL_MODULE(ChapelBase)

START_INTERNAL_MODULE(ChapelContext)
  INTERNAL_TYPE(contextManager, "interface contextManager {}")
END_INTERNAL_MODULE(ChapelContext)

START_INTERNAL_MODULE(CTypes)
  INTERNAL_TYPE(c_ptr, "record c_ptr {}")
  INTERNAL_TYPE(c_ptrConst, "record c_ptrConst {}")
END_INTERNAL_MODULE(CTypes)

START_INTERNAL_MODULE(ChapelDistribution)
  INTERNAL_TYPE(_distribution, "record _distribution {}")
END_INTERNAL_MODULE(ChapelDistribution)

START_INTERNAL_MODULE(ChapelDomain)
  INTERNAL_TYPE(_domain, "record _domain {}")
END_INTERNAL_MODULE(ChapelDomain)

START_INTERNAL_MODULE(ChapelLocale)
  INTERNAL_TYPE(_locale, "record _locale {}")
END_INTERNAL_MODULE(ChapelLocale)

START_INTERNAL_MODULE(LocaleModelHelpRuntime)
  INTERNAL_TYPE(chpl_localeID_t, "record chpl_localeID_t {}")
END_INTERNAL_MODULE(LocaleModelHelpRuntime)

START_INTERNAL_MODULE(OwnedObject)
  INTERNAL_TYPE(_owned, "record _owned {}")
END_INTERNAL_MODULE(OwnedObject)

START_INTERNAL_MODULE(ChapelRange)
  INTERNAL_TYPE(boundKind, "enum boundKind { both, low, high, neither };")
  INTERNAL_TYPE(_range, "record _range {}")
END_INTERNAL_MODULE(ChapelRange)

START_INTERNAL_MODULE(ChapelReduce)
  INTERNAL_TYPE(ReduceScanOp, "class ReduceScanOp {}")
END_INTERNAL_MODULE(ChapelReduce)

START_INTERNAL_MODULE(SharedObject)
  INTERNAL_TYPE(_shared, "record _shared {}")
END_INTERNAL_MODULE(SharedObject)

START_INTERNAL_MODULE(ChapelSyncvar)
  INTERNAL_TYPE(_syncvar, "record _syncvar {}")
END_INTERNAL_MODULE(ChapelSyncvar)

START_INTERNAL_MODULE(ChapelTuple)
  INTERNAL_TYPE(_tuple, "record _tuple {}")
END_INTERNAL_MODULE(ChapelTuple)

START_INTERNAL_MODULE(Errors)
  INTERNAL_TYPE(Error, "class Error {}")
END_INTERNAL_MODULE(Errors)

START_INTERNAL_MODULE(String)
  INTERNAL_TYPE(_string, "record _string {}")
END_INTERNAL_MODULE(String)

#undef START_INTERNAL_MODULE
#undef END_INTERNAL_MODULE
#undef INTERNAL_TYPE
