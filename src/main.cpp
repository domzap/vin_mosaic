// Qt libraries.
#include <iostream>
#include <QGuiApplication>
#include <QCommandLineParser>
#include <QByteArray>

// Project libraries.
#include "MosiacCreator.h"

// Standard libraries.

// Main function.
int main(int argc, char *argv[])
{

    // Create GUI application.
    QGuiApplication app(argc, argv);

    // Process command line.
    QCommandLineParser parser;
    parser.setApplicationDescription("Mosiac - advanced mosiac creator.");
    parser.addHelpOption();

    // Create and start the client...
    MosiacCreator mc;
    return app.exec();
}
