#include "QtTableDetailsDialog.h"

QtTableDetailsDialog::QtTableDetailsDialog(std::string database_name, std::string table_name, GuiUser *user)
{
	setupUi(this);

	this->database_name = database_name;
	this->table_name = table_name;
	this->user = user;

	setWindowTitle(QString::fromStdString("Data Hive | User: " + user->getUsername() + " | Table: " + table_name));
	setWindowIcon(QIcon("bee_logo.png"));

	tableView->horizontalHeader()->setStyleSheet("::section{ background-color: #424242;}");
	tableView->verticalHeader()->setStyleSheet("::section{ background-color: #424242;}");

	auto table = user->getStorageManager()->getTable(database_name, table_name);

	connect(addButton, &QPushButton::clicked, this, &QtTableDetailsDialog::add_button_clicked);
	connect(okButton, &QPushButton::clicked, this, &QtTableDetailsDialog::save_table);

	model = new QStandardItemModel();
	tableView->setModel(model);

	if (table->columnNames.size() > 0) {
		QList<QString> headers_list;
		for (int header = 0; header < table->columnNames.size(); header++) {
			headers_list << QString::fromStdString(table->columnNames.at(header));
		}

		QList<QStandardItem*> items;
		for (int item = 0; item < table->records.size(); item++) {
			QString text;
			
			for (int data_field = 0; data_field < table->records.at(item)->dataFields.size(); data_field++) {
				text.append(QString::fromStdString(table->records.at(item)->dataFields.at(data_field) + " "));
			}
			
			items.append(new QStandardItem(text));
			model->appendRow(items);
		}
	}
}

void QtTableDetailsDialog::add_button_clicked() {
	int colsToAdd = std::stoi(spinBoxCols->text().toStdString());
	int rowsToAdd = std::stoi(spinBoxRows->text().toStdString());
	
	for (int i = 0; i < rowsToAdd; i++) {	
		model->insertRow(model->rowCount(QModelIndex()));
	}

	for (int i = 0; i < colsToAdd; i++) {
		model->insertColumn(model->columnCount(QModelIndex()));
	}
}

QtTableDetailsDialog::~QtTableDetailsDialog()
{

}

void QtTableDetailsDialog::save_table()
{
	std::vector<std::string> recordData;
	for (int i = 0; i < model->rowCount(); i++) {
		for (int k = 0; k < model->columnCount(); k++) {
			auto item = model->item(i, k);
			recordData.push_back(item->text().toStdString());
		}
		this->user->getStorageManager()->addRecordToTableInDatabase(database_name, table_name, recordData);
	}

	QMessageBox msgbox;
	msgbox.setText("Table saved!");
	msgbox.setStyleSheet("QMessageBox { background-color: #424242; font: 75 italic 12pt \"Gill Sans MT\"; color: rgb(1, 223, 165);}");
	msgbox.exec();
}
