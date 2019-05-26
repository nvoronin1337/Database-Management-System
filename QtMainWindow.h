#pragma once

// CLASS QtMainWindow (MAIN WINDOW)
#ifndef QT_WELCOME
#define QT_WELCOME

#include "ui_QtGuiApplication1.h"	// ui

// From Qt c++
#include <QtWidgets/QMainWindow>
#include <qwidget.h>
#include <qmessagebox.h>
#include <QDebug> 
#include <QDesktopServices>
#include <QUrl>
#include <qvector.h>

// For Qt tree view
#include <QtGui\QStandardItemModel>
#include <QItemSelectionModel> 
#include <QModelIndex> 
#include <QFile>

// My Custom Tree
#include "TreeItem.h"
#include "TreeModel.h"

// From DBMS
#include "StorageManager.h"
#include "FileIOHelper.h"

// From Gui/User
#include "GuiUser.h"

// From <std>
#include <utility>
#include <algorithm>

// Details DIALOG
#include "QtTableDetailsDialog.h"


class QtMainWindow : public QMainWindow, private Ui::QtGuiApplication1Class
{
	Q_OBJECT
public:
	QtMainWindow(const std::string username, QWidget* parent = 0);
	virtual ~QtMainWindow();

private:
	void populateSimpleTreeFromStorage() const;
	void setupSlots() noexcept;

	void writeItemData(QVector<QVariant> item_data, bool isChild) const noexcept;
	void updateUsersStorage(TreeItem* parent, TreeItem* child = 0) const noexcept;

private:
	// Tree related
	QItemSelectionModel* tree_model_selection;
	TreeModel* tree_model;

	// User data
	std::string username;
	GuiUser* user;

	// png icon file names
	const std::string DATABASE_ICON = "dbIcon.png";
	const std::string TABLE_ICON = "tableIcon.png";

public slots:
	void updateActions();

private slots:
	void insertChild();
	bool insertColumn();
	void insertRow();
	bool removeColumn();
	void removeRow();

	//url
	bool openUrl();

	// save data
	void saveModelData(bool isQuitting = false) const;

	// see details
	void btn_details_pressed() const;
};

#endif // !QT_WELCOME

