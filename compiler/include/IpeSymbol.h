#ifndef _IPE_SYMBOL_H_
#define _IPE_SYMBOL_H_

#include "symbol.h"

/******************************** | *********************************
*                                                                   *
* An IpeSymbol is used to reference an IPE runtime variable         *
*                                                                   *
********************************* | ********************************/

class AstVisitor;
class BaseAST;
class Type;

class IpeSymbol : public Symbol
{
public:
                     IpeSymbol(const char* name,
                               Type*       type,
                               int         depth,
                               int         offset);
                    ~IpeSymbol();

  virtual void       verify();
  virtual void       accept(AstVisitor* visitor);

  DECLARE_SYMBOL_COPY(IpeSymbol);

  virtual void       replaceChild(BaseAST* oldAst, BaseAST* newAst);

  virtual bool       isConstant()                             const;
  virtual bool       isConstValWillNotChange()                const;
  virtual bool       isParameter()                            const;

  int                depth()                                  const;
  int                offset()                                 const;

private:
  int                mDepth;
  int                mOffset;
};

#endif
