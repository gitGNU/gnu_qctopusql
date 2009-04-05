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


#include <users_forward_find_dialog.h>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QRegExp>
#include <QValidator>
#include <QPalette>
#include <QComboBox>
#include <QSpinBox>
#include <QString>
#include <QMenu>
#include <users_forward_delete_dialog.h>
#include <users_forward_edit_dialog.h>

UsersForwardFindDialog::UsersForwardFindDialog(QSqlDatabase db, QWidget *parent)
        : QDialog(parent) {
  
  db_psql = db;

  setupUi( this );


  connect(pushButton_Search, SIGNAL(clicked()), this, SLOT(Find()));
  connect(checkBox, SIGNAL(clicked(bool)), lineEdit_Domain, SLOT(setEnabled(bool)));
  connect(tableWidget_Find, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));

  completer = new QCompleter(this);
  lineEdit_Domain->setCompleter(completer);
}

UsersForwardFindDialog::~UsersForwardFindDialog(){

  delete completer;

}


void UsersForwardFindDialog::Find(){
  
  QString StringFind;
  QString StringDomain;
  
  if( lineEdit_Domain->isEnabled() ){
	
	StringDomain.append("%");
	StringDomain.append(lineEdit_Domain->text());
	StringDomain.append("%");
  
  }else{
	
	StringDomain="%%";
  
  }

  StringFind.append("%");
  StringFind.append(lineEdit_Find->text());
  StringFind.append("%");

  TestQuery();
  
  if( db_psql.isOpen() ){ 	

	if(tableWidget_Find->isSortingEnabled()){
	  
	  tableWidget_Find->setSortingEnabled(false);
	  
	}
	
	tableWidget_Find->clearContents();
	tableWidget_Find->setRowCount(0);
	
	QSqlQuery query( db_psql );
	
	if(comboBox->currentIndex() == 0){
	  
	  query.prepare("SELECT local_part,domain,recipients FROM userforward_view WHERE local_part ILIKE :find and domain ILIKE :domain");
	
	}else{
	  
	  query.prepare("SELECT local_part,domain,recipients FROM userforward_view WHERE recipients ILIKE :find and domain ILIKE :domain");
	
	}
	
	query.bindValue(":find", StringFind);
	query.bindValue(":domain", StringDomain);
	
	if( !query.exec() ){
	  
	  QMessageBox::warning(this, tr("Query Error"),
						   query.lastError().text(),
						   QMessageBox::Ok);
	  query.clear();
	  
	}else{
	  
	  for(int i = 0; i < query.size(); i++){
		
		query.next();
		tableWidget_Find->setRowCount(i + 1);
		
		__item0 = new QTableWidgetItem();
		__item0->setText(query.value(0).toString());
		tableWidget_Find->setItem(i, 0, __item0);
		
		__item1 = new QTableWidgetItem();
		__item1->setText(query.value(1).toString());
		tableWidget_Find->setItem(i, 1, __item1);
		
		__item2 = new QTableWidgetItem();
		__item2->setText(query.value(2).toString());
		tableWidget_Find->setItem(i, 2, __item2);
		
	  }
	  
	}
	
	tableWidget_Find->setSortingEnabled(true);
	query.clear();
	
  }else{

	this->reject();
  
  }
  
}

void UsersForwardFindDialog::showContextMenu(const QPoint &point){
  
  tableWidget_Find->setCurrentItem(tableWidget_Find->itemAt(point));
  
  if(tableWidget_Find->indexAt(point).row() != -1){
	
	QMenu Pop_up;
	
	connect(Pop_up.addAction(tr("Delete Users Forward")), SIGNAL(triggered()), this, SLOT(DialogDelete()));
	connect(Pop_up.addAction(tr("Edit Users Forward")), SIGNAL(triggered()), this, SLOT(DialogEdit()));
	
	Pop_up.exec(QCursor::pos());
	
  }
  
}

void UsersForwardFindDialog::DialogDelete(){
	
	UsersForwardDeleteDialog *DialogDelete;
	DialogDelete = new UsersForwardDeleteDialog(db_psql, tableWidget_Find);
	DialogDelete->exec();
	delete DialogDelete;
	
	TestQuery();
	
	if( !db_psql.isOpen() ){
		
		this->reject();
	}
}

void UsersForwardFindDialog::DialogEdit(){
	
	UsersForwardEditDialog *DialogEdit;
	DialogEdit = new UsersForwardEditDialog(db_psql, tableWidget_Find);
	DialogEdit->exec();
	delete DialogEdit;
	
	TestQuery();
	
	if( !db_psql.isOpen() ){
		
		this->reject();	
	}
}

void UsersForwardFindDialog::TestQuery(){

  QSqlQuery query( db_psql );
  
  query.exec("SELECT 1");
  query.clear();
  
}

void UsersForwardFindDialog::setCompleterModel(QAbstractItemModel *model){

  completer->setModel(model);

}
