/*
 * Copyright 2004-2017 Cray Inc.
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

// LocaleTree.chpl
//
// A tree of locales used for recursive task invocation during privatization
//

module LocaleTree {

  use ChapelLocale; // For declaration of rootLocale.

  record chpl_localeTreeRecord {
    var left, right: locale;
  }

  pragma "locale private" var chpl_localeTree: chpl_localeTreeRecord;

  proc chpl_initLocaleTree() {
    for i in LocaleSpace {
      var left: locale = nil;
      var right: locale = nil;
      var child = (i+1)*2-1;    // Assumes that indices are dense.
      if child < numLocales {
        left = rootLocale.getChild(child);
        child += 1;
        if child < numLocales then
          right = rootLocale.getChild(child);
      }
      on rootLocale.getChild(i) {
        chpl_localeTree.left = left;
        chpl_localeTree.right = right;
      }
    }
  }

  chpl_initLocaleTree();

  iter localeTree(root = here, parent = here) : locale {
    halt("DONT USE SERIAL ITER");
    yield parent;
  }

  /*
   Logarithmic gather iterator
   */
  iter localeTree(param tag:iterKind, root = here, parent = here) : locale where tag == iterKind.standalone {
    on root {
      cobegin {
        if chpl_localeTree.left {
          for i in localeTree(tag=iterKind.standalone, chpl_localeTree.left, root) do yield i;
        }
        if chpl_localeTree.right {
          for i in localeTree(tag=iterKind.standalone, chpl_localeTree.right, root) do yield i;
        }
      }
      if parent != root then yield parent;
    }
  }

  iter localeTreeDown(root = here) : locale {
    halt("DONT USE SERIAL ITER");
    yield root;
  }

  /*
    Logarithmic broadcast iterator
   */
  iter localeTreeDown(param tag:iterKind, root = here) : locale where tag == iterKind.standalone {
    on root {
      cobegin {
        if chpl_localeTree.left {
          yield chpl_localeTree.left;
          for i in localeTreeDown(tag=iterKind.standalone, chpl_localeTree.left) do yield i;
        }
        if chpl_localeTree.right {
          yield chpl_localeTree.right;
          for i in localeTreeDown(tag=iterKind.standalone, chpl_localeTree.right) do yield i;
        }
      }
    }
  }


}
