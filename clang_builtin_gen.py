# This script generates C header that wraps some builtins from clang
# This has been heavily inspired by "include/clang/Basic/Builtins.def" header
# in clang sources (version 5.0), so that it is practically possible to copy
# paste some of builtins provided by clang with little editing effort.

types = {
    "v": "void",
    "b": "boolean",
    "c": "char",
    "s": "short",
    "i": "int",
    "h": "half",
    "f": "float",
    "d": "double",
    "z": "size_t",
    "w": "wchar_t",
}

typePrefix = {
    "X": "_Complex",
    "L": "long",
    "S": "signed",
    "U": "unsigned",
}

typePostfix = {
    "*": "*",
    "C": "const",
    "D": "volatile",
}


def parseType(typeString):
    if len(typeString) == 0:
        return None
    res = []
    i = 0
    while i < len(typeString) and typeString[i] in typePrefix:
        res.append(typePrefix[typeString[i]])
        i += 1

    while i < len(typeString) and typeString[i] in types:
        res.append(types[typeString[i]])
        i += 1

    while i < len(typeString) and typeString[i] in typePostfix:
        res.append(typePostfix[typeString[i]])
        i += 1
    return (typeString[i:], " ".join(res))


def iterTypes(typeString):
    check = parseType(typeString)
    while check:
        rest, cur = check
        yield cur
        check = parseType(rest)


def genBuiltin(functionName, typeString, attrs):
    functionTypes = list(iterTypes(typeString))

    returnType = functionTypes[0]
    argTypes = functionTypes[1:]
    argNames = ["arg" + str(i) for i in range(len(argTypes))]

    args = [argType + " " +
            argName for (argType, argName) in zip(argTypes, argNames)]

    print(
        "static " + returnType + " " + functionName + "(" + ", ".join(args) + ")\n" +
        "{\n" +
        "  return " + "__builtin_" + functionName + "(" + ", ".join(argNames) + ");\n" +
        "}\n"
    )

# Standard libc/libm functions:
genBuiltin("cabs", "dXd", "Fnc")
genBuiltin("cabsf", "fXf", "Fnc")
genBuiltin("cabsl", "LdXLd", "Fnc")
genBuiltin("cacos", "XdXd", "Fnc")
genBuiltin("cacosf", "XfXf", "Fnc")
genBuiltin("cacosh", "XdXd", "Fnc")
genBuiltin("cacoshf", "XfXf", "Fnc")
genBuiltin("cacoshl", "XLdXLd", "Fnc")
genBuiltin("cacosl", "XLdXLd", "Fnc")
genBuiltin("carg", "dXd", "Fnc")
genBuiltin("cargf", "fXf", "Fnc")
genBuiltin("cargl", "LdXLd", "Fnc")
genBuiltin("casin", "XdXd", "Fnc")
genBuiltin("casinf", "XfXf", "Fnc")
genBuiltin("casinh", "XdXd", "Fnc")
genBuiltin("casinhf", "XfXf", "Fnc")
genBuiltin("casinhl", "XLdXLd", "Fnc")
genBuiltin("casinl", "XLdXLd", "Fnc")
genBuiltin("catan", "XdXd", "Fnc")
genBuiltin("catanf", "XfXf", "Fnc")
genBuiltin("catanh", "XdXd", "Fnc")
genBuiltin("catanhf", "XfXf", "Fnc")
genBuiltin("catanhl", "XLdXLd", "Fnc")
genBuiltin("catanl", "XLdXLd", "Fnc")
genBuiltin("ccos", "XdXd", "Fnc")
genBuiltin("ccosf", "XfXf", "Fnc")
genBuiltin("ccosl", "XLdXLd", "Fnc")
genBuiltin("ccosh", "XdXd", "Fnc")
genBuiltin("ccoshf", "XfXf", "Fnc")
genBuiltin("ccoshl", "XLdXLd", "Fnc")
genBuiltin("cexp", "XdXd", "Fnc")
genBuiltin("cexpf", "XfXf", "Fnc")
genBuiltin("cexpl", "XLdXLd", "Fnc")
genBuiltin("cimag", "dXd", "Fnc")
genBuiltin("cimagf", "fXf", "Fnc")
genBuiltin("cimagl", "LdXLd", "Fnc")
genBuiltin("conj", "XdXd", "Fnc")
genBuiltin("conjf", "XfXf", "Fnc")
genBuiltin("conjl", "XLdXLd", "Fnc")
genBuiltin("clog", "XdXd", "Fnc")
genBuiltin("clogf", "XfXf", "Fnc")
genBuiltin("clogl", "XLdXLd", "Fnc")
genBuiltin("cproj", "XdXd", "Fnc")
genBuiltin("cprojf", "XfXf", "Fnc")
genBuiltin("cprojl", "XLdXLd", "Fnc")
genBuiltin("cpow", "XdXdXd", "Fnc")
genBuiltin("cpowf", "XfXfXf", "Fnc")
genBuiltin("cpowl", "XLdXLdXLd", "Fnc")
genBuiltin("creal", "dXd", "Fnc")
genBuiltin("crealf", "fXf", "Fnc")
genBuiltin("creall", "LdXLd", "Fnc")
genBuiltin("csin", "XdXd", "Fnc")
genBuiltin("csinf", "XfXf", "Fnc")
genBuiltin("csinl", "XLdXLd", "Fnc")
genBuiltin("csinh", "XdXd", "Fnc")
genBuiltin("csinhf", "XfXf", "Fnc")
genBuiltin("csinhl", "XLdXLd", "Fnc")
genBuiltin("csqrt", "XdXd", "Fnc")
genBuiltin("csqrtf", "XfXf", "Fnc")
genBuiltin("csqrtl", "XLdXLd", "Fnc")
genBuiltin("ctan", "XdXd", "Fnc")
genBuiltin("ctanf", "XfXf", "Fnc")
genBuiltin("ctanl", "XLdXLd", "Fnc")
genBuiltin("ctanh", "XdXd", "Fnc")
genBuiltin("ctanhf", "XfXf", "Fnc")
genBuiltin("ctanhl", "XLdXLd", "Fnc")
