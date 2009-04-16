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


#include <users_edit_dialog.h>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QRegExp>
#include <QValidator>
#include <QPalette>
#include <QComboBox>
#include <QSpinBox>

/**
 * Create user edit dialog.
 *
 * @param
 * (db)Connection database.
 * (*Table) Pointer to table aliases.
 *
 * @return
 *  accept or reject
 */
UsersEditDialog::UsersEditDialog(QSqlDatabase db,QTableWidget *Table,bool ReadOnly,QWidget *parent)
        : QDialog(parent) {

	pTable = Table;
	db_psql = db;
	
	setupUi( this );
	
	if( ReadOnly ){
		
		connect(buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
	}else{
		
		connect(buttonBox,SIGNAL(accepted()),this,SLOT(Edit()));
	}

	/*
	  get User info
	  
	*/
	TestQuery();
	
	if( db_psql.isOpen() ){ 
		
		QSqlQuery query(db_psql);
		query.prepare("SELECT login,name,decrypt,domain,quota,status,id_spam,"
					  "spam_score FROM users_view WHERE login=:login and domain=:domain LIMIT 1");
		query.bindValue(":login", pTable->item(pTable->currentRow(), 0)->text());
		query.bindValue(":domain", pTable->item(pTable->currentRow(), 1)->text());
		if(query.exec()){
	  
			for(int i=0;i<query.size();i++){
				
				query.next();
		
				lineEdit_Login->setText(query.value(0).toString());
				lineEdit_Name->setText(query.value(1).toString());
				lineEdit_Password->setText(query.value(2).toString());
				lineEdit_Domain->setText(query.value(3).toString());
				spinBox_Quote->setValue(query.value(4).toInt());

				if(query.value(5).toInt() == 1){
					
					checkBox->setCheckState(Qt::Checked);
				}else{
					
					checkBox->setCheckState(Qt::Unchecked);
				}
		
				comboBox_Spam->setCurrentIndex(query.value(6).toInt());
				spinBox_Spam_Score->setValue(query.value(7).toInt());
			}
			
			query.clear();
		}else{

			QMessageBox::warning(this, tr("Query Error"),
								 query.lastError().text(),
								 QMessageBox::Ok);
			query.clear();
		}
	}else{
		
		this->reject();
	}
	
	lineEdit_Login->setReadOnly(true);
	lineEdit_Domain->setReadOnly(true);
	
	if(ReadOnly){
		
		lineEdit_Password->setReadOnly(true);
		lineEdit_Name->setReadOnly(true);
		spinBox_Quote->setDisabled(true);
		comboBox_Spam->setDisabled(true);
		spinBox_Spam_Score->setDisabled(true);
		checkBox->setDisabled(true);
	}
}

/**
 * Checks for empty fields.
 *
 * @return
 * (true) not empty,
 * (false) empty.
 */
bool  UsersEditDialog::Empty_Test(){

	QPalette pal;
	
	lineEdit_Name->setPalette(QApplication::palette());
	lineEdit_Password->setPalette(QApplication::palette());
	
	pal.setColor(QPalette::Base, Qt::red);
	
	if(lineEdit_Name->text().isEmpty()){
		
		lineEdit_Name->setPalette(pal);
	}else if(lineEdit_Password->text().isEmpty()){
		
		lineEdit_Password->setPalette(pal);
	}else{
		
		return true;
	}
	
	return false;
}

/**
 * Update user.
 */
void UsersEditDialog::Edit(){
	
	if( Empty_Test() ){
		
		QColor ColorItem(0, 0, 0);
		TestQuery();
		
		if( db_psql.open() ){ 
			
			QSqlQuery query(db_psql);

			query.prepare("UPDATE users SET status=:status, name=:name, password=crypt(:password, gen_salt('des')),"
						  "decrypt=:decrypt, quota=:quota, id_spam=:spam,spam_score=:spam_score"
						  " WHERE login=:login and id_domain=get_domain_id(:domain)");
	  
	
			query.bindValue(":login", lineEdit_Login->text());//1
			query.bindValue(":name", lineEdit_Name->text());//2
			query.bindValue(":password",lineEdit_Password->text());//3
			query.bindValue(":decrypt", lineEdit_Password->text());//4
			query.bindValue(":domain", lineEdit_Domain->text());//5
			query.bindValue(":quota", spinBox_Quote->value());//6
	  
			if(checkBox->checkState()==Qt::Checked){

				query.bindValue(":status", 1);//7
			}else{

				query.bindValue(":status", 0);
			}
			
			query.bindValue(":spam",comboBox_Spam->currentIndex());//8
			query.bindValue(":spam_score",spinBox_Spam_Score->value() );//9
			
			if(!query.exec()){
				
				QMessageBox::warning(this, tr("Connections Error"),
									 query.lastError().text(),
									 QMessageBox::Ok);
				query.clear();
			}else{

				/* Update users table*/
				__item0 = pTable->item(pTable->currentRow(), 0);
				__item0->setText(lineEdit_Login->text());
				
				__item1 = pTable->item(pTable->currentRow(), 1);
				__item1->setText(lineEdit_Domain->text());
				
				__item2 = pTable->item(pTable->currentRow(), 2);
				__item2->setText(lineEdit_Name->text());
				
				__item4 = pTable->item(pTable->currentRow(), 4);
				__item4->setText(QString::number(spinBox_Quote->value(), 10));
				
				__item3 = pTable->item(pTable->currentRow(), 3);
				
				if(checkBox->checkState() == Qt::Checked){
					
					__item3->setText("1");
					ColorItem.setRgb(198, 252, 155);
				}else{
					
					__item3->setText("0");
					ColorItem.setRgb(255, 202, 82);
				}

                //Set color
				__item0->setBackgroundColor(ColorItem);
				__item1->setBackgroundColor(ColorItem);
				__item2->setBackgroundColor(ColorItem);
				__item3->setBackgroundColor(ColorItem);
				__item4->setBackgroundColor(ColorItem);
				
				query.clear();
				this->accept();
			}
		}else{
			
			this->reject();
		}
	}
}

/**
 * Test connection.
 */
void UsersEditDialog::TestQuery(){
	
	QSqlQuery query( db_psql );
	
	query.exec("SELECT 1");
	query.clear();
}
