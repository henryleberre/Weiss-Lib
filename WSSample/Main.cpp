#include <WSCore/WSInclude.h>

int WS::EntryPoint(int argc, char** argv) {
    WS::Window window("Weiss Test", 1920 / 2, 720);
    //WS::Image test("Branding/icon64x64.png");

    while (true) {
        window.Update();

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    std::cin.get();
    return 0;
}