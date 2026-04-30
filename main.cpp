#include "FileXplorerInstaller.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

bool LoadCNLanguagePack(QTranslator& translator, QString qmName) {
  if (!translator.load(qmName)) {
    qDebug("Load language[%s] pack failed", qPrintable(qmName));
    return false;
  }
  qDebug("Load language[%s] pack succeed", qPrintable(qmName));
  QCoreApplication::installTranslator(&translator);
  return true;
}

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  QTranslator translator;
  LoadCNLanguagePack(translator, ":/i18n/ZH_CN");

  FileXplorerInstaller w;
  w.show();

  return a.exec();
}
