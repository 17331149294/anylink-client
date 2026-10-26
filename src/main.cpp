#include "anylink.h"
#include "common.h"
#include "configmanager.h"
#include <QApplication>
#include <QStyleHints>
#include <QPalette>
#include <QStandardPaths>
#include <QDir>
#include <QTranslator>
#if defined(Q_OS_LINUX) || defined(Q_OS_MACOS) || defined(Q_OS_WIN)
#include "singleapplication.h"
#endif
#include <QDesktopServices>

void outdateCheck()
{
    if (QDate::currentDate().daysTo(QDate(2024,5,1)) < 0) {
        error(QObject::tr("The current version of the software has expired, please install the latest version!"));
    }
}

int main(int argc, char *argv[])
{
    qSetMessagePattern("%{type}:[%{file}:%{line}]  %{message}");
    //    qDebug() << QStyleFactory::keys();
    //    QApplication::setStyle("fusion");
    QApplication::setApplicationName("fuck");
    configLocation = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
//    qDebug() << configLocation;
    tempLocation = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QDir().mkpath(configLocation);

#if defined(Q_OS_LINUX) || defined(Q_OS_MACOS) || defined(Q_OS_WIN)
    SingleApplication app(argc, argv);
#else
    QApplication app(argc, argv);
#endif

    // 检测系统深色模式（Windows/Linux 深色主题），避免深色模式下登录界面全白、按钮不可见
    if (app.styleHints()->colorScheme() == Qt::ColorScheme::Dark) {
        isDarkMode = true;
        QApplication::setStyle("fusion");
        QPalette darkPalette;
        darkPalette.setColor(QPalette::Window, QColor(43, 43, 43));
        darkPalette.setColor(QPalette::WindowText, QColor(240, 240, 240));
        darkPalette.setColor(QPalette::Base, QColor(30, 30, 30));
        darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::ToolTipBase, QColor(43, 43, 43));
        darkPalette.setColor(QPalette::ToolTipText, QColor(240, 240, 240));
        darkPalette.setColor(QPalette::Text, QColor(240, 240, 240));
        darkPalette.setColor(QPalette::Button, QColor(70, 70, 70));
        darkPalette.setColor(QPalette::ButtonText, QColor(240, 240, 240));
        darkPalette.setColor(QPalette::BrightText, QColor(255, 80, 80));
        darkPalette.setColor(QPalette::Link, QColor(88, 166, 255));
        darkPalette.setColor(QPalette::Highlight, QColor(70, 130, 180));
        darkPalette.setColor(QPalette::HighlightedText, QColor(255, 255, 255));
        darkPalette.setColor(QPalette::PlaceholderText, QColor(160, 160, 160));
        app.setPalette(darkPalette);
    }

    configManager = new ConfigManager();
    // Multiple translation files can be installed.
    // Translations are searched for in the reverse order in which they were installed
    QTranslator myTranslator; // must global

    if(configManager->loadConfig(Json)) {
        if(configManager->config["local"].toBool()) {
//            qDebug() << QLocale::system().name();
            // embedded in qrc
            if(myTranslator.load(QLocale(), QLatin1String("anylink"), QLatin1String("_"), QLatin1String(":/i18n"))) {
                app.installTranslator(&myTranslator);
            }
        }
    }

//    outdateCheck();

    AnyLink w;
    w.show();

    QApplication::setQuitOnLastWindowClosed(false);
    QObject::connect(&app, &SingleApplication::instanceStarted, &w, &AnyLink::showNormal);

    return app.exec();
}
