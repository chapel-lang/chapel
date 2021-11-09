/*
 * Copyright 2021 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_UTIL_ITERATION_H
#define CHPL_UTIL_ITERATION_H

namespace chpl {

/**
 Defines a read-only iterator over elements of a container type C
 */
template <typename C> class Iterable {
 private:
  typename C::const_iterator begin_;
  typename C::const_iterator end_;

 public:
  Iterable(const C &c) : begin_(c.cbegin()), end_(c.cend()) {}

  typename C::const_iterator begin() const { return begin_; }
  typename C::const_iterator end() const { return end_; }
};

} // namespace chpl

#endif
