class CallExpr;
class FnSymbol;
class AggregateType;

void temporaryInitializerFixup(CallExpr* call);

void handleInitializerRules(FnSymbol* fn, AggregateType* t);
