#pragma once

// CLASS USER MANAGER
#ifndef USER_MANAGER
#define USER_MANAGER

#include "ui_QtGuiLogin.h"
#include "Login.h"
#include "Registrator.h"
#include <string>
#include <memory>
#include <utility>

namespace LoginHelper {
	class UserManager {
	private:
		std::unique_ptr<Ui::QtGuiLogin> ui;

		std::string username;
		std::string password;
		std::string pass_confirm;

		bool is_same_password();
	public:
		UserManager(Ui::QtGuiLogin ui);	// throws std::invalid_argument exception
		~UserManager() = default;

		std::string getUsername() const {
			return this->username;
		}
		bool is_login();						// throws std::invalid_argument exception
		std::pair<std::string, std::string> register_clicked();
	};
}

#endif //  !USERMANAGER

#ifndef QT_GUI_LOGIN
#define QT_GUI_LOGIN

#include <QDialog>
#include <qwidget.h>
#include "ui_QtGuiLogin.h"	// ui
#include <qmessagebox.h>
#include <qpixmap.h>
#include <qbitmap.h>

class QtGuiLogin : public QDialog
{
	Q_OBJECT

public:
	QtGuiLogin(QWidget *parent = Q_NULLPTR);
	virtual ~QtGuiLogin();

	bool is_complete() const {
		return is_dialog_complete;
	}

	std::string getUsername() const {
		return username;
	}

public slots:
	void loginButtonClicked();	// throws std::runtime_error exception
	void registerButtonClicked();	// throws std::runtime_error exception

private:
	bool is_dialog_complete = false;
	std::string username;

	Ui::QtGuiLogin ui;
	std::unique_ptr<LoginHelper::UserManager> user_manager;
};

#endif	// !QT_GUI_LOGIN
