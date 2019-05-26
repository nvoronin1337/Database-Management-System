#include "QtMainWindow.h"

QtMainWindow::QtMainWindow(const std::string username, QWidget* parent)
	: QMainWindow(parent)
{
	setupUi(this);

	this->username = username;
	this->user = new GuiUser(username);

	std::string fileName = username + "_form_gui.txt";

	std::string title = "Data Hive | User: " + username;
	setWindowTitle(QString::fromStdString(title));

	QStringList headers;
	headers << tr("Title") << tr("Description") << tr("Date Modified");

	QFile file(QString::fromStdString(fileName));
	file.open(QIODevice::ReadOnly);
	tree_model = new TreeModel(headers, file.readAll());
	tree_model_selection = new QItemSelectionModel(tree_model);
	
	file.close();

	treeView->header()->setStyleSheet("::section{ background-color: #424242;}");
	treeView->setHeaderHidden(false);
	treeView->setModel(tree_model);
	treeView->setSelectionModel(tree_model_selection);
	
	
	for (int column = 0; column < tree_model->columnCount(); ++column)
		treeView->resizeColumnToContents(column);
	
	setupSlots();
	
}

QtMainWindow::~QtMainWindow()
{
	saveModelData(true);
}

void QtMainWindow::populateSimpleTreeFromStorage() const
{
	QStandardItem* db_item;
	QStandardItem* table_item;
	QStandardItem* action1;

	QList<QStandardItem*> items;
	
	QStandardItemModel *tree_model = new QStandardItemModel();
	/** 
	* Loop through the Storage. For each database add an item to the tree.
	*/
	for (size_t i = 0; i < user->getStorageManager()->getStorage()->databases.size(); i++) {
		db_item = new QStandardItem(QIcon("dbIcon.png"), QString::fromStdString(user->getStorageManager()->getStorage()->databases[i]->databaseName));
		action1 = new QStandardItem(QIcon("deleteIcon.png"), QString::fromStdString(""));
		
		items.append(db_item);
		items.append(action1);
		tree_model->appendRow(items);
		
		items.clear();
		// Loop through the database. For each table add an item to the tree.
		for (size_t k = 0; k < user->getStorageManager()->getStorage()->databases[i]->tables.size(); k++) {
			table_item = new QStandardItem(QIcon("tableIcon.png"), QString::fromStdString(user->getStorageManager()->getStorage()->databases[i]->tables[k]->tableName));
			action1 = new QStandardItem(QIcon("deleteIcon.png"), QString::fromStdString(""));
			
			items.append(table_item);
			items.append(action1);
			db_item->appendRow(items);

			items.clear();
		}
	}
	treeView->setModel(tree_model);

	QModelIndex child_index = tree_model->index(0,0);
	tree_model->item(0, 0)->removeRow(0);
}

void QtMainWindow::setupSlots() noexcept
{
	connect(readMeAction, &QAction::triggered, this, &QtMainWindow::openUrl);
	connect(treeView->selectionModel(), &QItemSelectionModel::selectionChanged,
		this, &QtMainWindow::updateActions);
	connect(menuActions, &QMenu::aboutToShow, this, &QtMainWindow::updateActions);
	connect(insertRowAction, &QAction::triggered, this, &QtMainWindow::insertRow);
	connect(insertColumnAction, &QAction::triggered, this, &QtMainWindow::insertColumn);
	connect(removeRowAction, &QAction::triggered, this, &QtMainWindow::removeRow);
	connect(removeColumnAction, &QAction::triggered, this, &QtMainWindow::removeColumn);
	connect(insertChildAction, &QAction::triggered, this, &QtMainWindow::insertChild);

	connect(btnSave, SIGNAL(pressed()), this, SLOT(saveModelData()));
	connect(btnDetails, SIGNAL(pressed()), this, SLOT(btn_details_pressed()));

	updateActions();
}

void QtMainWindow::writeItemData(QVector<QVariant> item_data, bool isChild) const noexcept
{
	std::string filename_str = user->getUsername() + "_form_gui.txt";
	QString filename = QString::fromStdString(filename_str);
	QFile fileout(filename);
	if (fileout.open(QFile::ReadWrite | QFile::Text | QFile::Append)) {
		QTextStream out(&fileout);
		for (QVector<QVariant>::iterator iter = item_data.begin(); iter < item_data.end(); iter++) {
			if(!isChild)
				out <<  iter->toString() << "        ";
			else
				out << "    " << iter->toString() << "        ";
		}
		out << "\n";
		fileout.close();
	}
}

void QtMainWindow::updateUsersStorage(TreeItem* parent,TreeItem* child) const noexcept
{
	auto parent_data = parent->getItemData();
	QVector<QVariant>::iterator parent_iter = parent_data.begin();
	std::string database_name = parent_iter->toString().toStdString();

	// if database -> add database
	if (child == nullptr) {
		user->getStorageManager()->addDatabase(database_name);
	}
	// if table -> addTable
	else {
		auto child_data = child->getItemData();
		QVector<QVariant>::iterator child_iter = child_data.begin();
		std::string table_name = child_iter->toString().toStdString();

		//empty columns for now (better change)
		std::vector<std::string> cols = { "test" };

		user->getStorageManager()->addTableToDatabase(database_name, table_name, cols);
	}
}

