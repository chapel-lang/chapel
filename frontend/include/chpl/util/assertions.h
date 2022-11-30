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

#ifndef CHPL_UTIL_ASSERTIONS_H
#define CHPL_UTIL_ASSERTIONS_H


namespace chpl {

/*
  Custom assertion implementation to avoid performing assertions in dyno when
  --no-devel is passed, and also to not halt when an assertion fails if
  --ignore-errors was passed at runtime (of chpl, not the chapel runtime)
*/
void assertion(bool expr, const char* filename, const char* func, int lineno, const char* exprText);

/*
  Macro for our custom assertion mechanism - folded out if NDEBUG is defined
*/
#ifdef NDEBUG
// release mode
#define CHPL_ASSERT(expr__) \
  do { \
    bool ignore = expr__; \
    (void) ignore; \
   } while (0)
#else
// debug mode
#define CHPL_ASSERT(expr__) \
  do { \
    chpl::assertion(expr__, __FILE__, __FUNCTION__, __LINE__, #expr__); \
  } while (0)
#endif

} // namespace chpl


#endif // CHPL_UTIL_ASSERTIONS_H