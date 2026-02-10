/*
 * Copyright 2023-2026 Hewlett Packard Enterprise Development LP
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

CLASS_BEGIN(Error)
  PLAIN_GETTER(Error, code_snippets, "Get the locations of code snippets printed by this error",
               std::vector<chpl::ErrorCodeSnippet>,

               CompatibilityWriter writer(context);
               (*node)->write(writer);
               return writer.codeSnippets())
  PLAIN_GETTER(Error, location, "Get the location at which this error occurred",
               chpl::Location, return (*node)->location(context))
  PLAIN_GETTER(Error, message, "Retrieve the contents of this error message",
               std::string, return (*node)->message())
  PLAIN_GETTER(Error, notes, "Get the locations and text of additional notes printed by this error",
               std::vector<LocationAndNote>,

               std::vector<LocationAndNote> toReturn;
               CompatibilityWriter writer(context);
               (*node)->write(writer);
               for (auto& note : writer.notes()) {
                 toReturn.push_back(std::make_tuple(std::get<0>(note).computeLocation(context),
                                                    std::get<1>(note)));
               }
               return toReturn)
  PLAIN_GETTER(Error, kind, "Retrieve the kind ('error', 'warning') of this type of error",
               const char*, return chpl::ErrorBase::getKindName((*node)->kind()))
  PLAIN_GETTER(Error, type, "Retrieve the unique name of this type of error",
               std::optional<const char*>,
               const char* name = chpl::ErrorBase::getTypeName((*node)->type());
               return name ? std::optional(name) : std::nullopt;
               )
CLASS_END(Error)

CLASS_BEGIN(ErrorManager)
  PLAIN_GETTER(ErrorManager, __enter__, "The context manager 'enter' method for this ErrorManager object",
               PyObject*,

               std::ignore = node;
               auto list = ((PythonErrorHandler*) context->errorHandler())->pushList();
               Py_INCREF(list);
               return list)

  METHOD(ErrorManager, __exit__, "The context manager 'enter' method for this ErrorManager object",
         void(PyObject*, PyObject*, PyObject*),

         std::ignore = node;
         ((PythonErrorHandler*) context->errorHandler())->popList())
CLASS_END(ErrorManager)

#define DIAGNOSTIC_CLASS(NAME, ERROR, EINFO...) \
  CLASS_BEGIN(NAME) \
    PLAIN_GETTER(NAME, info, "Return the additional data associated with this " #NAME " error", ErrorInfoBundle<chpl::Error##NAME>, return ErrorInfoBundle<chpl::Error##NAME>(node->info())) \
  CLASS_END(NAME)
#include "chpl/framework/error-classes-list.h"
#undef DIAGNOSTIC_CLASS