void QtMainWindow::saveModelData(bool isQuitting) const
{
	user->getStorageManager()->getStorage()->databases.clear();

	std::string filename_str = user->getUsername() + "_form_gui.txt";

	std::ofstream ofs;
	ofs.open(filename_str, std::ofstream::out | std::ofstream::trunc);
	ofs.close();

	auto row_max = tree_model->rowCount();
	auto column = 0;

	TreeItem* tree_item;
	QModelIndex model_index;

	for (int row = 0; row < row_max; row++) {
		model_index = tree_model->index(row, column);
		
		tree_item = tree_model->getItem(model_index);
		auto item_data = tree_item->getItemData();

		writeItemData(item_data, false);
		updateUsersStorage(tree_item);

		if (tree_item->childCount() > 0) {
			for (int child_num = 0; child_num < tree_item->childCount(); child_num++) {
				auto childItem = tree_item->child(child_num);
				writeItemData(childItem->getItemData(), true);
				updateUsersStorage(tree_item, childItem);
			}
		}
	}

	auto helper = std::make_unique<dbms::FileIOHelper>();
	helper->saveStorageText(user->getStorageManager()->getStorage(), this->user->getUsername() + ".txt");

	if (!isQuitting) {
		QMessageBox msgbox;
		msgbox.setText("Data is saved!");
		msgbox.setStyleSheet("QMessageBox { background-color: #424242; font: 75 italic 12pt \"Gill Sans MT\"; color: rgb(1, 223, 165);}");
		msgbox.exec();
	}
}


// DETAILS PRESSED
void QtMainWindow::btn_details_pressed() const
{
	QModelIndex index = treeView->selectionModel()->currentIndex();
	auto item = tree_model->getItem(index);
	if (item->childCount() != 0) {
		QMessageBox msgbox;
		msgbox.setText("Please select a table!");
		msgbox.setStyleSheet("QMessageBox { background-color: #424242; font: 75 italic 12pt \"Gill Sans MT\"; color: rgb(1, 223, 165);}");
		msgbox.exec();
		return;
	}

	auto parent_item = item->parent();
	auto parent_item_data = parent_item->getItemData();
	auto iter_parent = parent_item_data.begin();
	std::string database = iter_parent->toString().toStdString();

	auto item_data = item->getItemData();
	auto iter = item_data.begin();
	std::string table_name = iter->toString().toStdString();


	std::unique_ptr<QtTableDetailsDialog> details_dialog = std::make_unique<QtTableDetailsDialog>(database, table_name, user);
	//Qt::WindowFlags flags(Qt::WindowTitleHint);
	//details_dialog->setWindowFlags(flags);
	details_dialog->exec();
}

// SLOTS
void QtMainWindow::insertChild()
{
	QModelIndex index = treeView->selectionModel()->currentIndex();
	QAbstractItemModel* model = treeView->model();

	if (model->columnCount(index) == 0) {
		if (!model->insertColumn(0, index))
			return;
	}

	if (!model->insertRow(0, index))
		return;

	for (int column = 0; column < model->columnCount(index); ++column) {
		QModelIndex child = model->index(0, column, index);
		model->setData(child, QVariant("[No data]"), Qt::EditRole);
		if (!model->headerData(column, Qt::Horizontal).isValid())
			model->setHeaderData(column, Qt::Horizontal, QVariant("[No header]"), Qt::EditRole);
	}

	treeView->selectionModel()->setCurrentIndex(model->index(0, 0, index),
		QItemSelectionModel::ClearAndSelect);
	updateActions();
}

bool QtMainWindow::insertColumn()
{
	QAbstractItemModel* model = treeView->model();
	int column = treeView->selectionModel()->currentIndex().column();

	// Insert a column in the parent item.
	bool changed = model->insertColumn(column + 1);
	if (changed)
		model->setHeaderData(column + 1, Qt::Horizontal, QVariant("[No header]"), Qt::EditRole);

	updateActions();

	return changed;
}

void QtMainWindow::insertRow()
{
	QModelIndex index = treeView->selectionModel()->currentIndex();
	QAbstractItemModel* model = treeView->model();

	if (!model->insertRow(index.row() + 1, index.parent()))
		return;

	updateActions();

	for (int column = 0; column < model->columnCount(index.parent()); ++column) {
		QModelIndex child = model->index(index.row() + 1, column, index.parent());
		model->setData(child, QVariant("[No data]"), Qt::EditRole);
	}
}

bool QtMainWindow::removeColumn()
{
	QAbstractItemModel* model = treeView->model();
	int column = treeView->selectionModel()->currentIndex().column();

	// Insert columns in each child of the parent item.
	bool changed = model->removeColumn(column);

	if (changed)
		updateActions();

	return changed;
}

void QtMainWindow::removeRow()
{
	QModelIndex index = treeView->selectionModel()->currentIndex();
	QAbstractItemModel* model = treeView->model();
	if (model->removeRow(index.row(), index.parent()))
		updateActions();
}

bool QtMainWindow::openUrl()
{
	return QDesktopServices::openUrl(QUrl("file:///C:/Users/Nikita/source/repos/QtGuiDBMS_v4_0/QtGuiApplication1/html/annotated.html", QUrl::TolerantMode));
}

void QtMainWindow::updateActions() 
{
	bool hasSelection = !treeView->selectionModel()->selection().isEmpty();
	removeRowAction->setEnabled(hasSelection);
	removeColumnAction->setEnabled(hasSelection);

	bool hasCurrent = treeView->selectionModel()->currentIndex().isValid();
	insertRowAction->setEnabled(hasCurrent);
	insertColumnAction->setEnabled(hasCurrent);

	if (hasCurrent) {
		treeView->closePersistentEditor(treeView->selectionModel()->currentIndex());

		int row = treeView->selectionModel()->currentIndex().row();
		int column = treeView->selectionModel()->currentIndex().column();
		if (treeView->selectionModel()->currentIndex().parent().isValid())
			Ui::QtGuiApplication1Class::statusBar->showMessage(tr("Position: (%1,%2)").arg(row).arg(column));
		else
			Ui::QtGuiApplication1Class::statusBar->showMessage(tr("Position: (%1,%2) in top level").arg(row).arg(column));
	}
}





