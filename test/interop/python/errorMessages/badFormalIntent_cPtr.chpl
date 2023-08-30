use CTypes;

export proc badFormalIntentNone(s: c_ptrConst(c_char)) {}

export proc badFormalIntentConst(const s: c_ptrConst(c_char)) {}

export proc badFormalIntentOut(out s: c_ptrConst(c_char)) {}

export proc badFormalIntentInOut(inout s: c_ptrConst(c_char)) {}

export proc badFormalIntentRef(ref s: c_ptrConst(c_char)) {}

export proc badFormalIntentConstRef(const ref s: c_ptrConst(c_char)) {}
