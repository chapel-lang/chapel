/*
 * Copyright 2015-2020 Cray Inc.
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

#include <set>
#include <string>
#include <assert.h>

class StringCache {

 private:
   std::set<std::string> storage;

 public:
   StringCache() {};

   void clear() { storage.clear(); }

   unsigned long size() { return (unsigned long) storage.size(); }

   const char *getString(char *str) {
      std::string strstr (str);
      std::set<std::string>::iterator got = storage.find (strstr);
      
      if (got == storage.end()) {
         (void) storage.insert(strstr);
         got = storage.find (strstr);
         if (got == storage.end() ) {
            assert(false);
         }
      }
      
      return got->c_str();
      
      return str;
   }

};
