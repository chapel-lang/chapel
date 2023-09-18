

export proc badFormalIntentNone(s: chpl_c_string) {}

export proc badFormalIntentConst(const s: chpl_c_string) {}

export proc badFormalIntentOut(out s: chpl_c_string) {}

export proc badFormalIntentInOut(inout s: chpl_c_string) {}

export proc badFormalIntentRef(ref s: chpl_c_string) {}

export proc badFormalIntentConstRef(const ref s: chpl_c_string) {}
