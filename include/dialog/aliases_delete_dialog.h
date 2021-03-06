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



#ifndef ALIASES_DELETE_DIALOG_H
#define ALIASES_DELETE_DIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QTableWidget>
#include "ui_aliases_delete_dialog.h"


class AliasesDeleteDialog : public QDialog, private Ui::DialogAliasesDelete {
    Q_OBJECT
public:
	AliasesDeleteDialog (QSqlDatabase db, QTableWidget *Table);
															  
private slots:
	void Delete();
	
private:
	QTableWidget *pTable;
    //QTableWidgetItem *Table;
	QSqlDatabase db_psql;
	void TestQuery();
    //int CurrentRow;
//signals:
//	void AliasesDelete(int row);
};


#endif
