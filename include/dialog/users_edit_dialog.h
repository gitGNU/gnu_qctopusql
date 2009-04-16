/*
 * Copyright (C) 2008  by Kravchuk Sergei V. (alfss@obsd.ru)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * */


#ifndef USERS_EDIT_DIALOG_H
#define USERS_EDIT_DIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QColor>
#include "ui_users_edit_dialog.h"

class UsersEditDialog : public QDialog, private Ui::DialogEdit {

    Q_OBJECT

public:

	UsersEditDialog(QSqlDatabase db,QTableWidget *Table,bool RaedOnly, QWidget *parent=0);
																						 
private slots:
	
	void Edit();
	
private:
	
	QTableWidget *pTable;
	QTableWidgetItem *__item0;
	QTableWidgetItem *__item1;
	QTableWidgetItem *__item2;
	QTableWidgetItem *__item3;
	QTableWidgetItem *__item4;
	QSqlDatabase db_psql;
	bool Empty_Test();
	void TestQuery();
};

#endif
