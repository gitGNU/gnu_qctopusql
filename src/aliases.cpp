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


#include <aliases.h>
#include <aliases_add_dialog.h>
#include <aliases_delete_dialog.h>
#include <aliases_edit_dialog.h>
#include <aliases_find_dialog.h>

/**
 * Create aliases table.
 */
Aliases::Aliases(QSqlDatabase db, QWidget *parent)
   :QWidget(parent) {
	
	db_psql = db;
	
	ui.setupUi( this );
	
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(GetAliases()));
	connect(ui.tableWidget_Aliases, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));
	
	for(int i = 0; i < 3; i++){
		
		ui.tableWidget_Aliases->setColumnWidth(i, 100);
	}

	QStringList wordList;
	wordList << "obsd.org" ;
	
	completerModel = new QStringListModel;
	completerModel->setStringList(wordList);

	completer = new QCompleter(this);
	completer->setModel(completerModel);
	
	ui.lineEdit_Domain->setCompleter(completer);

}

/**
 * Get aliases
 */
void Aliases::GetAliases(){
	
	/*freezing input Aliases Widget*/
	ui.pushButton->setEnabled(false);
	ui.lineEdit_Domain->setEnabled(false);
	ui.tableWidget_Aliases->setEnabled(false);
	
	TestQuery();
	
	if( db_psql.isOpen() ){ 
		
		if(ui.tableWidget_Aliases->isSortingEnabled()){
			
			ui.tableWidget_Aliases->setSortingEnabled(false);
		}
		
		ui.tableWidget_Aliases->clearContents();
		ui.tableWidget_Aliases->setRowCount(0);
		
		QSqlQuery query( db_psql );
		
		query.prepare("SELECT local_part,domain,recipients  FROM aliases_view WHERE domain=:id_domain");
		query.bindValue(":id_domain", ui.lineEdit_Domain->text());
		
		if( query.exec() ){
			
			ui.tableWidget_Aliases->setRowCount(query.size());
			QProgressDialog progress(tr("Getting a list of aliases."), tr("Cancel"),0,query.size(),this);

            /* Filling the table */
			for(int i = 0; i < query.size(); i++){
		
				query.next();
				progress.setValue(i);
				qApp->processEvents();
				
				if (progress.wasCanceled()){

					ui.tableWidget_Aliases->setRowCount(i);
					break;
				}
				
				__item0 = new QTableWidgetItem();
				__item0->setText(query.value(0).toString());
				ui.tableWidget_Aliases->setItem(i, 0, __item0);
				
				__item1 = new QTableWidgetItem();
				__item1->setText(query.value(1).toString());
				ui.tableWidget_Aliases->setItem(i, 1, __item1);
				
				__item2 = new QTableWidgetItem();
				__item2->setText(query.value(2).toString());
				ui.tableWidget_Aliases->setItem(i, 2, __item2);
			}
			
			ui.tableWidget_Aliases->resizeColumnsToContents();

            /* The minimum size of column 100 pixels */
			for(int i = 0; i < 3; i++){
		
				if(ui.tableWidget_Aliases->columnWidth(i) < 100){
		
					ui.tableWidget_Aliases->setColumnWidth(i, 100);
				}
			}
	  
			ui.tableWidget_Aliases->setSortingEnabled(true);
			query.clear();
		}else{
	  
			QMessageBox::warning(this, tr("Query Error"),
								 query.lastError().text(),
								 QMessageBox::Ok);
			query.clear();
		}
	}else{
		
		emit DisconnectDB();
	}
  
	/*defrosting input Aliases Widget*/
	ui.pushButton->setEnabled(true);
	ui.tableWidget_Aliases->setEnabled(true);
	ui.lineEdit_Domain->setEnabled(true);
}

/**
 * Function creates a context menu at the point
 * of pressing the right button on the table.
 * @param
 * (const QPoint & point)
 *  point of pressing
 */
void Aliases::showContextMenu(const QPoint &point){

	ui.tableWidget_Aliases->setCurrentItem(ui.tableWidget_Aliases->itemAt(point));
	
	QMenu Pop_up;
	
	connect(Pop_up.addAction(tr("New Aliases")), SIGNAL(triggered()), this, SLOT(Dialog_Add_Aliases()));
	connect(Pop_up.addAction(tr("Find Aliases")), SIGNAL(triggered()), this, SLOT(Dialog_Find_Aliases()));
  
	if(ui.tableWidget_Aliases->indexAt(point).row() != -1){
		
		connect(Pop_up.addAction(tr("Delete Aliases")), SIGNAL(triggered()), this, SLOT(Dialog_Delete_Aliases()));
		connect(Pop_up.addAction(tr("Edit Aliases")), SIGNAL(triggered()), this, SLOT(Dialog_Edit_Aliases()));
	}
  
	Pop_up.exec(QCursor::pos());
}

/**
 * Call dialog add aliases.
 */
void Aliases::Dialog_Add_Aliases(){
	
	AliasesAddDialog *DialogAdd;
	DialogAdd = new AliasesAddDialog( db_psql, ui.tableWidget_Aliases);
	DialogAdd->setCompleterModel( completerModel );
	DialogAdd->exec();
	delete DialogAdd;
	
	TestQuery();
	
	if( !db_psql.isOpen() ){
		
		emit DisconnectDB();
	}
}

/**
 * Call dialog delete aliases.
 */
void Aliases::Dialog_Delete_Aliases(){
  
	AliasesDeleteDialog *DialogDelete;
	DialogDelete = new AliasesDeleteDialog(db_psql, ui.tableWidget_Aliases);
	DialogDelete->exec();
	delete DialogDelete;
	
	TestQuery();
	
	if( !db_psql.isOpen() ){
		
		emit DisconnectDB();
	}
}

/**
 * Call dialog edit aliases.
 */
void Aliases::Dialog_Edit_Aliases(){
  
	AliasesEditDialog *DialogEdit;
	DialogEdit = new AliasesEditDialog(db_psql, ui.tableWidget_Aliases);
	DialogEdit->exec();
	delete DialogEdit;
	
	TestQuery();
	
	if( !db_psql.isOpen() ){
		
		emit DisconnectDB();
	}
}

/**
 * Call dialog find aliases.
 */
void Aliases::Dialog_Find_Aliases(){

	AliasesFindDialog *DialogFind;
	DialogFind = new AliasesFindDialog( db_psql );
	DialogFind->setCompleterModel( completerModel );
	DialogFind->exec();
	delete DialogFind;
	
	TestQuery();
  
	if( !db_psql.isOpen() ){
		
		emit DisconnectDB();
	}
}

/**
 * Test connection.
 */
void Aliases::TestQuery(){
	
	QSqlQuery query( db_psql );
	
	query.exec("SELECT 1");
	query.clear();
}

/**
 * Function sets the list of domains
 */
void Aliases::SetCompleterList(QStringList list){
	
	completerModel->setStringList(list); 
}
