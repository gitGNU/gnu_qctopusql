/*
Copyright (C) 2008-2009  by Kravchuk Sergei V. (alfss@obsd.ru)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/



#include <QApplication>
#include <main_window.h>
#include <QTranslator>
#include <QPixmap>

int main (int argc, char *argv[]){

  QApplication app(argc, argv);

  QString locale = QLocale::system().name();
  qDebug(locale.toAscii());
  
  QTranslator translator_qt;
  translator_qt.load(QString(":tr/langs/qt_") + locale);
  app.installTranslator(&translator_qt);

  QTranslator translator;
  translator.load(QString(":tr/langs/qctopusql_") + locale);
  app.installTranslator(&translator);

  MainWindow MW;
  MW.show();
  MW.resize(800, 600);

  QStringList listlib;
  listlib = app.libraryPaths();
  for(int i = 0; i < listlib.size(); i++){

	qDebug(listlib.at(i).toAscii());
	
  }

  qDebug(app.applicationFilePath().toAscii());

  app.setWindowIcon(QPixmap(QString::fromUtf8(":/logo_128/images/logo/logo_128.png")));
  return app.exec();

}
