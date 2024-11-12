#include <minrzbas/Parser.hpp>
#include <clang-c/Index.h>

#include <utils/Misc.hpp>

#include <filesystem>
#include <stdexcept>
#include <format>

namespace json = boost::json;
namespace fs = std::filesystem;

namespace Fenton::Minrzbas {
    struct ReflVisitData {

    };

    // Converts a clang error code to a string.
    std::string to_string(const CXErrorCode& v) {
        switch (v) {
            /**
            * Zero (\c CXError_Success) is the only error code indicating success.  Other
            * error codes, including not yet assigned non-zero values, indicate errors.
            */
        case CXError_Success: return "CXError_Success";

            /**
             * A generic error code, no further details are available.
             *
             * Errors of this kind can get their own specific error codes in future
             * libclang versions.
             */
        case CXError_Failure: return "CXError_Failure";

            /**
             * libclang crashed while performing the requested operation.
             */
        case CXError_Crashed: return "CXError_Crashed";

            /**
             * The function detected that the arguments violate the function
             * contract.
             */
        case CXError_InvalidArguments: return "CXError_InvalidArguments";

            /**
             * An AST deserialization error has occurred.
             */
        case CXError_ASTReadError: return "CXError_ASTReadError";
        default: return "UNKNOWN";
        }
    }
    std::ostream& operator<<(std::ostream& stream, const CXString& str)
    {
        stream << clang_getCString(str);
        clang_disposeString(str);
        return stream;
    }
    std::string to_string(const CXString& cxstr) {
        std::string _str = clang_getCString(cxstr);
        clang_disposeString(cxstr);
        return _str;
    }
    std::string to_string(const CXCursor& cursor) {
        return to_string(clang_getCursorSpelling(cursor));
    }
    std::string to_string(const CXType& type) {
        return to_string(clang_getTypeSpelling(clang_getCanonicalType(type)));
        //return to_string(/*clang_getTypeSpelling(clang_getCanonicalType(type))*/clang_getTypeSpelling(type));
    }
    std::string to_string(const CXCursorKind& kind) {
        return to_string(clang_getCursorKindSpelling(kind));
    }
    std::string to_string(bool v) {
        return v ? "true" : "false";
    }

#if 0
    static CXChildVisitResult reflVisitor(CXCursor c, CXCursor parent, CXClientData client_data) {
        if (!clang_Location_isFromMainFile(clang_getCursorLocation(c)))
            return CXChildVisit_Continue;

        ReflVisitData* _scope = static_cast<ReflVisitData*>(client_data);
        std::string _cursorName = to_string(c);

        // Print the cursor.

        switch (clang_getCursorKind(c)) {
        case CXCursor_TypeAliasDecl:
        case CXCursor_MacroExpansion:
            std::cout << std::string(_scope->indent * 2, ' ')
                << to_string(c) << "->" << to_string(clang_getCursorKind(c)) << std::endl;
        }
        // Independent of the scope's type.
        switch (clang_getCursorKind(c)) {
        case CXCursor_ClassDecl:
            addClass(*_scope, c, _cursorName, false);
            break;
        case CXCursor_StructDecl:
            addClass(*_scope, c, _cursorName, true);
            break;
        }
        // If the scope is a namespace.
        if (_scope->isNS) {
            ptr<Namespace> _paren = std::static_pointer_cast<Namespace>(_scope->inner);

            switch (clang_getCursorKind(c)) {
            case CXCursor_VarDecl:
            {
                addVariable<ptr<Variable>>(*_scope, c, _cursorName, _paren->vars, _paren->varMap, true,
                    [](ReflVisitData& visitData, const CXCursor& c, const std::string& cursorName,bool)->ptr<Variable> {
                    return std::make_shared<Variable>(cursorName, to_string(clang_getCursorType(c)));
                });
                break;
            }
            case CXCursor_FunctionDecl:
            {
                addFunction<ptr<Function>>(*_scope, c, _cursorName, _paren->funcs, _paren->funcMap,
                    [](ReflVisitData& visitData, const CXCursor& c, 
                        const std::string& cursorName, const std::string& cursorType)->ptr<Function> {
                    return std::make_shared<Function>(cursorName, cursorType, getParams(c));
                });
                break;
            }
            case CXCursor_Namespace:
            {
                ptr<Namespace> _ns;
                auto it = _paren->nsMap.find(_cursorName);
                if (it == _paren->nsMap.end()) {
                    _ns = std::make_shared<Namespace>(_cursorName);
                    _paren->namespaces.push_back(_ns);
                    _paren->nsMap[_cursorName] = _ns;
                }
                else {
                    _ns = it->second;
                }
                // It's a namespace, so there are no access modifiers.
                ReflVisitData _newScope(_scope->unit, true, _ns, Access::None, _scope->indent + 1);
                clang_visitChildren(c, reflVisitor, &_newScope);
                break;
            }
            }
        }
        // If the scope is a class.
        else {
            ptr<Class> _paren = std::static_pointer_cast<Class>(_scope->inner);
            bool _isStatic = false;

            switch (clang_getCursorKind(c)) {
            case CXCursor_CXXBaseSpecifier:
            {
                Access _acc = Access::None;
                switch (clang_getCXXAccessSpecifier(c)) {
                case CX_CXXPrivate:
                    _acc = Access::Private;
                    break;
                case CX_CXXProtected:
                    _acc = Access::Protected;
                    break;
                case CX_CXXPublic:
                    _acc = Access::Public;
                    break;
                }
                // TODO: Get the virtual specifier.
                ptr<Base> _base = std::make_shared<Base>(to_string(c), _acc, clang_isVirtualBase(c));
                _paren->bases.push_back(_base);

                /*clang_visitChildren(c, [](CXCursor c, CXCursor parent, CXClientData client_data) {
                    std::cout << "PARENT: " << to_string(parent) << "; " << to_string(c)
                        << "->" << to_string(clang_getCursorKind(c)) << std::endl;
                    return CXChildVisit_Recurse;
                }, nullptr);*/

                break;
            }
            case CXCursor_CXXAccessSpecifier:
            {
                switch (clang_getCXXAccessSpecifier(c)) {
                case CX_CXXPrivate:
                    _scope->currAcc = Access::Private;
                    break;
                case CX_CXXProtected:
                    _scope->currAcc = Access::Protected;
                    break;
                case CX_CXXPublic:
                    _scope->currAcc = Access::Public;
                    break;
                }
                break;
            }
            case CXCursor_VarDecl:
                // The only difference is that this one is static.
                _isStatic = true;
                [[fallthrough]];
            case CXCursor_FieldDecl:
            {
                addVariable<ptr<Field>>(*_scope, c, _cursorName, _paren->fields, _paren->fieldMap, _isStatic,
                    [](ReflVisitData& visitData, const CXCursor& c, const std::string& cursorName, bool isStatic)->ptr<Field> {
                    return std::make_shared<Field>(
                        cursorName,
                        to_string(clang_getCursorType(c)),
                        // Current access.
                        visitData.currAcc, 
                        // Non-static.
                        isStatic);
                });
                break;
            }
            case CXCursor_CXXMethod:
            {
                addFunction<ptr<Method>>(*_scope, c, _cursorName, _paren->methods, _paren->methodMap,
                    [](ReflVisitData& visitData, const CXCursor& c, 
                        const std::string& cursorName, const std::string& cursorType)->ptr<Method> {
                        return std::make_shared<Method>(
                            cursorName, cursorType, getParams(c), visitData.currAcc,
                            clang_CXXMethod_isStatic(c), clang_CXXMethod_isVirtual(c), clang_CXXMethod_isConst(c)
                        );
                });
                break;
            }
            }
        }

        switch (clang_getCursorKind(c)) {
        case CXCursor_TypeAliasDecl:
        {
            ptr<Alias> _alias = std::make_shared<Alias>(to_string(c), to_string(clang_getCursorType(c)));
            _scope->inner->aliases.push_back(_alias);
            break;
        }
        case CXCursor_MacroExpansion:
        {
            std::cout << std::endl << "#### " << to_string(c) << " ####" << std::endl;
            tokeniseMacro(_scope->unit, c);
            std::cout << "####" << std::endl;
            break;
        }
        }

        return CXChildVisit_Continue;
    }
#endif
    static CXChildVisitResult reflVisitor(CXCursor c, CXCursor parent, CXClientData client_data);

