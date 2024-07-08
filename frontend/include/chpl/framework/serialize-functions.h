/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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
#ifndef CHPL_FRAMEWORK_SERIALIZATION_H
#define CHPL_FRAMEWORK_SERIALIZATION_H

#include "chpl/util/assertions.h"
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

// forward declarations
class Context;
template<typename T> struct serialize;
template<typename T> struct deserialize;

namespace uast {
  class AstNode;
}
namespace libraries {
  class LibraryFileSerializationHelper;
  class LibraryFileDeserializationHelper;
}

/** this class is what is passed to serialize methods & helps
    with the process */
class Serializer {
public:
  // strings >= this length will go through the long string table.
  static const int LONG_STRING_SIZE = 20;
  // serializing a string > this long will result in an error
  static const int MAX_STRING_SIZE = 100000000;

  // Note: currently these char* entries are expected to support UniqueStrings,
  // which are allocated from a Context and are null-terminated.
  // Maps from string to {index in the long strings table, length}
  using stringCacheType = std::map<const char*, std::pair<uint32_t, size_t>>;

private:
  uint32_t longStringCounter_ = 1;
  std::ostream& os_;
  libraries::LibraryFileSerializationHelper* libraryFileHelper_ = nullptr;
  stringCacheType stringCache_;
  bool ok_ = true;

  /** write a variable-byte encoded unsigned integer */
  void writeUnsignedVarint(uint64_t num);
  /** write a variable-byte encoded signed integer */
  void writeSignedVarint(int64_t num);

public:
  Serializer(std::ostream& os,
             libraries::LibraryFileSerializationHelper* helper)
    : os_(os), libraryFileHelper_(helper) {
  }

  const stringCacheType& stringCache() {
    return stringCache_;
  }

  template <typename T>
  void write(const T& data) {
    chpl::serialize<T>{}(*this, data);
  }

  /** write one byte */
  void writeByte(unsigned char b) {
    os_.put(b);
  }

  /** write a numeric type */
  template <typename T>
  void writeNumeric(T val) {
    os_.write(reinterpret_cast<const char*>(&val), sizeof(val));
  }

  /** write 'n' bytes from the pointer 'ptr' */
  void writeData(const void* ptr, size_t n) {
    os_.write(reinterpret_cast<const char*>(ptr), n);
  }

  /** Check that a string length is OK.
      If it is, return 'true' and take no other action.
      If it is not, record an error in this Serializer and return 'false'. */
  bool checkStringLength(size_t len, size_t max = MAX_STRING_SIZE) {
    if (len < max) {
      return true;
    } else {
      ok_ = false;
      return false;
    }
  }


  /** Save a string in the long string table */
  uint32_t saveLongString(const char* str, size_t len) {
    if (checkStringLength(len)) {
      auto idx = stringCache_.find(str);
      if (idx == stringCache_.end()) {
        auto ret = longStringCounter_;
        stringCache_.insert({str, {ret,len}});
        longStringCounter_ += 1;
        return ret;
      } else {
        return idx->second.first;
      }
    }

    return 0; // value should not be used because we set !ok in this case
  }

  uint32_t nextStringIdx() { return longStringCounter_; }

  void beginAst(const uast::AstNode* ast);
  void endAst(const uast::AstNode* ast);

  /** Return 'false' if an error was encountered */
  bool ok() const { return ok_ && os_.good(); }

  /* Write a variable-length byte-encoded 64-bit unsigned integer */
  void writeVU64(uint64_t num) {
    writeUnsignedVarint(num);
  }
  /* Write a variable-length byte-encoded 64-bit signed integer */
  void writeVI64(int64_t num) {
    writeSignedVarint(num);
  }
  /* Write a variable-length byte-encoded 'unsigned int' */
  void writeVUint(unsigned int num) {
    writeUnsignedVarint(num);
  }
  /* Write a variable-length byte-encoded 'int' */
  void writeVInt(int num) {
    writeSignedVarint(num);
  }
};


/** this class is what is passed to deserialize methods & helps
    with the process */
class Deserializer {
 public:
  // Note: currently these char* entries are expected to support
  // UniqueStrings, which are allocated from a Context and are
  // null-terminated.
  using stringCacheType = std::vector<std::pair<size_t, const char*>>;
  // deserializing a string > this long will result in an error
  static const int MAX_STRING_SIZE = 100000000;

 private:
  Context* context_ = nullptr;
  const unsigned char* start_ = nullptr;
  const unsigned char* cur_ = nullptr;
  const unsigned char* end_ = nullptr;
  owned<stringCacheType> localStringsTable_;
  libraries::LibraryFileDeserializationHelper* libraryFileHelper_ = nullptr;
  bool ok_ = true;

  /** read a variable-byte encoded unsigned integer */
  uint64_t readUnsignedVarint();
  /** read a variable-byte encoded signed integer */
  int64_t readSignedVarint();

 public:
  Deserializer(Context* context, const void* data, size_t len)
    : context_(context),
      start_((const unsigned char*) data),
      cur_((const unsigned char*) data),
      end_(((const unsigned char*) data) + len),
      libraryFileHelper_(nullptr) {
  }

  Deserializer(Context* context,
               const void* start, const void* cur, const void* end,
               libraries::LibraryFileDeserializationHelper& helper)
    : context_(context),
      start_((const unsigned char*) start),
      cur_((const unsigned char*) cur),
      end_((const unsigned char*) end),
      libraryFileHelper_(&helper) {
    CHPL_ASSERT(start_ <= cur_ && cur_ <= end_);
  }

