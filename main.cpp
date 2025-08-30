#include "src/application/application.h"
#include "src/application/global_status.h"

int main(int argc,char* argv[]) {
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg=="--rd" && i!=argc-1) {
            int render_radius = atoi(argv[i+1]);
            if (render_radius>=5&&render_radius<=30) {
                global_status::renderRadius = render_radius;
            }
        }
        if (arg=="--ms" && i!=argc-1) {
            int mapSeed = atoi(argv[i+1]);
            global_status::mapSeed = mapSeed;
        }
        if (arg=="--ts" && i!=argc-1) {
            int treeSeed = atoi(argv[i+1]);
            global_status::treeSeed = treeSeed;
        }
    }

    Application* app = Application::create();
    app->init(800,600,200);
    app->run();

    delete app;
    std::cout<<"press any key to exit"<<std::endl;
    getchar();
    return 0;
}
