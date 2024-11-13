#include <utils/Misc.hpp>

#ifdef _WIN32
#include <Windows.h>
#endif

#include <iostream>
#include <locale>

namespace Fenton::Minrzbas::Tests {
    bool classes();
    bool filtering();
    bool parsing();
}
int main() {
    using namespace Fenton::Minrzbas::Tests;

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    std::ostream& _out = std::cout;

    std::cout << "Não." << std::endl;

    // Changes the default output stream.
    Fenton::setDefaultOutput(_out);

    Fenton::println("Desde já, olá! こんにちは！");
    Fenton::println(2349.92304);

    std::cout << 1123123 << std::endl;

    Fenton::println(CP_UTF8);

    std::string rareLatinCharacters = "ÀÁÂÃÄÅĀĂĄǍǞǠǺȀȂȦȨɅƁƂƄƆƇƉƊƋƌƎƏƐƑƓƔƖƘǶǷƜƞƟƠƢƤƦƬƮƯƱƲƳƵȜȝȢȤȥȦȧȨȩȪȫȬȭȮȯȰȱȲȳȴȵȶȷɀɁɂɃɄɆɇɈɉɊɋɌɍɎɏʙʛʞʠʬʰʲʳʴʵʶʷʸʹʺʻʼʽʾʿˀˁ˂˃˄˅ˇˈˉˊˋˌˍˎˏːˑ˒˓˔˕˖˗˘˙˚˛˜˝˞˟ˠˡˢˣˤ˥˦˧˨˩˪˫ˬ˭ˮ˯˰˱˲˳˴˵˶˷˸˹˺˻˼˽˾˿";

    std::cout << rareLatinCharacters << std::endl;

    std::cin.get();

    return 0;

    bool _pass = true;
    try {
        // _pass &= classes();
        // _pass &= filtering();
        _pass &= parsing();
    } catch(const std::exception& e) {
        _pass = false;
        Fenton::printlnf("[EXCEPTION]\n{}", e.what());
    } catch(...) {
        _pass = false;
        Fenton::printlnf("[UNKNOWN EXCEPTION]");
    }
    Fenton::printlnf("[FINAL]: {}", _pass? "PASS" : "FAIL");
    std::cin.get();
    return 0;
}