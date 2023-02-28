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

/*
  This file implements the generic chpl::serialize and chpl::deserialize
  as well as specializations for some common types.
 */

#ifndef CHPL_QUERIES_SERIALIZE_FUNCTIONS_H
#define CHPL_QUERIES_SERIALIZE_FUNCTIONS_H

#include "chpl/util/memory.h"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

#include "llvm/ADT/DenseMap.h"


namespace chpl {
class Context;

template<typename T> struct serialize;
template<typename T> struct deserialize;

class Serializer {
private:
  using stringCacheType = std::map<const char*, std::pair<int, size_t>>;
  int counter_ = 0;

  std::ostream& os_;
  stringCacheType stringCache_;

public:
  Serializer(std::ostream& os): os_(os) {
  }

  std::ostream& os() const {
    return os_;
  }

  stringCacheType stringCache() {
    return stringCache_;
  }

  template <typename T>
  void write(const T& data) {
    chpl::serialize<T>{}(*this, data);
  }

  int cacheString(const char* str, size_t len) {
    auto idx = stringCache_.find(str);
    if (idx == stringCache_.end()) {
      auto ret = counter_;
      stringCache_.insert({str, {ret,len}});
      counter_ += 1;
      return ret;
    } else {
      return idx->second.first;
    }
  }
};

class Deserializer {
  private:
    Context* context_;
    std::istream& is_;
    using StringCache = std::vector<std::pair<size_t, const char*>>;
    StringCache cache_;

  public:
    Deserializer(Context* context, std::istream& is)
      : context_(context), is_(is) { }

    Deserializer(Context* context, std::istream& is, const StringCache& cache)
      : context_(context), is_(is), cache_(cache) { }

    //
    // Convenience version to convert a Serializer's form of the string cache
    //
    Deserializer(Context* context, std::istream& is,
                 std::map<const char*, std::pair<int, size_t>> serCache)
      : context_(context), is_(is) {
      cache_.resize(serCache.size());
      for (const auto& pair : serCache) {
        cache_[pair.second.first] = {pair.second.second, pair.first};
      }
    }

    Context* context() const {
      return context_;
    }

    std::istream& is() const {
      return is_;
    }

    std::pair<size_t, const char*>& getString(int id) {
      return cache_[id];
    }

    template <typename T>
    T operator()() {
      return chpl::deserialize<T>{}(*this);
    }

    template <typename T>
    T read() {
      return chpl::deserialize<T>{}(*this);
    }
};

// define the generic serialize template
template<typename T> struct serialize {
  void operator()(Serializer& ser, const T& value) {
    value.serialize(ser);
  };
};

// define serialize for pointers to call serialize on a reference to avoid
//  duplication for types that appear both as references and as pointers
template<typename T> struct serialize<const T*> {
  void operator()(Serializer& ser,
                  const T* value) const {
    assert(value != nullptr);
    serialize<T>{}(ser, *value);
  }
};

// define the generic deserialize template
template<typename T> struct deserialize {
  T operator()(Deserializer& des) {
    return T::deserialize(des);
  }
};


/*
 * Helper macro for enums
 */
#define DECLARE_SERDE_ENUM(TYPE, CONV) \
template<> struct serialize<TYPE> { \
  void operator()(Serializer& ser, TYPE val) { \
    ser.write((CONV)val); \
  } \
}; \
template<> struct deserialize<TYPE> { \
  TYPE operator()(Deserializer& des) { \
    auto ret = des.read<CONV>(); \
    return static_cast<TYPE>(ret); \
  } \
};

/*
  Templates for integral types start here
*/
#define SERIALIZE_PRIM(TYPE) \
template<> struct serialize<TYPE> { \
  void operator()(Serializer& ser, \
                  TYPE val) const { \
    ser.os().write(reinterpret_cast<const char*>(&val), sizeof(val)); \
  } \
}; \
template<> struct deserialize<TYPE> { \
  TYPE operator()(Deserializer& des) { \
    TYPE ret; \
    des.is().read(reinterpret_cast<char*>(&ret), sizeof(ret)); \
    return ret; \
  } \
};

SERIALIZE_PRIM(uint8_t);
SERIALIZE_PRIM(uint16_t);
SERIALIZE_PRIM(uint32_t);
SERIALIZE_PRIM(uint64_t);
SERIALIZE_PRIM(int8_t);
SERIALIZE_PRIM(int16_t);
SERIALIZE_PRIM(int32_t);
SERIALIZE_PRIM(int64_t);
SERIALIZE_PRIM(double);

#undef SERIALIZE_PRIM

template<> struct serialize<bool> {
  void operator()(Serializer& ser, bool val) const {
    uint8_t copy = val;
    ser.os().write(reinterpret_cast<const char*>(&copy), sizeof(copy));
  }
};

template<> struct deserialize<bool> {
  bool operator()(Deserializer& des) {
    uint8_t val;
    des.is().read(reinterpret_cast<char*>(&val), sizeof(val));
    return (bool)val;
  }
};

/*
 * Serialize std::string
 */
template<> struct serialize<std::string> {
  void operator()(Serializer& ser, const std::string& val) const {
    ser.write((uint64_t)val.size());
    if (val.size() > 0) {
      ser.os().write(val.c_str(), val.size());
    }
  }
};

template<> struct deserialize<std::string> {
  std::string operator()(Deserializer& des) {
    size_t len = (size_t)des.read<uint64_t>();
    char* buf = nullptr;
    if (len > 0) {
      buf = (char*)malloc(len+1);
      des.is().read(buf, len);
      buf[len] = '\0';
      auto ret = std::string(buf, len);
      free(buf);
      return ret;
    } else {
      return std::string();
    }
  }
};

template<typename T> struct serialize<std::vector<T>> {
 void operator()(Serializer& ser,
                 const std::vector<T>& stringVec) const {
   ser.write((uint64_t)stringVec.size());
   for (const auto &elt : stringVec ) {
     ser.write(elt);
   }
 }
};

template<typename T> struct deserialize<std::vector<T>> {
  std::vector<T> operator()(Deserializer& des) const {
    std::vector<T> ret;
    auto n = des.read<uint64_t>();
    for (uint64_t i = 0; i < n; i++) {
      ret.push_back(des.read<T>());
    }
    return ret;
  }
};

template<typename T> struct serialize<std::set<T>> {
 void operator()(Serializer& ser,
                 const std::set<T>& val) const {
   ser.write((uint64_t)val.size());
   for (const auto& elt : val) {
     ser.write(elt);
   }
 }
};

template<typename T> struct deserialize<std::set<T>> {
  std::set<T> operator()(Deserializer& des) const {
    std::set<T> ret;
    auto len = des.read<uint64_t>();
    for (uint64_t i = 0; i < len; i++) {
      ret.insert(des.read<T>());
    }
    return ret;
  }
};

template<typename K, typename V>
struct serialize<llvm::DenseMap<K,V>> {
  void operator()(Serializer& ser,
                  const llvm::DenseMap<K,V>& val) const {
    ser.write((uint64_t)val.size());
    for (const auto& pair : val) {
      ser.write(pair.first);
      ser.write(pair.second);
    }
  }
};

template<typename K, typename V>
struct deserialize<llvm::DenseMap<K,V>> {
  llvm::DenseMap<K,V> operator()(Deserializer& des) const {
    auto len = des.read<uint64_t>();
    llvm::DenseMap<K,V> ret(len);
    for (uint64_t i = 0; i < len; i++) {
      ret.insert({des.read<K>(), des.read<V>()});
    }
    return ret;
  }
};


} // end namespace chpl

#endif
