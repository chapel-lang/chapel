/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

//
// Expose the underlying locales array (and its domain) for user
// convenience and backward compatibility.
//
// RootLocale is a mechanism to provide this convenience.
//
// Note that rootLocale must be initialized before the Locales array
// can be initialized, but in order to initialize the replicated
// private versions of rootLocale, the Locales array and
// DefaultRectangular must be initialized.
//
// To solve this problem, the rootLocale is initialized on locale 0
// first, with all other locales pointing to this rootLocale.  After
// the Locales array and DefaultRectangular are initialized, we go
// back replicate the rootLocale on every other locale and update the
// Locales array to point to the local copy (if replicateRootLocale is
// true).  See chpl_rootLocaleInitPrivate() in ChapelLocale.chpl for
// more info on how this is done.
//

module LocalesArray {
  private use ChapelStandard;

  // Initialize the rootLocale
  chpl_init_rootLocale();

  // The Locales array should be private so that each locale can
  // point to its private copy of rootLocale, but we don't (yet) have
  // a good way to declare and initialize private aliases.  Instead,
  // we set up the version on all other locales during LocaleModel
  // initialization (see chpl_rootLocaleInitPrivate()).  The copy for
  // locale 0 is set up here for the declaration.
  pragma "locale private"
  const ref Locales = (rootLocale._instance:borrowed RootLocale?)!.getDefaultLocaleArray();

  // We don't use the same private "trick" as with Locales above with
  // LocaleSpace/ because it's small enough to not matter.
  const LocaleSpace = Locales.domain;

  chpl_set_defaultLocale(Locales[0]._value);

}

