# Copyright 2021-2023 Hewlett Packard Enterprise Development LP
# Other additional copyright holders may be indicated within.
#
# The entirety of this work is licensed under the Apache License,
# Version 2.0 (the "License"); you may not use this file except
# in compliance with the License.
#
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

target_sources(ChplFrontend-obj
               PRIVATE

               AnyType.cpp
               ArrayType.cpp
               BasicClassType.cpp
               BoolType.cpp
               BuiltinType.cpp
               CStringType.cpp
               ClassType.cpp
               ClassTypeDecorator.cpp
               ComplexType.cpp
               CompositeType.cpp
               DomainType.cpp
               EnumType.cpp
               ErroneousType.cpp
               ImagType.cpp
               IntType.cpp
               NilType.cpp
               NothingType.cpp
               Param.cpp
               PrimitiveType.cpp
               QualifiedType.cpp
               RealType.cpp
               RecordType.cpp
               TupleType.cpp
               Type.cpp
               TypeTag.cpp
               UintType.cpp
               UnionType.cpp
               UnknownType.cpp
               VoidType.cpp

              )
