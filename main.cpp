#include "QtMainWindow.h"
#include "QtGuiLogin.h"
#include <QtWidgets/QApplication>
//#include 


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	// unique pointer to the Login/Register Dialog 
	auto login_dialog = std::make_unique<QtGuiLogin>();
	login_dialog->setWindowIcon(QIcon("bee_logo.png"));
	//Qt::WindowFlags flags(Qt::WindowTitleHint);
	//login_dialog->setWindowFlags(flags);

	std::unique_ptr<QtMainWindow> main_window;
	
	login_dialog->setModal(true);
	login_dialog->exec();

	if (login_dialog->is_complete()) {
		login_dialog->hide();
		
		// unique pointer to the Main Window
		main_window = std::make_unique<QtMainWindow>(login_dialog->getUsername());
		main_window->setWindowIcon(QIcon("bee_logo.png"));
		main_window->show();
	}
	
	return a.exec();
}
