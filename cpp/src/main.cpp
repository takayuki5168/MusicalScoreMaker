#include <iostream>
#include <signal.h>
#include <exception>
#include <QApplication>
#include "mainwindow.hpp"

void sigIntHandler(int)
{
    std::cout << "Pressed Ctrl-C" << std::endl;
    exit(1);
}

int main(int argc, char* argv[])
{
    try {
        signal(SIGINT, sigIntHandler);

        QApplication app{argc, argv};
        mainWindow().show();
        return app.exec();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown Exception" << std::endl;
    }

    return 0;
}
