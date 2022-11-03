/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_QUERIES_GLOBAL_STRINGS_H
#define CHPL_QUERIES_GLOBAL_STRINGS_H

#include "Context.h"

// NOTE: to add a global string for USTR, add it to
//   all-global-strings.h

/* Max length of a global string that can appear in a USTR("<something>") macro  */
/* If updating here, you should also ensure there is a EXPAND{MAXLEN} macro */
#define MAXLEN 16

/*
EXPAND4("foo", 0) expands to
  EXPAND2("foo", 0),
    EXPAND1("foo", 0),
      (sizeof("foo") - 1 < 0 ? '\0' : "foo"[0]),
    EXPAND1("foo", 1),
      (sizeof("foo") - 1 < 1 ? '\0' : "foo"[1]),
  EXPAND2("foo", 2)
    EXPAND1("foo", 3),
      (sizeof("foo") - 1 < 2 ? '\0' : "foo"[2]),
    EXPAND1("foo", 4),
      (sizeof("foo") - 1 < 3 ? '\0' : "foo"[3]),

which simplifies to
EXPAND4("foo", 0) => 'f', 'o', 'o', '\0'

And we can then use that in a template instantiation like
template<> USTRTemplate<'f, 'o', 'o', '\0'>
*/

#define EXPAND1(s, i) ((sizeof((s)) - 1) < (i) ? '\0' : s[(i)])

#define EXPAND2(s, i) \
  EXPAND1(s, i),      \
  EXPAND1(s, i + 1)

#define EXPAND4(s, i) \
  EXPAND2(s, i),      \
  EXPAND2(s, i + 2)

#define EXPAND8(s, i) \
  EXPAND4(s, i),      \
  EXPAND4(s, i + 4)

#define EXPAND16(s) \
  EXPAND8(s, 0),    \
  EXPAND8(s, 8)

#define USTR(s) ({static_assert(sizeof((s)) < MAXLEN, "String to USTR too long"); chpl::detail::USTRTemplate<EXPAND16((s))>::value;})

/// \cond DO_NOT_DOCUMENT
namespace chpl {

  namespace detail {
    struct GlobalStrings {
#define X(field, str) UniqueString field; static_assert(sizeof(str) < MAXLEN, "global string too long: " str);
  #include "all-global-strings.h"
#undef X
    };

    extern GlobalStrings globalStrings;

    /* This weird template is just to support a static_assert below,
     We need the truthiness to be dependent on the template
     parameters so it is't just static_assert(false, ...);*/
    template <char... T> struct dependent_false : std::false_type {};

    /* Base case for no matching global string */
    template <char... c> struct USTRTemplate {
      static_assert(dependent_false<c...>::value, "No matching USTR");
    };

    /* Stamp out a template specialization for each global string */
#define X(field, str)                                                          \
  template <> struct USTRTemplate<EXPAND16(str)> {                             \
    static constexpr UniqueString& value = globalStrings.field;                \
  };
#include "all-global-strings.h"
#undef X

   } // namespace detail
} // namespace chpl
/// \endcond

#endif
