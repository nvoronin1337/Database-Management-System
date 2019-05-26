#include "QtGuiLogin.h"

QtGuiLogin::QtGuiLogin(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	this->user_manager = std::make_unique<LoginHelper::UserManager>(ui);

	std::string title = "Data Hive | DBMS";
	setWindowTitle(QString::fromStdString(title));

	//QPixmap pixmap("honey.png");
	//ui.backLabel->setPixmap(pixmap);
	//ui.backLabel->setMask(pixmap.mask());


	connect(ui.b_login, SIGNAL(clicked()), this, SLOT(loginButtonClicked()));
	connect(ui.b_register, SIGNAL(clicked()), this, SLOT(registerButtonClicked()));

	ui.b_login->setDefault(true);
	ui.le_username->setFocus();
}

QtGuiLogin::~QtGuiLogin()
{

}

LoginHelper::UserManager::UserManager(Ui::QtGuiLogin ui)
{
	if (&ui == nullptr) {
		throw std::invalid_argument("UI is not defined!");
	}
	this->ui = std::make_unique<Ui::QtGuiLogin>(ui);
}

bool LoginHelper::UserManager::is_login()
{
	QString q_username = this->ui->le_username->text();
	this->username = q_username.toStdString();
	if (this->username.empty()) {
		throw std::invalid_argument("Username field can't be empty!");
	}

	QString q_password = this->ui->le_password->text();
	this->password = q_password.toStdString();

	if (this->password.empty()) {
		throw std::invalid_argument("Password field can't be empty!");
	}

	auto login = std::make_unique<dbms::Login>();
	bool is_user = login->isUser(this->username, this->password);

	return is_user;
}

bool LoginHelper::UserManager::is_same_password()
{
	if (this->password == this->pass_confirm) {
		return true;
	}
	else {
		return false;
	}
}

std::pair<std::string, std::string> LoginHelper::UserManager::register_clicked()
{
	QString q_username = this->ui->le_username_2->text();
	this->username = q_username.toStdString();
	if (this->username.empty()) {
		throw std::invalid_argument("Username field can't be empty!");
	}

	QString q_password = this->ui->le_password_2->text();
	this->password = q_password.toStdString();

	if (this->password.empty()) {
		throw std::invalid_argument("Password field can't be empty!");
	}

	QString q_password_confirm = this->ui->le_password_3->text();
	this->pass_confirm = q_password_confirm.toStdString();

	if (this->pass_confirm.empty()) {
		throw std::invalid_argument("Password confirmation field can't be empty!");
	}

	if (is_same_password()) {
		return std::make_pair(this->username, this->password);
	}
	else {
		throw std::invalid_argument("Passwords are not equal!");
	}

}

void QtGuiLogin::loginButtonClicked()
{
	QObject* button = QObject::sender();
	if (button == ui.b_login)
	{
		try {
			bool is_user = this->user_manager->is_login();
			if (is_user) {
				// if username and password are correct, do something...
			
				is_dialog_complete = true;
				this->username = this->user_manager->getUsername();
				accept();	// end this dialog
			}
			else {
				// else let user know that the username or password is incorrect...
				std::cerr << "Invalid Password";
				//throw std::runtime_error("Invalid Username or Password");

				// print "Invalid Username or Password" to the small window.
				auto message_box = std::make_unique<QMessageBox>();
				message_box->warning(this, tr("DBMS Login System"), tr("Invalid Username or Password!"));
			}
		}
		catch (std::invalid_argument ex) {
			// print exception message to the small window
			auto message_box = std::make_unique<QMessageBox>();
			message_box->warning(this, tr("DBMS Login System"), tr(ex.what()));
		}
	}
	else
	{
		throw std::runtime_error("Unknown sender!");
	}
}

void QtGuiLogin::registerButtonClicked()
{
	QObject* button = QObject::sender();
	if (button == ui.b_register)
	{
		try {
			std::pair<std::string, std::string> user_data = this->user_manager->register_clicked();
			auto registrator = std::make_unique<dbms::Registrator>(user_data.first, user_data.second);
			
			is_dialog_complete = true;
			this->username = user_data.first;
			accept();	// end this dialog
		}
		catch (std::invalid_argument ex) {
			// print exception in a small window
			auto message_box = std::make_unique<QMessageBox>();
			message_box->warning(this, tr("DBMS Login System"), tr(ex.what()));
		}
	}
	else
	{
		throw std::runtime_error("Unknown sender!");
	}
}