    static json::object& atOrInsertObject(json::object& obj, std::string_view key) {
        if (json::value* vPtr = obj.if_contains(key)) {
            return vPtr->as_object();
        } else {
            return (obj[key] = json::object{}).get_object();
        }
    }

    static CXChildVisitResult debugReflVisitor(CXCursor c, CXCursor parent, CXClientData client_data) {
        if (
            clang_Location_isInSystemHeader(clang_getCursorLocation(c))
            || (!clang_Location_isFromMainFile(clang_getCursorLocation(c))
            && clang_getCursorKind(c) != CXCursor_InclusionDirective)
        )
            return CXChildVisit_Continue;

        std::string* _indent = static_cast<std::string*>(client_data);

        // Prints the cursor's name.
        Fenton::println(*_indent + "<" + to_string(clang_getCursorKind(c)) + ">: " + to_string(c));
        if (clang_getCursorKind(c) == CXCursor_InclusionDirective) {
            // Prints the included file.
            Fenton::println(
                *_indent + "    "
                + quote(to_string(clang_getFileName(clang_getIncludedFile(c))))
            );
        }
        // if (clang_Cursor_hasAttrs(c)) {
        //     // Prints if it has attributes.
        //     Fenton::println(*_indent + "    " + "[HAS ATTRIBUTES]");
        // }

        // Indent.
        _indent->append(4, ' ');
        // Visits the children recursively.
        clang_visitChildren(c, debugReflVisitor, client_data);
        // De-indent.
        _indent->resize(_indent->size()-4);

        return CXChildVisit_Continue;
    }
    template<bool isClass, bool isStruct, bool isUnion> static void addRecord(
        json::object& obj, json::object*& typesPtr, CXCursor c, std::string_view name
    ) {
        if (!typesPtr)
            typesPtr = &atOrInsertObject(obj, "types");
        
        bool _isAnonymous = clang_Cursor_isAnonymousRecordDecl(c);
        bool _isDef = clang_isCursorDefinition(c);
        
        json::object& _type = atOrInsertObject(*typesPtr, _isAnonymous? "" : name);

        json::value& _isDefVal = _type["isDefined"];
        if (_isDef) {
            if (bool* _isDefPtr = _isDefVal.if_bool()) {
                if (*_isDefPtr) {
                    // Prevents redefining.
                    return;
                } else {
                    *_isDefPtr = true;
                }
            } else {
                _isDefVal = true;
            }
        } else {
            // Prevents undefining if already defined.
            if (!_isDefVal.is_bool())
                _isDefVal = false;
            // It's not a definition, so we skip it.
            return;
        }

        _type["isClass"] = isClass;
        _type["isStruct"] = isStruct;
        _type["isUnion"] = isUnion;
        _type["isAnonoymous"] = _isAnonymous;

        // TODO: Implement it for the memebers.
        // _type["isBitField"] = clang_Cursor_isBitField(c);

        clang_visitChildren(
            c, reflVisitor,
            &_type
        );
    }
    static bool isTypeUnsigned(CXType type) {
        switch (type.kind) {
            case CXType_UChar:
            case CXType_UShort:
            case CXType_UInt:
            case CXType_ULong:
            case CXType_ULongLong:
            case CXType_UInt128:
            case CXType_UShortAccum:
            case CXType_UAccum:
            case CXType_ULongAccum:
                return true;
            default:
                return false;
        }
    }
    template<bool isUnsigned>
    static CXChildVisitResult visitEnum(CXCursor c, CXCursor parent, CXClientData client_data) {
        // Only consider enum elements.
        if(clang_getCursorKind(c) != CXCursor_EnumConstantDecl)
            return CXChildVisit_Continue;

        json::object& _values = *static_cast<json::object*>(client_data);
        const std::string _cursorName = to_string(c);

        if constexpr (isUnsigned) {
            _values[_cursorName] = clang_getEnumConstantDeclUnsignedValue(c);
        } else {
            _values[_cursorName] = clang_getEnumConstantDeclValue(c);
        }
        return CXChildVisit_Continue;
    }
    static CXChildVisitResult reflVisitor(CXCursor c, CXCursor parent, CXClientData client_data) {
        if (!clang_Location_isFromMainFile(clang_getCursorLocation(c)))
            return CXChildVisit_Continue;
        
        // An anonymous declaration cannot be referenced elsewhere, so we skip it.
        if (clang_Cursor_isAnonymousRecordDecl(c)) {
            return CXChildVisit_Continue;
        }
        
        json::object& _obj = *static_cast<json::object*>(client_data);

        json::object* _nss = nullptr;
        json::object* _types = nullptr;
        json::object* _enums = nullptr;
        json::object* _fields = nullptr;
        json::object* _vars = nullptr;
        json::object* _values = nullptr;
        
        const std::string _cursorName = to_string(c);

        switch(clang_getCursorKind(c)) {
            case CXCursor_Namespace:
                if (!_nss)
                    _nss = &atOrInsertObject(_obj, "namespaces");

                // Visits the children recursively.
                clang_visitChildren(
                    c, reflVisitor,
                    // Adds the namespace if it hadn't been already.
                    &atOrInsertObject(*_nss, _cursorName)
                );
                break;
            case CXCursor_ClassDecl:
                addRecord<true, false, false>(_obj, _types, c, _cursorName);
                break;
            case CXCursor_StructDecl:
                addRecord<false, true, false>(_obj, _types, c, _cursorName);
                break;
            case CXCursor_UnionDecl:
                addRecord<false, false, true>(_obj, _types, c, _cursorName);
                break;
            case CXCursor_EnumDecl: {
                if (!_enums)
                    _enums = &atOrInsertObject(_obj, "enums");
                
                json::object& _enum = atOrInsertObject(*_enums, _cursorName);
                json::value& _isDefVal = _enum["isDefined"];
                bool* _isAlreadyDef = _isDefVal.if_bool();
                bool _isDef = clang_isCursorDefinition(c);

                if (_isDef) {
                    if (bool* _isDefPtr = _isDefVal.if_bool()) {
                        if (*_isDefPtr) {
                            // Prevents redefining.
                            break;
                        } else {
                            *_isDefPtr = true;
                        }
                    } else {
                        _isDefVal = true;
                    }
                } else {
                    // Prevents undefining if already defined.
                    if (!_isDefVal.is_bool())
                        _isDefVal = false;
                    // It's not a definition, so we skip it.
                    break;
                }
                
                _enum["isScoped"] = clang_EnumDecl_isScoped(c);

                CXType _enumType = clang_getEnumDeclIntegerType(c);
                _enum["underlyingType"] = to_string(_enumType);

                if (isTypeUnsigned(_enumType)) {
                    clang_visitChildren(c, visitEnum<false>, &_enum["values"].emplace_object());
                } else {
                    clang_visitChildren(c, visitEnum<false>, &_enum["values"].emplace_object());
                }
                break;
            }
            case CXCursor_FieldDecl: {
                if (!_fields)
                    _fields = &atOrInsertObject(_obj, "fields");
                
                json::value& _fieldVal = (*_fields)[_cursorName];

                // Makes sure fields are not redefined.
                if (!_fieldVal.is_object()) {
                    json::object& _field = _fieldVal.emplace_object();
                    _field["type"] = to_string(clang_getCursorType(c));
                }
                break;
            }
            case CXCursor_VarDecl: {
                if (!_vars)
                    _vars = &atOrInsertObject(_obj, "variables");
                
                json::value& _varVal = (*_vars)[_cursorName];

                // Makes sure variables are not redefined.
                if (!_varVal.is_object()) {
                    json::object& _var = _varVal.emplace_object();
                    _var["type"] = to_string(clang_getCursorType(c));
                }
                break;
            }
        }
        return CXChildVisit_Continue;
    }

