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

#include "chpl/types/CompositeType.h"

#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/can-pass.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/types/BasicClassType.h"
#include "chpl/types/ClassType.h"
#include "chpl/types/RecordType.h"
#include "chpl/uast/Decl.h"
#include "chpl/uast/NamedDecl.h"

namespace chpl {
namespace types {


using namespace resolution;

bool
CompositeType::areSubsInstantiationOf(Context* context,
                                      const CompositeType* partial) const {
  // Check to see if the substitutions of `this` are all instantiations
  // of the field types of `partial`
  //
  // Note: Assumes 'this' and 'partial' share a root instantiation.

  const SubstitutionsMap& mySubs = substitutions();
  const SubstitutionsMap& pSubs = partial->substitutions();

  // check, for each substitution in mySubs, that it matches
  // or is an instantiation of pSubs.

  for (const auto& mySubPair : mySubs) {
    ID mySubId = mySubPair.first;
    QualifiedType mySubType = mySubPair.second;

    // look for a substitution in pSubs with the same ID
    auto pSearch = pSubs.find(mySubId);
    if (pSearch != pSubs.end()) {
      QualifiedType pSubType = pSearch->second;
      // check the types
      auto r = canPass(context, mySubType, pSubType);
      if (r.passes() && !r.promotes() && !r.converts()) {
        // instantiation and same-type passing are allowed here
      } else {
        // it was not an instantiation
        return false;
      }
    } else {
      // If the ID isn't found, then that means the generic component doesn't
      // exist in the other type, which means this cannot be an instantiation
      // of the other type.
      //
      // Currently this check assumes that 'this' and 'partial' share a root
      // instantiation, so how could we reach this condition? One path here
      // involves passing a tuple to a tuple formal with a fewer number of
      // elements. For example, passing "(1, 2, 3)" to "(int, ?)".
      return false;
    }
  }

  return true;
}

CompositeType::~CompositeType() {
}

void CompositeType::stringify(std::ostream& ss,
                              chpl::StringifyKind stringKind) const {
  // compute the parent class type for BasicClassType
  const Type* superType = nullptr;
  if (auto bct = this->toBasicClassType()) {
    superType = bct->parentClassType();
  }

  //std::string ret = typetags::tagToString(tag());
  name().stringify(ss, stringKind);

  auto sorted = sortedSubstitutions();

  bool printSupertype =
    superType != nullptr && stringKind != StringifyKind::CHPL_SYNTAX;

  // Prepend parent substitutions to 'sorted' list
  if (!printSupertype && superType != nullptr) {
    auto cur = superType->toBasicClassType();
    while (cur != nullptr) {
      auto parentSubs = cur->getCompositeType()->sortedSubstitutions();
      sorted.insert(sorted.begin(), parentSubs.begin(), parentSubs.end());
      cur = cur->parentClassType();
    }
  }

  if (printSupertype || !sorted.empty()) {
    bool emittedField = false;
    ss << "(";

    if (printSupertype) {
      ss << "super:";
      superType->stringify(ss, stringKind);
      emittedField = true;
    }

    for (const auto& sub : sorted) {
      if (emittedField) ss << ", ";

      if (stringKind != StringifyKind::CHPL_SYNTAX) {
        sub.first.stringify(ss, stringKind);
        ss << ":";
        sub.second.stringify(ss, stringKind);
      } else {
        if (sub.second.isType() || (sub.second.isParam() && sub.second.param() == nullptr)) {
          sub.second.type()->stringify(ss, stringKind);
        } else if (sub.second.isParam()) {
          sub.second.param()->stringify(ss, stringKind);
        } else {
          // Some odd configuration; fall back to printing the qualified type.
          CHPL_UNIMPL("attempting to stringify odd type representation as Chapel syntax");
          sub.second.stringify(ss, stringKind);
        }
      }

      emittedField = true;
    }
    ss << ")";
  }
}

const RecordType* CompositeType::getStringType(Context* context) {
  auto name = UniqueString::get(context, "string");
  auto id = parsing::getSymbolFromTopLevelModule(context, "String", "_string");
  return RecordType::get(context, id, name,
                         /* instantiatedFrom */ nullptr,
                         SubstitutionsMap());
}

const RecordType* CompositeType::getRangeType(Context* context) {
  auto name = UniqueString::get(context, "_range");
  auto id = parsing::getSymbolFromTopLevelModule(context, "ChapelRange", "_range");
  return RecordType::get(context, id, name,
                         /* instantiatedFrom */ nullptr,
                         SubstitutionsMap());
}

const RecordType* CompositeType::getBytesType(Context* context) {
  auto name = UniqueString::get(context, "bytes");
  auto id = parsing::getSymbolFromTopLevelModule(context, "Bytes", "_bytes");
  return RecordType::get(context, id, name,
                         /* instantiatedFrom */ nullptr,
                         SubstitutionsMap());
}

const RecordType* CompositeType::getLocaleType(Context* context) {
  auto name = UniqueString::get(context, "locale");
  auto id = parsing::getSymbolFromTopLevelModule(context, "ChapelLocale", "_locale");
  return RecordType::get(context, id, name,
                         /* instantiatedFrom */ nullptr,
                         SubstitutionsMap());
}

const RecordType* CompositeType::getLocaleIDType(Context* context) {
  auto name = UniqueString::get(context, "chpl_localeID_t");
  auto id = ID();
  return RecordType::get(context, id, name,
                         /* instantiatedFrom */ nullptr,
                         SubstitutionsMap());
}

bool CompositeType::isMissingBundledType(Context* context, ID id) {
  return isMissingBundledClassType(context, id) ||
         isMissingBundledRecordType(context, id);
}

bool CompositeType::isMissingBundledRecordType(Context* context, ID id) {
  bool noLibrary = parsing::bundledModulePath(context).isEmpty();
  if (noLibrary) {
    auto path = id.symbolPath();
    return path == "String._string" ||
           path == "ChapelRange._range" ||
           path == "ChapelTuple._tuple" ||
           path == "Bytes._bytes";
  }

  return false;
}

bool CompositeType::isMissingBundledClassType(Context* context, ID id) {
  bool noLibrary = parsing::bundledModulePath(context).isEmpty();
  if (noLibrary) {
    auto path = id.symbolPath();
    return path == "ChapelReduce.ReduceScanOp" ||
           path == "Errors.Error" || 
           path == "CTypes.c_ptr" ||
           path == "CTypes.c_ptrConst";
  }

  return false;
}

const ClassType* CompositeType::getErrorType(Context* context) {
  auto name = UniqueString::get(context, "Error");
  auto id = parsing::getSymbolFromTopLevelModule(context, "Errors", "Error");
  auto dec = ClassTypeDecorator(ClassTypeDecorator::GENERIC_NONNIL);
  auto bct = BasicClassType::get(context, id,
                                name,
                                BasicClassType::getRootClassType(context),
                                /* instantiatedFrom */ nullptr,
                                SubstitutionsMap());
  return ClassType::get(context, bct, /* manager */ nullptr, dec);
}


using SubstitutionPair = CompositeType::SubstitutionPair;

struct SubstitutionsMapCmp {
  bool operator()(const SubstitutionPair& x, const SubstitutionPair& y) {
    return x.first < y.first;
  }
};

static
std::vector<SubstitutionPair>
sortedSubstitutionsMap(const CompositeType::SubstitutionsMap& subs) {
  // since it's an unordered map, iteration will occur in a
  // nondeterministic order.
  // it's important to sort the keys / iterate in a deterministic order here,
  // so we create a vector of pair<K,V> and sort that instead
  std::vector<SubstitutionPair> v(subs.begin(), subs.end());
  SubstitutionsMapCmp cmp;
  std::sort(v.begin(), v.end(), cmp);
  return v;
}

std::vector<SubstitutionPair> CompositeType::sortedSubstitutions(void) const {
  return sortedSubstitutionsMap(subs_);
}

size_t hashSubstitutionsMap(const CompositeType::SubstitutionsMap& subs) {
  auto sorted = sortedSubstitutionsMap(subs);
  return hashVector(sorted);
}

void stringifySubstitutionsMap(std::ostream& streamOut,
                               StringifyKind stringKind,
                               const CompositeType::SubstitutionsMap& subs) {
  auto sorted = sortedSubstitutionsMap(subs);
  bool first = true;
  for (auto const& x : sorted)
  {
    ID id = x.first;
    QualifiedType qt = x.second;

    if (first) {
      first = false;
    } else {
      streamOut << ", ";
    }

    id.stringify(streamOut, stringKind);

    streamOut << "= ";

    qt.stringify(streamOut, stringKind);
  }
}


} // end namespace types
} // end namespace chpl
