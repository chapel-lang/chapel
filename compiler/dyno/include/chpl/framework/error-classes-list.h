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

ERROR_CLASS(IncompatibleIfBranches,
    const uast::Conditional*,
    types::QualifiedType,
    types::QualifiedType)
ERROR_CLASS(TupleExpansionNamedArgs, const uast::OpCall*, const uast::FnCall*)
ERROR_CLASS(MemManagementRecords, const uast::FnCall*, const uast::Record*)
ERROR_CLASS(PrivateToPublicInclude, const uast::Include*, const uast::Module*)
ERROR_CLASS(PrototypeInclude, const uast::Include*, const uast::Module*)
ERROR_CLASS(MissingInclude, const uast::Include*, std::string)
ERROR_CLASS(Redefinition, const uast::NamedDecl*, std::vector<ID>)
