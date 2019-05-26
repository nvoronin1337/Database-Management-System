#pragma once
#ifndef DETAILS
#define DETAILS

#include <qdialog.h>
#include <ui_TableDetailsDialog.h>
#include "GuiUser.h"

#include <QMessageBox>
#include <QStandardItemModel>

class QtTableDetailsDialog :
	public QDialog, private Ui::Dialog
{
public:
	QtTableDetailsDialog(std::string database_name, std::string table_name, GuiUser* user);
	~QtTableDetailsDialog();
private:
	std::string database_name;
	std::string table_name;
	GuiUser* user;

	QStandardItemModel* model;

private slots:
	void add_button_clicked();

	void save_table();
};
#endif
