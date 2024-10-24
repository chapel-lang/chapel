/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#ifndef CHPL_UTIL_FILTERING_H
#define CHPL_UTIL_FILTERING_H

#include <string>

namespace chpl
{

/*
  Removes the Sphinx inline markup for printing messages to the console.
  See https://chapel-lang.org/docs/tools/chpldoc/chpldoc.html#inline-markup-2
*/
std::string removeSphinxMarkup(const std::string& msg);

} // namespace chpl


#endif // CHPL_UTIL_FILTERING_H
