#include "src/application/application.h"

int main() {
    Application* app = Application::create();
    app->init(800,600,200);
    app->run();

    delete app;
    std::cout<<"press any key to exit"<<std::endl;
    getchar();
    return 0;
}
