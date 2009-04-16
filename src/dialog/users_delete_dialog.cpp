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


#include <users_delete_dialog.h>
#include <QMessageBox>
#include <QString>

/**
 * Create users delete dialog.
 *
 * @param
 * (db) Connection database.
 * (*Table) Pointer to table aliases
 *
 * @return
 *  accept or reject
 */
UsersDeleteDialog::UsersDeleteDialog (QSqlDatabase db, QTableWidget *Table){

	pTable = Table;
	db_psql = db;
	setupUi( this );
	
	lineEdit_Login->setText(pTable->item(pTable->currentRow(), 0)->text());
	lineEdit_Domain->setText(pTable->item(pTable->currentRow(), 1)->text());
	connect(buttonBox,SIGNAL(accepted()),this,SLOT(Delete()));
}

/**
 * Delete user.
 */
void UsersDeleteDialog::Delete(){
	
	QSqlQuery query( db_psql );
	
	query.exec("SELECT 1");  
	query.clear();
	
	if( db_psql.isOpen() ){
		
		query.prepare("DELETE FROM users WHERE login=:login and id_domain=get_domain_id(:domain)");
		
		query.bindValue(":login", lineEdit_Login->text());
		query.bindValue(":domain", lineEdit_Domain->text());
		
		if(!query.exec()){
			
			QMessageBox::warning(this, tr("Query Error"),
								 query.lastError().text(),
								 QMessageBox::Ok);
			query.clear();
		}else{

			/* Update user table.*/
			query.clear();
			pTable->removeRow(pTable->currentRow());
			this->accept();	
		}	
	}else{
		
		this->reject();	
	}
}