  //
  // Convenience version to convert a Serializer's form of the string cache
  //
  Deserializer(Context* context,
               const void* data, size_t len,
               Serializer::stringCacheType serCache)
    : context_(context),
      start_((const unsigned char*) data),
      cur_((const unsigned char*) data),
      end_(((const unsigned char*) data) + len) {
    localStringsTable_.reset(new stringCacheType());
    localStringsTable_->resize(serCache.size()+1);
    stringCacheType& table = *localStringsTable_.get();
    for (const auto& pair : serCache) {
      table[pair.second.first] = {pair.second.second, pair.first};
    }
  }

  Context* context() const {
    return context_;
  }

  /** Get a string from the long strings table by index */
  std::pair<size_t, const char*> getString(int id);

  /** Return the current offset into the deserialization region */
  uint64_t position() {
    return cur_ - start_;
  }

  /** Check that a string length is OK (but not assuming it will be read).
      If it is OK, return 'true' and take no other action.
      If it is not, record an error in this Deserializer and return 'false'. */
  bool checkStringLength(size_t len, size_t max = MAX_STRING_SIZE) {
    if (len < max) {
      return true;
    } else {
      ok_ = false;
      return false;
    }
  }

  /** Check that a string length is OK & it would be valid to read
      'len' bytes from here.
      If it is OK, return 'true' and take no other action.
      If it is not, record an error in this Deserializer and return 'false'. */
  bool checkStringLengthAvailable(size_t len, size_t max = MAX_STRING_SIZE) {
    if (len < max && cur_ + len <= end_) {
      return true;
    } else {
      ok_ = false;
      return false;
    }
  }

  /** Return 'false' if an error was encountered */
  bool ok() const { return ok_; }

  void registerAst(const uast::AstNode* ast, uint64_t startOffset);

  template <typename T>
  T operator()() {
    return chpl::deserialize<T>{}(*this);
  }

  template <typename T>
  T read() {
    return chpl::deserialize<T>{}(*this);
  }

  /** Gets a byte */
  unsigned char readByte() {
    if (cur_ < end_) {
      unsigned char ret = *cur_;
      cur_++;
      return ret;
    } else {
      ok_ = false;
      return 0;
    }
  }

  /** read a numeric type */
  template <typename T>
  T readNumeric() {
    if (cur_ + sizeof(T) <= end_) {
      T ret;
      memcpy(&ret, cur_, sizeof(T));
      cur_ += sizeof(T);
      return ret;
    } else {
      ok_ = false;
      return 0;
    }
  }

  /** read 'n' bytes into the pointer 'ptr' */
  void readData(void* ptr, size_t n) {
    if (cur_ + n <= end_) {
      memcpy(ptr, cur_, n);
      cur_ += n;
    } else {
      memset(ptr, 0, n);
      ok_ = false;
    }
  }

  /** Read a variable-length byte-encoded unsigned integer
      and return a 'uint64_t'*/
  uint64_t readVU64() {
    return readUnsignedVarint();
  }
  /** Read a variable-length byte-encoded signed integer & return an 'int64_t'
   */
  int64_t readVI64() {
    return readSignedVarint();
  }
  /** Read a variable-length byte-encoded unsigned integer
      and return an 'unsigned int'*/
  unsigned int readVUint() {
    return readUnsignedVarint();
  }
  /** Read a variable-length byte-encoded signed integer & return an 'int'*/
  int readVInt() {
    return readSignedVarint();
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
    auto ret = des.readNumeric<CONV>(); \
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
    ser.writeNumeric<TYPE>(val); \
  } \
}; \
template<> struct deserialize<TYPE> { \
  TYPE operator()(Deserializer& des) { \
    return des.readNumeric<TYPE>(); \
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
    ser.writeNumeric<uint8_t>(val);
  }
};

template<> struct deserialize<bool> {
  bool operator()(Deserializer& des) {
    uint8_t val = des.readNumeric<uint8_t>();
    return (bool)val;
  }
};

/*
 * Serialize std::string
 */
template<> struct serialize<std::string> {
  void operator()(Serializer& ser, const std::string& val) const {
    uint64_t len = val.size();
    if (ser.checkStringLength(len)) {
      ser.writeVU64(len);
      if (len > 0) {
        ser.writeData(val.c_str(), len);
      }
    }
  }
};

template<> struct deserialize<std::string> {
  std::string operator()(Deserializer& des) {
    uint64_t len = des.readVU64();
    if (des.checkStringLengthAvailable(len)) {
      if (len > 0) {
        std::string ret(len, 0);
        des.readData(&ret[0], len);
        return ret;
      }
    }
    return std::string();
  }
};

template<typename T> struct serialize<std::vector<T>> {
 void operator()(Serializer& ser,
                 const std::vector<T>& vec) const {
   ser.writeVU64(vec.size());
   for (const auto &elt : vec ) {
     ser.write(elt);
   }
 }
};

template<typename T> struct deserialize<std::vector<T>> {
  std::vector<T> operator()(Deserializer& des) const {
    std::vector<T> ret;
    uint64_t n = des.readVU64();
    for (uint64_t i = 0; i < n; i++) {
      ret.push_back(des.read<T>());
    }
    return ret;
  }
};

template<typename T> struct serialize<std::set<T>> {
 void operator()(Serializer& ser,
                 const std::set<T>& val) const {
   ser.writeVU64(val.size());
   for (const auto& elt : val) {
     ser.write(elt);
   }
 }
};

template<typename T> struct deserialize<std::set<T>> {
  std::set<T> operator()(Deserializer& des) const {
    std::set<T> ret;
    uint64_t len = des.readVU64();
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
