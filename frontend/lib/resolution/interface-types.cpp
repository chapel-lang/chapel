/*
 * Copyright 2024-2025 Hewlett Packard Enterprise Development LP
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

#include "chpl/resolution/interface-types.h"

namespace chpl {
namespace resolution {

using namespace uast;
using namespace types;

owned<ImplementationPoint> const&
ImplementationPoint::getImplementationPoint(Context* context, const InterfaceType* interface,
                                            ID id) {
  QUERY_BEGIN(getImplementationPoint, context, interface, id);
  auto result = toOwned(new ImplementationPoint(interface, std::move(id)));
  return QUERY_END(result);
}

const ImplementationPoint*
ImplementationPoint::get(Context* context, const InterfaceType* interface,
                         ID id) {
  return getImplementationPoint(context, interface, std::move(id)).get();
}

void ImplementationPoint::mark(Context* context) const {
  interface_->mark(context);
  id_.mark(context);
}

void ImplementationPoint::stringify(std::ostream& ss, chpl::StringifyKind stringKind) const {
  ss << "implements(";
  interface_->stringify(ss, stringKind);
  ss << " via )";
  id_.stringify(ss, stringKind);
}

const owned<ImplementationWitness>&
ImplementationWitness::getImplementationWitness(Context* context,
                                                ConstraintMap associatedConstraints,
                                                AssociatedTypeMap associatedTypes,
                                                FunctionMap requiredFns,
                                                bool allGenerated) {
  QUERY_BEGIN(getImplementationWitness, context, associatedConstraints,
              associatedTypes, requiredFns, allGenerated);

  auto result = toOwned(new ImplementationWitness(std::move(associatedConstraints),
                                                  std::move(associatedTypes),
                                                  std::move(requiredFns),
                                                  allGenerated));

  return QUERY_END(result);
}

ImplementationWitness* ImplementationWitness::get(Context* context,
                                                  ConstraintMap associatedConstraints,
                                                  AssociatedTypeMap associatedTypes,
                                                  FunctionMap requiredFns,
                                                  bool allGenerated) {
  return getImplementationWitness(context, std::move(associatedConstraints),
                                  std::move(associatedTypes),
                                  std::move(requiredFns),
                                  allGenerated).get();
}

void ImplementationWitness::stringify(std::ostream& ss, chpl::StringifyKind stringKind) const {
  ss << "witness(";
  ss << "constraints: ";
  for (auto& c : associatedConstraints_) {
    c.first.stringify(ss, stringKind);
    ss << " => ";
    c.second->stringify(ss, stringKind);
    ss << ", ";
  }
  ss << "associated types: ";
  for (auto& a : associatedTypes_) {
    a.first.stringify(ss, stringKind);
    ss << " => ";
    a.second->stringify(ss, stringKind);
    ss << ", ";
  }
  ss << "required functions: ";
  for (auto& f : requiredFns_) {
    f.first.stringify(ss, stringKind);
    ss << " => ";
    CHPL_ASSERT(f.second.size() >= 1);
    f.second[0]->stringify(ss, stringKind);
    ss << ", ";
  }
  ss << ")";
}

} // end namespace resolution
} // end namespace chpl
