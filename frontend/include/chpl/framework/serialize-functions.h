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
  This file implements the generic chpl::stringify
  as well as specializations for some common types.

  stringify can be used to get a string representation of an object
  for debugging purposes or whenever a string representation of an
  object is needed
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


namespace chpl {
class Context;

template<typename T> struct serialize;
template<typename T> struct deserialize;

class Serializer {
private:
  std::ostream& os_;

public:
  Serializer(std::ostream& os): os_(os) {
  }

  std::ostream& os() const {
    return os_;
  }

  template <typename T>
  void write(const T& data) {
    chpl::serialize<T>{}(*this, data);
  }

  template <typename T>
  void operator()(const T& data) {
    chpl::serialize<T>{}(*this, data);
  }
};

class Deserializer {
  private:
    Context* context_;
    std::istream& is_;

  public:
    Deserializer(Context* context, std::istream& is)
      : context_(context), is_(is) { }

    Context* context() const {
      return context_;
    }

    std::istream& is() const {
      return is_;
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

// define serialize for pointers to call stringify on a reference to avoid
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
    ser((CONV)val); \
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
    ser((uint64_t)val.size());
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
    }
    return std::string(buf, len);
  }
};

// stringify a vector by stringifying each element; uses [a, b, c] formatting
template<typename T>
static inline void defaultSerializeVec(Serializer& ser,
                                       const std::vector<T>& stringVec) {
  ser((uint64_t)stringVec.size());
  for (const auto &elt : stringVec ) {
    ser(elt);
  }
}

template<typename T>
static inline void defaultDeserializeVec(Deserializer& des,
                                         std::vector<T>& vec) {
  auto n = des.read<uint64_t>();
  for (uint64_t i = 0; i < n; i++) {
    vec.push_back(des.read<T>());
  }
}

template<typename T>
static inline void defaultSerializeSet(Serializer& ser,
                                       const std::set<T>& val) {
  ser((uint64_t)val.size());
  for (const auto& elt : val) {
    ser(elt);
  }
}

template<typename T>
static inline void defaultDeserializeSet(Deserializer& des,
                                         std::set<T>& val) {
  auto len = des.read<uint64_t>();
  for (uint64_t i = 0; i < len; i++) {
    val.insert(des.read<T>());
  }
}

template<typename T> struct serialize<std::vector<T>> {
 void operator()(Serializer& ser,
                 const std::vector<T>& stringMe) const {
   defaultSerializeVec(ser, stringMe);
 }
};

template<typename T> struct deserialize<std::vector<T>> {
  std::vector<T> operator()(Deserializer& des) const {
    std::vector<T> ret;
    defaultDeserializeVec(des, ret);
    return ret;
  }
};

template<typename T> struct serialize<std::set<T>> {
 void operator()(Serializer& ser,
                 const std::set<T>& val) const {
   defaultSerializeSet(ser, val);
 }
};

template<typename T> struct deserialize<std::set<T>> {
  std::set<T> operator()(Deserializer& des) const {
    std::set<T> ret;
    defaultDeserializeSet(des, ret);
    return ret;
  }
};


} // end namespace chpl

#endif
