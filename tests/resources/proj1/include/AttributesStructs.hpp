struct [[clang::annotate("Fwd(0)")]] StructFwd;
struct [[clang::annotate("Fwd(1)")]] StructFwd {};

struct [[clang::annotate("Back(0)")]] StructBack {};
struct [[clang::annotate("Back(1)")]] StructBack;

struct [[clang::annotate("Undefined(0)")]] StructUndefined;

struct [[clang::annotate("Both(0)")]] StructBoth;
struct StructBoth {};
struct [[clang::annotate("Both(1)")]] StructBoth;