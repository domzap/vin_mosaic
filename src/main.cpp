// Qt libraries.
#include <iostream>
#include <QApplication>
#include <QCommandLineParser>
#include <QByteArray>

// Project libraries.
#include "MosaicCreator.h"

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

    // Create and MosaicCreator
    MosaicCreator mc;
    return app.exec();
}