    boost::json::object unitToJSON(
        const std::string& filePath,
        const std::vector<std::string>& args
    ) {
        if (!fs::exists(filePath)) {
            throw std::runtime_error(std::format(
                "The file {0} could not be found.", quote(filePath)
            ));
        }

        std::vector<const char*> _args {
            "-std=c++23"
        };
        // Prepares the arguments.
        for (auto& a : args) {
            _args.emplace_back(a.c_str());
        }
        CXIndex index = clang_createIndex(
            0,
            // Does display diagnostics.
            true
        );
        CXTranslationUnit unit;
        CXErrorCode _errorCode = clang_parseTranslationUnit2(
            index,
            filePath.c_str(), _args.data(), static_cast<int>(_args.size()),
            nullptr, 0,
            CXTranslationUnit_DetailedPreprocessingRecord
            | CXTranslationUnit_SkipFunctionBodies
            | CXTranslationUnit_KeepGoing,
            &unit
        );
        // If the error code is not 0.
        if (_errorCode)
        {
            throw std::runtime_error(std::format(
                "Unable to parse translation unit. Error: {0}", 
                to_string(_errorCode)
            ));
        }

        CXCursor cursor = clang_getTranslationUnitCursor(unit);

        std::string _indent;
        // DEBUG - Visits the children recursively.
        clang_visitChildren(
            cursor,
            debugReflVisitor,
            &_indent
        );

        json::object _rootObj;
        // Visits the children, building the JSON AST.
        clang_visitChildren(
            cursor,
            // reflVisitor,
            reflVisitor,
            &_rootObj
        );

        clang_disposeTranslationUnit(unit);
        clang_disposeIndex(index);

        return _rootObj;
    }
}