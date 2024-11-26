#include <string>
#include <cstdint>

namespace Test1 {
    class Printer {
        std::string text = "ERROR!";
    public:
        Printer();
        Printer(const std::string& text);
        void SetText(const std::string& text);
        std::string GetText() const;
        void Print() const;
    };
}