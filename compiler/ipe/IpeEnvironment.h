#ifndef _IPE_ENVIRONMENT_H_
#define _IPE_ENVIRONMENT_H_

#include "IpeValue.h"

#include <vector>

class IpeSymbol;
class VarSymbol;

class IpeEnvironment
{
public:
                     IpeEnvironment(IpeEnvironment*                parent,
                                    const std::vector<IpeSymbol*>& symbols);

                    ~IpeEnvironment();

  IpeValue           lookup(IpeSymbol* variable);
  void               assign(IpeSymbol* variable, IpeValue value);

  void               describe()                                       const;

private:
                     IpeEnvironment();

  IpeEnvironment*    mParent;

  int                mSlotCount;
  IpeSymbol**        mSlotArray;

  void*              mData;
};

#endif
