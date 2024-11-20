#include <minrzbas/Parser.hpp>
#include <clang-c/Index.h>

#include <utils/Misc.hpp>

#include <filesystem>
#include <stdexcept>
#include <format>
#include <sstream>
#include <list>
#include <iterator>

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
        CXType _canonType = clang_getCanonicalType(type);
        CXCursor _cursor = clang_getTypeDeclaration(_canonType);
        
        bool _isAnonymous = 
            clang_Cursor_isAnonymousRecordDecl(_cursor) || 
            clang_Cursor_isAnonymous(_cursor)
        ;
        // Returns an empty name if the type is anonymous.
        return _isAnonymous? "" : to_string(clang_getTypeSpelling(_canonType));
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
            }
        }

        switch (clang_getCursorKind(c)) {
        case CXCursor_TypeAliasDecl:
        {
            ptr<Alias> _alias = std::make_shared<Alias>(to_string(c), to_string(clang_getCursorType(c)));
            _scope->inner->aliases.push_back(_alias);
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
        // Fenton::println(*_indent + "< PARENT = " + to_string(clang_getCursorKind(parent)) + ">: " + to_string(c));
        // Fenton::println(*_indent + "< SEMANTIC PARENT = "
        //     + to_string(clang_getCursorKind(clang_getCursorSemanticParent(c)))
        //     + ">: " + to_string(c)
        // );
        // Fenton::println(*_indent + "< LEXICAL PARENT = "
        //     + to_string(clang_getCursorKind(clang_getCursorLexicalParent(c)))
        //     + ">: " + to_string(c)
        // );
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
        _type["isAnonymous"] = _isAnonymous;

        // TODO: Implement it for the memebers.
        // _type["isBitField"] = clang_Cursor_isBitField(c);

        clang_visitChildren(
            c, reflVisitor,
            &_type
        );
    }
    static CXChildVisitResult paramVisitor(CXCursor c, CXCursor parent, CXClientData client_data) {
        if (clang_getCursorKind(c) != CXCursor_ParmDecl)
            return CXChildVisit_Continue;
        
        json::array& _params = *static_cast<json::array*>(client_data);

        _params.emplace_back(json::object{
            { "name", to_string(c) },
            { "type", to_string(clang_getCursorType(c)) }
        });
        return CXChildVisit_Continue;
    }
    // Used when building the "signature" of a destructor from its parameters.
    static CXChildVisitResult signatureParamVisitor(
        CXCursor c, CXCursor parent, CXClientData client_data
    ) {
        if (clang_getCursorKind(c) != CXCursor_ParmDecl)
            return CXChildVisit_Continue;
        
        // Unpacks the parameter list.
        std::list<std::string>& _params = *static_cast<std::list<std::string>*>(client_data);
        // Emplaces the string representation of the parameter's type.
        _params.emplace_back(to_string(clang_getCursorType(c)));

        return CXChildVisit_Continue;
    }
    template<
        bool isMethod = false, bool isCtor = false,
        bool hasReturnType = true, bool mustVisitForParams = false
    >
    static void addOverload(json::object& overloads, CXCursor c) {
        std::string _type;
        // If we must visit for retrieving the parameters, then simply using the 
        // entity's type is not enough for disambiguating it from other overloads.
        // Actually, only destructors have this problem.
        if constexpr (mustVisitForParams) {
            std::list<std::string> _params;
            // Visits the children to retrieve the parameters.
            clang_visitChildren(
                c, signatureParamVisitor,
                // Adds the namespace if it hadn't been already.
                &_params
            );
            _type = "void (";
            if (!_params.empty()) {
                // Not using indexes because it's a list.
                _type.append(*_params.cbegin());
                for (auto it = std::next(_params.cbegin()); it != _params.cend(); ++it) {
                    // Using std::string::append here is fast enough.
                    _type.append(", " + *it);
                }
            }
            _type.append(")");

            // Checks if a noexcept at the end is necessary.
            // NOTE: Does not handle complex noexcept specifiers.
            if (
                clang_getCursorExceptionSpecificationType(c)
                == CXCursor_ExceptionSpecificationKind_BasicNoexcept
            ) {
                _type.append(" noexcept");
            }
        } else {
            _type = to_string(clang_getCursorType(c));
        }

        json::value& _callableVal = overloads[_type];

        // Makes sure variables are not redefined.
        if (!_callableVal.is_object()) {
            json::object& _callable = _callableVal.emplace_object();

            if constexpr (hasReturnType) {
                // Stores the callable's return type.
                _callable["returnType"] = to_string(clang_getCursorResultType(c));
            }
        
            if constexpr (isMethod) {
                json::array& _mods = _callable["methodSpecifiers"].emplace_array();

                if (clang_CXXMethod_isDefaulted(c))              _mods.emplace_back("defaulted");
                if (clang_CXXMethod_isDeleted(c))                _mods.emplace_back("deleted");
                if (clang_CXXMethod_isPureVirtual(c))            _mods.emplace_back("pure");
                if (clang_CXXMethod_isStatic(c))                 _mods.emplace_back("static");
                if (clang_CXXMethod_isVirtual(c))                _mods.emplace_back("virtual");
                if (clang_CXXMethod_isCopyAssignmentOperator(c)) _mods.emplace_back("copy");
                if (clang_CXXMethod_isMoveAssignmentOperator(c)) _mods.emplace_back("move");
                if (clang_CXXMethod_isExplicit(c))               _mods.emplace_back("explicit");
                if (clang_CXXMethod_isConst(c))                  _mods.emplace_back("const");
            }
            if constexpr (isCtor) {
                json::array& _mods = _callable["constructorSpecifiers"].emplace_array();

                if (clang_CXXConstructor_isConvertingConstructor(c)) _mods.emplace_back("converting");
                if (clang_CXXConstructor_isCopyConstructor(c))       _mods.emplace_back("copy");
                if (clang_CXXConstructor_isDefaultConstructor(c))    _mods.emplace_back("default");
                if (clang_CXXConstructor_isMoveConstructor(c))       _mods.emplace_back("move");
            }
            json::array& _params = _callable["parameters"].emplace_array();
            // libclang does not expose the parameters directly through the specific functions, 
            // so an explicit visit is necessary.
            if constexpr (mustVisitForParams) {
                // Visits the children to build the parameter array.
                clang_visitChildren(
                    c, paramVisitor,
                    // Adds the namespace if it hadn't been already.
                    &_params
                );
            } else {
                int _paramCount = clang_Cursor_getNumArguments(c);
                for (int i = 0; i < _paramCount; ++i) {
                    // Gets the cursor corresponding to the argument.
                    CXCursor _argCursor = clang_Cursor_getArgument(c, i);
                    // Stores the argument's name and type.
                    _params.emplace_back(json::object{
                        { "name", to_string(_argCursor) },
                        { "type", to_string(clang_getCursorType(_argCursor)) }
                    });
                }
            }
        }
    }
    template<bool isMethod = false, bool isCtor = false>
    static void addCallable(json::object& callables, CXCursor c, std::string_view name) {
        json::object& _overloads = atOrInsertObject(callables, name);
        addOverload<isMethod>(_overloads, c);
    }
    // Returns true is the type is unsigned and false if the type is not. Uses the type's kind.
    // NOTE: Evaluating std::is_signed<std::underlying_type<type>> with type being the enum's
    // type could theoretically be more assertive, but certainly way slower.
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
        
        // At this point, it is not known whether the declaration is associated with another 
        // entity or not, so we skip it.
        if (
            clang_Cursor_isAnonymousRecordDecl(c) ||
            clang_Cursor_isAnonymous(c)
        ) {
            return CXChildVisit_Continue;
        }

        // Only registers entities when the semantic and lexical parents are the same.
        // This avoids error with out-of-line definitions.
        if (!clang_equalCursors(
            // Semantic parent.
            clang_getCursorSemanticParent(c),
            // Lexical parent.
            parent
        ))
            return CXChildVisit_Continue; 
        
        json::object& _obj = *static_cast<json::object*>(client_data);

        json::object* _nss = nullptr;
        json::object* _types = nullptr;
        json::object* _enums = nullptr;
        json::object* _values = nullptr;
        json::object* _fields = nullptr;
        json::object* _vars = nullptr;
        json::object* _funcs = nullptr;
        json::object* _methods = nullptr;
        json::object* _ctors = nullptr;
        json::object* _dtors = nullptr;

        const std::string _cursorName = to_string(c);

        switch(clang_getCursorKind(c)) {
            // Namespace.
            case CXCursor_Namespace: {
                if (!_nss)
                    _nss = &atOrInsertObject(_obj, "namespaces");

                // Visits the children recursively.
                clang_visitChildren(
                    c, reflVisitor,
                    // Adds the namespace if it hadn't been already.
                    &atOrInsertObject(*_nss, _cursorName)
                );
                break;
            }
            // Type declarations.
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
                
                _enum["isScoped"] = static_cast<bool>(clang_EnumDecl_isScoped(c));

                CXType _enumType = clang_getCanonicalType(clang_getEnumDeclIntegerType(c));
                
                _enum["type"] = to_string(_enumType);

                if (isTypeUnsigned(_enumType)) {
                    clang_visitChildren(c, visitEnum<true>, &_enum["values"].emplace_object());
                } else {
                    clang_visitChildren(c, visitEnum<false>, &_enum["values"].emplace_object());
                }
                break;
            }
            // Variables and fields.
            case CXCursor_FieldDecl: {
                if (!_fields)
                    _fields = &atOrInsertObject(_obj, "fields");
                
                json::value& _fieldVal = (*_fields)[_cursorName];

                // Makes sure the field is not redefined.
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

                // Makes sure the variable is not redefined.
                if (!_varVal.is_object()) {
                    json::object& _var = _varVal.emplace_object();
                    _var["type"] = to_string(clang_getCursorType(c));
                }
                break;
            }
            // Callables.
            case CXCursor_FunctionDecl: {
                if (!_funcs)
                    _funcs = &atOrInsertObject(_obj, "functions");
                
                // It's not a method.
                addCallable(*_funcs, c, _cursorName);
                break;
            }
            case CXCursor_CXXMethod: {
                if (!_methods)
                    _methods = &atOrInsertObject(_obj, "methods");
                
                // It's a method.
                addCallable<true>(*_methods, c, _cursorName);
                break;
            }
            case CXCursor_Constructor: {
                if (!_ctors)
                    _ctors = &atOrInsertObject(_obj, "constructors");
                
                addOverload<
                    // May have method and/or constructor modifiers.
                    true, true,
                    // Technically has no return type.
                    true
                >(*_ctors, c);
                break;
            }
            case CXCursor_Destructor: {
                if (!_dtors)
                    _dtors = &atOrInsertObject(_obj, "destructors");
                
                addOverload<
                    // May have method modifiers.
                    true,
                    // Is not a constructor.
                    false,
                    // Always has the same return type (void), so need not have it as a field.
                    false,
                    // An explicit visit is needed to retrieve a destructor's parameters.
                    true
                >(*_dtors, c);
                break;
            }
            // Aliases.
            case CXCursor_TypeAliasDecl: {
                break;
            }
            case CXCursor_TypedefDecl: {
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

        return std::move(_rootObj);
    }
}