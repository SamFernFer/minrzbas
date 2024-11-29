#include <minrzbas/Parser.hpp>
#include <clang-c/Index.h>

#include <utils/Misc.hpp>

#include <filesystem>
#include <stdexcept>
#include <format>
#include <sstream>
#include <list>
#include <iterator>
#include <set>

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

    // Returns the string representation of the canonical version of the type.
    static std::string typeToString(CXType type) {
        CXCursor _decl = clang_getTypeDeclaration(type);

        // Returns an empty string if the type is anonymous.
        if (clang_Cursor_isAnonymousRecordDecl(_decl) || 
            clang_Cursor_isAnonymous(_decl)) {
            return "";
        }

        // Returns the canonical spelling of the type.
        return to_string(clang_getTypeSpelling(clang_getCanonicalType(type)));
    }
    std::string to_string(const CXCursorKind& kind) {
        return to_string(clang_getCursorKindSpelling(kind));
    }
    std::string to_string(bool v) {
        return v ? "true" : "false";
    }
    static CXChildVisitResult reflVisitor(CXCursor c, CXCursor parent, CXClientData client_data);
    // Adds the "type" and "canonicalType" fields with the visual and canonical type spellings 
    // respectively.
    static void addViewAndCanonicalType(json::object& obj, CXType type) {
        obj["type"] = typeToString(type);
        // obj["canonicalType"] = typeToString<true>(type);
    }
    // Adds the "type" and "canonicalType" fields with the visual and canonical type spellings 
    // of the cursor's type, respectively.
    static void addViewAndCanonicalType(json::object& obj, CXCursor c) {
        addViewAndCanonicalType(obj, clang_getCursorType(c));
    }
    // Adds the "access" field containing the entity's access level if it is specified, 
    // else does nothing.
    static void addAccess(json::object& obj, CXCursor c) {
        switch (clang_getCXXAccessSpecifier(c)) {
            case CX_CXXPublic:
                obj["access"] = "public";
                break;
            case CX_CXXProtected:
                obj["access"] = "protected";
                break;
            case CX_CXXPrivate:
                obj["access"] = "private";
                break;
            default:
                break;
        }
    }
    static json::object& atOrInsertObject(json::object& obj, std::string_view key) {
        if (json::value* vPtr = obj.if_contains(key)) {
            return vPtr->as_object();
        } else {
            return (obj[key] = json::object{}).get_object();
        }
    }

    static CXChildVisitResult debugReflVisitor(
        CXCursor c, CXCursor parent, CXClientData client_data
    ) {
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
            CXFile _file = clang_getIncludedFile(c);

            if (_file) {
                // Prints the included file.
                Fenton::println(*_indent + "    " + quote(to_string(clang_getFileName(_file))));
            } else {
                Fenton::println(*_indent + "    " + to_string(c));
            }
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
    // If the parent parameter has an expression inside it, then it has a default value 
    // and the "hasDefaultValue" is added to the JSON object sent as client_data.
    static CXChildVisitResult defaultParamValueVisitor(
        CXCursor c, CXCursor parent, CXClientData client_data
    ) {
        CXCursorKind _kind = clang_getCursorKind(c);
        // If the cursor is an expression, sets the "hasDefaultValue" to true and 
        // stops the visit.
        if (
            _kind >= CXCursor_FirstExpr
            && _kind <= CXCursor_LastExpr
        ) {
            json::object& _param = *static_cast<json::object*>(client_data);
            _param["hasDefaultValue"] = true;
            return CXChildVisit_Break;
        }
        return CXChildVisit_Continue;
    }
    // Creates a JSON object from the parameter represented by the cursor.
    static json::object getParamFromCursor(CXCursor c) {
        json::object _param = json::object{
            { "name", to_string(c) }
        };
        // Adds the type to the parameter.
        addViewAndCanonicalType(_param, c);
        // If the parameter has an expression inside it, then it has a default value 
        // and the "hasDefaultValue" will be added.
        clang_visitChildren(c, defaultParamValueVisitor, &_param);

        return std::move(_param);
    }
    static CXChildVisitResult paramVisitor(CXCursor c, CXCursor parent, CXClientData client_data) {
        if (clang_getCursorKind(c) != CXCursor_ParmDecl)
            return CXChildVisit_Continue;
        
        json::array& _params = *static_cast<json::array*>(client_data);
        
        // Moves the parameter into the array.
        _params.emplace_back(getParamFromCursor(c));
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
        _params.emplace_back(typeToString(clang_getCursorType(c)));

        return CXChildVisit_Continue;
    }
    static CXChildVisitResult methodAttrsVisitor(
        CXCursor c, CXCursor parent, CXClientData client_data
    ) {
        json::array& _attrs = *static_cast<json::array*>(client_data);
        switch (clang_getCursorKind(c)) {
            case CXCursor_CXXOverrideAttr:
                _attrs.emplace_back("override");
                break;
            case CXCursor_CXXFinalAttr:
                _attrs.emplace_back("final");
                break;
        }
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
            clang_visitChildren(c, signatureParamVisitor, &_params);
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
            _type = typeToString(clang_getCursorType(c));
        }

        json::value& _callableVal = overloads[_type];

        // Makes sure functions are not redefined.
        if (!_callableVal.is_object()) {
            json::object& _callable = _callableVal.emplace_object();

            // Adds the callable's access level.
            addAccess(_callable, c);

            if constexpr (hasReturnType) {
                // Stores the callable's return type.
                _callable["returnType"] = typeToString(clang_getCursorResultType(c));
                // _callable["canonicalReturnType"] = typeToString<true>(clang_getCursorResultType(c));
            }
        
            if constexpr (isMethod) {
                json::array& _specs = _callable["methodSpecifiers"].emplace_array();

                if (clang_CXXMethod_isDefaulted(c))              _specs.emplace_back("defaulted");
                if (clang_CXXMethod_isDeleted(c))                _specs.emplace_back("deleted");
                if (clang_CXXMethod_isPureVirtual(c))            _specs.emplace_back("pure");
                if (clang_CXXMethod_isStatic(c))                 _specs.emplace_back("static");
                if (clang_CXXMethod_isVirtual(c))                _specs.emplace_back("virtual");
                if (clang_CXXMethod_isExplicit(c))               _specs.emplace_back("explicit");
                if (clang_CXXMethod_isConst(c))                  _specs.emplace_back("const");

                // NOTE: Those are technically not specifiers, but would take too much space on 
                // their own.

                if (clang_CXXMethod_isCopyAssignmentOperator(c)) _specs.emplace_back("copy");
                if (clang_CXXMethod_isMoveAssignmentOperator(c)) _specs.emplace_back("move");

                // Retrieves the override and final attributes, if present.
                if (clang_Cursor_hasAttrs(c)) {
                    // Visits the cursor to retrieve the attributes.
                    clang_visitChildren(c, methodAttrsVisitor, &_specs);
                }
            }
            if constexpr (isCtor) {
                json::string& _ctorType = _callable["constructorType"].emplace_string();

                if (clang_CXXConstructor_isCopyConstructor(c))
                    _ctorType = "copy";
                else if (clang_CXXConstructor_isMoveConstructor(c))
                    _ctorType = "move";
                // libclang consinders copy and move constructors as converting constructors 
                // so this case must go after them.
                else if (clang_CXXConstructor_isConvertingConstructor(c))
                    _ctorType = "converting";
                else if (clang_CXXConstructor_isDefaultConstructor(c))
                    _ctorType = "default";
                else
                    _ctorType = "general";
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
                    // Moves the parameter into the array.
                    _params.emplace_back(getParamFromCursor(_argCursor));
                }
            }
        }
    }
    template<bool isMethod = false, bool isCtor = false>
    static void addCallable(json::object& callables, CXCursor c, std::string_view name) {
        json::object& _overloads = atOrInsertObject(callables, name);
        addOverload<isMethod>(_overloads, c);
    }
    template<RecordType recordType> static void addRecord(
        json::object& obj, json::object*& typesPtr, CXCursor c, std::string_view name
    ) {
        if (!typesPtr)
            typesPtr = &atOrInsertObject(obj, "types");
        
        bool _isAnonymous = clang_Cursor_isAnonymousRecordDecl(c);
        
        json::object& _type = atOrInsertObject(*typesPtr, _isAnonymous? "" : name);

        {
            json::value& _isDefined = _type["isDefined"];
            // Prevents redefining.
            if (_isDefined.is_bool() && _isDefined.get_bool()) {
                return;
            } else {
                // Registers whether this is a definition and signs to break if it is not.
                if (!(_isDefined.emplace_bool() = clang_isCursorDefinition(c)))
                    return;
            }
        }
        _type["isAnonymous"] = _isAnonymous;

        // Adds the record's access level.
        addAccess(_type, c);
        // Adds the record's type.
        json::string& _recordType = _type["recordType"].emplace_string();

        switch (recordType) {
            case RecordType::Class:
                _recordType = "class";
                break;
            case RecordType::Struct:
                _recordType = "struct";
                break;
            case RecordType::Union:
                _recordType = "union";
                break;
        }

        // TODO: Implement it for the members.
        // _type["isBitField"] = clang_Cursor_isBitField(c);

        clang_visitChildren(
            c, reflVisitor,
            &_type
        );
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
        
        const CXCursorKind _kind = clang_getCursorKind(c);
        
        // At this point, it is not known whether the declaration is associated with another 
        // entity or not, so we skip it.
        if (
            // Anonymous namespaces are easy to reference.
            _kind != CXCursor_Namespace &&
            (clang_Cursor_isAnonymousRecordDecl(c) ||
            clang_Cursor_isAnonymous(c))
        ) {
            return CXChildVisit_Continue;
        }

        json::object& _obj = *static_cast<json::object*>(client_data);
        const std::string _cursorName = to_string(c);

        // If the cursor is a base class specifier.
        if (_kind == CXCursor_CXXBaseSpecifier) {
            // Makes sure there is a "bases" array.
            json::array& _bases = [&_obj]()->json::array&{
                json::value& _v = _obj["bases"];
                return _v.is_array()? _v.get_array() : _v.emplace_array();
            }();
            json::object _base;
            // Adds the base's type.
            addViewAndCanonicalType(_base, c);
            // Adds the base's access.
            addAccess(_base, c);
            // Adds the base.
            _bases.emplace_back(_base);
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

        switch(_kind) {
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
                addRecord<RecordType::Class>(_obj, _types, c, _cursorName);
                break;
            case CXCursor_StructDecl:
                addRecord<RecordType::Struct>(_obj, _types, c, _cursorName);
                break;
            case CXCursor_UnionDecl:
                addRecord<RecordType::Union>(_obj, _types, c, _cursorName);
                break;
            case CXCursor_EnumDecl: {
                if (!_enums)
                    _enums = &atOrInsertObject(_obj, "enums");
                
                json::object& _enum = atOrInsertObject(*_enums, _cursorName);

                bool _shouldBreak = false;
                {
                    json::value& _isDefined = _enum["isDefined"];
                    // Prevents redefining.
                    if (_isDefined.is_bool() && _isDefined.get_bool()) {
                        _shouldBreak = true;
                    } else {
                        // Registers whether this is a definition and signs to break if it is not.
                        if (!(_isDefined.emplace_bool() = clang_isCursorDefinition(c)))
                            _shouldBreak = true;
                    }
                }
                // Registers whether the enum is scoped or not.
                {
                    json::value& _isScoped = _enum["isScoped"];
                    // If it wasn't registered already, registers whether the enum is scoped 
                    // or not.
                    if (!_isScoped.is_bool())
                        _isScoped.emplace_bool() = static_cast<bool>(clang_EnumDecl_isScoped(c));
                }
                if (_shouldBreak)
                    break;

                CXType _enumType = clang_getCanonicalType(clang_getEnumDeclIntegerType(c));
                
                // Adds the enum's underlying type.
                addViewAndCanonicalType(_enum, _enumType);

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
                    // Adds the field's access level.
                    addAccess(_field, c);
                    // Adds the field's type.
                    addViewAndCanonicalType(_field, c);
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
                    // Adds the variable's access level.
                    addAccess(_var, c);
                    // Adds the variable's type.
                    addViewAndCanonicalType(_var, c);
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
                    false
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
    // Returns true if path is a subpath of base, meaning path is inside the base directory.
    static bool is_subpath(const fs::path& path, const fs::path& base) {
        const auto mismatch_pair = std::mismatch(
            path.begin(), path.end(), base.begin(), base.end()
        );
        return mismatch_pair.second == base.end();
    }
    // Visitor function for retrieving all dependencies.
    static void inclusionVisitor(
        CXFile included_file,
        CXSourceLocation *inclusion_stack,
        unsigned include_len,
        CXClientData client_data
    ) {
        // Ignores invalid inclusion directives.
        if (!included_file)
            return;

        // The pair is (inputIncludeDirs, outputIncludeDirs).
        auto& _incData = *static_cast<
            std::pair<const std::vector<std::string>*, std::set<json::string>>*
        >(client_data);

        if (include_len > 0) {
            CXSourceLocation _loc = inclusion_stack[0];
            // Ignore anything included by a system header.
            if (clang_Location_isInSystemHeader(_loc))
                return;
        } else {
            return;
        }

        // Gets the file's name, allocating a new CXString.
        CXString _cxstr = clang_getFileName(included_file);
        // Constructs an std::filesystem::path from the CXString so that we can dispose it.
        fs::path _filePath = clang_getCString(_cxstr);
        // Disposes the CXString to prevent a memory leak.
        clang_disposeString(_cxstr);

        // Iterates over the non-system include directories and checks whether.
        bool _isSubpath = false;
        for (const std::string& d : *_incData.first) {
            if (is_subpath(_filePath, d.c_str())) {
                _isSubpath = true;
                break;
            }
        }
        if (!_isSubpath) {
            return;
        }

        // Inserts the file's name in POSIX format while automatically rejecting duplicates.
        _incData.second.emplace(_filePath.generic_string());
        // Disposes the CXString.
        clang_disposeString(_cxstr);
    }

    boost::json::object unitToJSON(
        const std::string& filePath,
        const std::vector<std::string>& includeDirs,
        const std::vector<const char*>& args,
        bool dumpAST
    ) {
        if (!fs::exists(filePath)) {
            throw std::runtime_error(std::format(
                "The file {0} could not be found.", quote(filePath)
            ));
        }
        CXIndex index = clang_createIndex(
            0,
            // Does display diagnostics.
            true
        );
        CXTranslationUnit unit;
        CXErrorCode _errorCode = clang_parseTranslationUnit2(
            index,
            filePath.c_str(), args.data(), static_cast<int>(args.size()),
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

        if (dumpAST) {
            std::string _indent;
            // DEBUG - Visits the children recursively.
            clang_visitChildren(
                cursor,
                debugReflVisitor,
                &_indent
            );
        }

        std::pair<const std::vector<std::string>*, std::set<json::string>> _incData = {
            // Passing as a pointer to prevent copying. Moving the vector could make 
            // displying information for the tests more troublesome later.
            &includeDirs,
            {}
        };
        // Visits the children and builds the list of included files.
        clang_getInclusions(unit, inclusionVisitor, &_incData);

        json::object _rootObj;
        // Visits the children, building the JSON AST.
        clang_visitChildren(
            cursor,
            // reflVisitor,
            reflVisitor,
            &_rootObj
        );
        json::array _incs;
        // Resizes the array once.
        _incs.resize(_incData.second.size());
        {
            // The iterator to be beginning of the JSON array.
            auto _incsIt = _incs.begin();
            // Moves each element from the set into the array.
            for (auto it = _incData.second.begin(); it != _incData.second.end(); ++it, ++_incsIt) {
                *_incsIt = std::move(*it);
            }
        }
        // Moves the JSON array of inclusion directives into the JSON AST.
        _rootObj["inclusions"] = std::move(_incs);

        clang_disposeTranslationUnit(unit);
        clang_disposeIndex(index);

        return std::move(_rootObj);
    }
    OptionsDecomposition decomposeOptions(const boost::program_options::variables_map& vm) {
        // The input path.
        std::string _input = vm["input"].as<std::string>();
        // The optional output path.
        std::string _output = [&vm]()->std::string{
            auto _find = vm.find("output");
            return _find != vm.end()?
                _find->second.as<std::string>() : std::string{}
            ;
        }();
        // Compilation definitions.
        std::vector<std::string> _defs = [&vm]()->std::vector<std::string>{
            auto _find = vm.find("define");
            return _find != vm.end()?
                _find->second.as<std::vector<std::string>>() : std::vector<std::string>{}
            ;
        }();
        // Include directories.
        std::vector<std::string> _incs = [&vm]()->std::vector<std::string>{
            auto _find = vm.find("include");
            return _find != vm.end()?
                _find->second.as<std::vector<std::string>>() : std::vector<std::string>{}
            ;
        }();
        // System include directories.
        std::vector<std::string> _sysIncs = [&vm]()->std::vector<std::string>{
            auto _find = vm.find("isystem");
            return _find != vm.end()?
                _find->second.as<std::vector<std::string>>() : std::vector<std::string>{}
            ;
        }();
        // Positional arguments.
        std::vector<std::string> _posArgs = [&vm]()->std::vector<std::string>{
            auto _find = vm.find("libclang-arg");
            return _find != vm.end()?
                _find->second.as<std::vector<std::string>>() : std::vector<std::string>{}
            ;
        }();
        // Whether the AST should be displayed before the output.
        bool _dumpAST = vm.count("dump-ast");

        std::vector<std::string> _args;
        // Adds the define options.
        for (const std::string& d : _defs) {
            _args.emplace_back("-D");
            _args.emplace_back(d);
        }
        // Adds the include options.
        for (const std::string& i : _incs) {
            _args.emplace_back("-I");
            _args.emplace_back(i);
        }
        // Adds the system include options.
        for (const std::string& i : _sysIncs) {
            _args.emplace_back("-isystem");
            _args.emplace_back(i);
        }
        // Adds the positional arguments.
        for (const std::string& a : _posArgs) {
            _args.emplace_back(a);
        }
        // Makes sure libclang uses the most recent standards.
        std::vector<const char*> _argv;
        // Generates the vector of C-strings necessary for using with libclang.
        for (const std::string& s : _args) {
            _argv.emplace_back(s.c_str());
        }
        return {
            .input = std::move(_input),
            .output = std::move(_output),
            .includeDirs = std::move(_incs),
            .args = std::move(_args),
            .argv = std::move(_argv),
            .dumpAST = _dumpAST
        };
    }
}