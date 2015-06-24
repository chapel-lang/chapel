/*
 * Copyright 2004-2015 Cray Inc.
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

#ifndef _IPE_VALUE_H_
#define _IPE_VALUE_H_

class IpeModule;
class IpeProcedure;
class Type;
class VarSymbol;

class IpeValue
{
public:
                IpeValue();
                IpeValue(bool          value);
                IpeValue(long          value);
                IpeValue(double        value);
                IpeValue(const char*   value);
                IpeValue(Type*         value);
                IpeValue(IpeModule*    value);
                IpeValue(IpeProcedure* value);
                IpeValue(IpeValue*     value);

  bool          boolGet()                              const;
  void          boolSet(bool value);

  long          integerGet()                           const;
  void          integerSet(long value);

  double        realGet()                              const;
  void          realSet(double value);

  const char*   cstringGet()                           const;
  void          cstringSet(const char* value);

  Type*         typeGet()                              const;

  IpeModule*    moduleGet()                            const;

  IpeProcedure* procedureGet()                         const;

  IpeValue*     refGet()                               const;

private:

  union ipeValue
  {
    long          iValue;
    double        rValue;

    const char*   sValue;

    Type*         typePtr;

    IpeValue*     valuePtr;
    IpeModule*    modulePtr;
    IpeProcedure* procedurePtr;
  };

  ipeValue    mValue;
};

#endif
