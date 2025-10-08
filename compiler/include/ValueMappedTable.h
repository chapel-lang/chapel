/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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

#ifndef _VALUE_MAPPED_TABLE_H_
#define _VALUE_MAPPED_TABLE_H_

#include "misc.h"

#include <limits>
#include <optional>
#include <unordered_set>
#include <vector>

/** This datastructure is used to represent a table which is also indexable
    by values of its entry type. It is suitable for representing e.g., the
    filename table or the function table. */
template <typename T>
class ValueMappedTable {
 public:
  using Table = std::vector<T>;

 private:
  std::unordered_map<T, int> valueToIndex_;
  Table table_;

 public:
  ValueMappedTable() = default;
 ~ValueMappedTable() = default;

  ValueMappedTable(Table initial)
    : table_(std::move(initial)) {
    for (int i = 0; i < table_.size(); i++) {
      valueToIndex_.insert({table_[i], i});
    }
  }

  int add(const T& entry) {
    if (table_.size() > std::numeric_limits<int>::max()) {
      INT_FATAL("Table is at maximum size and will overflow!");
    }

    auto it = valueToIndex_.find(entry);
    if (it != valueToIndex_.end()) return it->second;

    int ret = ((int) table_.size());
    valueToIndex_.emplace_hint(it, entry, ret);
    table_.push_back(entry);

    return ret;
  }

  std::optional<int> index(const T& entry) const {
    if (auto it = valueToIndex_.find(entry); it != valueToIndex_.end()) {
      return it->second;
    }
    return {};
  }

  int size() const {
    return ((int) table_.size());
  }

  const Table& table() const {
    return table_;
  }
};

#endif
