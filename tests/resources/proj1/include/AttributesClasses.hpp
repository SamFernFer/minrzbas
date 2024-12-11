class [[clang::annotate("Fwd(0)")]] ClassFwd;
class [[clang::annotate("Fwd(1)")]] ClassFwd {};

class [[clang::annotate("Back(0)")]] ClassBack {};
class [[clang::annotate("Back(1)")]] ClassBack;

class [[clang::annotate("Undefined(0)")]] ClassUndefined;

class [[clang::annotate("Both(0)")]] ClassBoth;
class ClassBoth {};
class [[clang::annotate("Both(1)")]] ClassBoth;