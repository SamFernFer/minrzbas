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
    static CXChildVisitResult reflVisitor(CXCursor c, CXCursor parent, CXClientData client_data) {
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

        // Indent.
        _indent->append(4, ' ');
        // Visits the children recursively.
        clang_visitChildren(c, reflVisitor, client_data);
        // De-indent.
        _indent->resize(_indent->size()-4);

        return CXChildVisit_Continue;
    }

    boost::json::object unitToJSON(
        const std::string& filePath,
        const std::vector<std::string>& args
    ) {
        if (!fs::exists(filePath)) {
            throw std::runtime_error(std::format(
                "The file \"{0}\" could not be found.", filePath
            ));
        }

        std::vector<const char*> _args;
        // Prepares the arguments.
        for (auto& a : args) {
            _args.emplace_back(a.c_str());
        }
        /*for (auto& a : args) {
            std::cout << a << std::endl;
        }*/
        
        // Display the diagnostics.
        CXIndex index = clang_createIndex(0, true);
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

        // json::object _rootNS;
        // ReflVisitData _data;
        std::string _indent;

        CXCursor cursor = clang_getTranslationUnitCursor(unit);
        clang_visitChildren(
            cursor,
            reflVisitor,
            &_indent
        );

        clang_disposeTranslationUnit(unit);
        clang_disposeIndex(index);

        // Not returning anything for now.
        return {};
    }
}