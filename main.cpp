#include <QApplication>
#include "mainwindow.h"

BOOST_CLASS_EXPORT_IMPLEMENT(Astronaut)
BOOST_CLASS_EXPORT_IMPLEMENT(DoctorAstronaut)

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
