// Qt libraries.
#include <iostream>
#include <QApplication>
#include <QCommandLineParser>
#include <QByteArray>

// Project libraries.
#include "MosiacCreator.h"

// Standard libraries.

// Main function.
int main(int argc, char *argv[])
{

    // Create GUI application.
    QApplication app(argc, argv);

    // Process command line.
    QCommandLineParser parser;
    parser.setApplicationDescription("EasyMosaic");
    parser.addHelpOption();

    // Create and start the client...
    MosiacCreator mc;
    return app.exec();
}
