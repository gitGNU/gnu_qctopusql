/*
 * Copyright (C) 2008-2009  by Kravchuk Sergei V. (alfss@obsd.ru)
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


#include <aliases_add_dialog.h>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QRegExp>
#include <QValidator>
#include <QPalette>
#include <QComboBox>
#include <QSpinBox>
#include <QString>
#include <QTableWidgetItem>

/**
 * Create aliases add dialog.
 *
 * @param
 * (db)Connection database.
 * (*Table) Pointer to table aliases.
 *
 * @return
 *  accept or reject
 */
AliasesAddDialog::AliasesAddDialog(QSqlDatabase db, QTableWidget *Table,QWidget *parent)
	: QDialog(parent) {
	
	db_psql = db;
	pTable = Table;
	
	setupUi( this );
	
	alpha = new QRegExp("-?[a-z.\\-_0-9]+");
	ValLocal_Part = new QRegExpValidator(*alpha, this);
	
	connect(pushButton_Add, SIGNAL(clicked()), this, SLOT(NewRow()));
	connect(pushButton_Deleted, SIGNAL(clicked()), this, SLOT(DeleteRow()));
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(Add()));
	
	lineEdit_Local_Part->setValidator(ValLocal_Part);
	tableWidget_recipients->setColumnWidth(0, 230);
	tableWidget_recipients->setColumnWidth(1, 230);
	
	completer = new QCompleter(this);
	lineEdit_Domain->setCompleter(completer);
}

AliasesAddDialog::~AliasesAddDialog(){

	delete completer;
	delete alpha;
	delete ValLocal_Part;
}

/**
 * Create new item recipients.
 */
void AliasesAddDialog::NewRow(){
	
	tableWidget_recipients->setRowCount(tableWidget_recipients->rowCount() + 1);
	
	QTableWidgetItem *__item0 = new QTableWidgetItem();
	__item0->setText("login");
	tableWidget_recipients->setItem(tableWidget_recipients->rowCount() - 1, 0, __item0);
	
	QTableWidgetItem *__item1 = new QTableWidgetItem();
	__item1->setText(lineEdit_Domain->text());
	tableWidget_recipients->setItem(tableWidget_recipients->rowCount() - 1, 1, __item1);  
}

/**
 * Delete item recipients.
 */
void AliasesAddDialog::DeleteRow(){
  
	tableWidget_recipients->removeRow(tableWidget_recipients->rowCount() - 1);
}

/**
 * Create new aliases.
 */
void AliasesAddDialog::Add(){
	
	QString recipients;
	if( Empty_Test() ){
		
	/*
	  Create string recipients
	*/
		for(int i = 0; i < tableWidget_recipients->rowCount(); i++){
			
			recipients.append(tableWidget_recipients->item(i, 0)->text());
			recipients.append("@");
			recipients.append(tableWidget_recipients->item(i, 1)->text());
			if(tableWidget_recipients->rowCount() != (i + 1)){
				
				recipients.append(",");
			}
			
		}
		
		TestQuery();
		
		if( db_psql.isOpen() ){ 
			
			QSqlQuery query( db_psql );

            //Create SQL query
			query.prepare("INSERT INTO aliases (local_part,id_domain,recipients)"
						  "VALUES (:local_part,get_domain_id(:domain),:recipients)");
			
			query.bindValue(":local_part", lineEdit_Local_Part->text());
			query.bindValue(":domain", lineEdit_Domain->text());
			query.bindValue(":recipients", recipients);			

			if( !query.exec() ){
				
				QMessageBox::warning(this, tr("Query Error"),
									 query.lastError().text(),
									 QMessageBox::Ok);
				query.clear();
			}else{
				
				/* Update aliases table.*/
				pTable->insertRow(0);
				
				__item0 = new QTableWidgetItem();
				__item0->setText(lineEdit_Local_Part->text());
				pTable->setItem(0, 0, __item0);
				
				__item1 = new QTableWidgetItem();
				__item1->setText(lineEdit_Domain->text());
				pTable->setItem(0, 1, __item1);
				
				__item2 = new QTableWidgetItem();
				__item2->setText(recipients);
				pTable->setItem(0, 2, __item2);
				
				query.clear();
				this->accept();
			}
		}else{
			
			this->reject();
		}	
	}
} 

/**
 * Checks for empty fields.
 *
 * @return
 * (true) not empty,
 * (false) empty.
 */
bool AliasesAddDialog::Empty_Test(){
  
	QPalette pal;
	
	lineEdit_Local_Part->setPalette(QApplication::palette());
	lineEdit_Domain->setPalette(QApplication::palette());
	
	pal.setColor(QPalette::Base, Qt::red);
	
	if(lineEdit_Domain->text().isEmpty()){//Checks for empty field domain.		
		
		lineEdit_Domain->setPalette(pal);		
	}else if(lineEdit_Local_Part->text().isEmpty()){//Checks for empty field Local Part.
		
		lineEdit_Local_Part->setPalette(pal);
	}else{

		//recipients >0
		if(tableWidget_recipients->rowCount() == 0){
			
			return false;
		}

		//Check recipients fields
		for(int i = 0; i < tableWidget_recipients->rowCount(); i++){
			
			if(tableWidget_recipients->item(i, 0)->text().isEmpty()){
				return false;
			}
			
			if(tableWidget_recipients->item(i, 1)->text().isEmpty()){
				return false;
			}
			
		}
		
		return true;	
	}
	
  return false;
}

/**
 * Test connection.
 */
void AliasesAddDialog::TestQuery(){
	
	QSqlQuery query( db_psql );
	
	query.exec("SELECT 1");
	query.clear();	
}

/**
 * Function sets the list of domains
 */
void AliasesAddDialog::setCompleterModel(QAbstractItemModel *model){
	
	completer->setModel(model);
}
