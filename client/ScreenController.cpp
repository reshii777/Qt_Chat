#include "ScreenController.h"
#include "ui_ScreenController.h"

#include <QtDebug>
#include <QMessageBox>

#include "SHA_1_Wrapper.h"

namespace{
  enum{
    LOGIN_WINDOW = 0,
    REGISTRATION_WINDOW,
    CHAT_WINDOW
  };
}


//public=======================================================================
ScreenController::ScreenController(QWidget* parent):
  QDialog(parent), ui(new Ui::ScreenController)
{
  ui->setupUi(this);

  //Setup network
  client_ = new Client("127.0.0.1", 7777);

  ui->loginWindow->setClient(client_);
  ui->registrationWindow->setClient(client_);
  ui->chatWindow->setClient(client_);

  //Set start window
  setLoginWindow();

  setupSignals();
}



ScreenController::~ScreenController()
{
  delete ui;
  qApp->exit(0);
}



//public slots-----------------------------------------------------------------
void ScreenController::setLoginWindow()
{
  ui->stackedWidget->setCurrentIndex(LOGIN_WINDOW);
}



void ScreenController::setRegistrationWindow()
{
  ui->stackedWidget->setCurrentIndex(REGISTRATION_WINDOW);
}



void ScreenController::setChatWindow()
{
  ui->stackedWidget->setCurrentIndex(CHAT_WINDOW);
  ui->chatWindow->connectNetworkSignal_UpdateMessage();
}



void ScreenController::exit()
{
  this->close();
}



void ScreenController::handleNetworkFailed()
{
  QString message = "Server connection failed. Contact the server administrator or try to connect later.";
  QMessageBox::critical(this, "Error", message);
  this->close();
}


//private----------------------------------------------------------------------
void ScreenController::setupSignals()
{ 
  connect(client_, &Client::failConnection,
        this, &ScreenController::handleNetworkFailed);

  //Signals - to close application
  connect(ui->loginWindow, &LoginWindow::close,
        this, &ScreenController::exit);

  connect(ui->registrationWindow, &RegistrationWindow::close,
        this, &ScreenController::exit);


  //Signals - windows transition

  //Transitions from LoginWindow
  connect(ui->loginWindow, &LoginWindow::toRegistration,
          this, &ScreenController::setRegistrationWindow);

  connect(ui->loginWindow, &LoginWindow::toChat,
          this, &ScreenController::setChatWindow);

  connect(ui->loginWindow, &LoginWindow::toChat,
          ui->chatWindow, &ChatWindow::setUserLogin);

  //Transitions from RegistrationWindow

  connect(ui->registrationWindow, &RegistrationWindow::toLogin,
          this, &ScreenController::setLoginWindow);
  connect(ui->registrationWindow, &RegistrationWindow::toChat,
          this, &ScreenController::setChatWindow);

  connect(ui->registrationWindow, &RegistrationWindow::toChat,
          ui->chatWindow, &ChatWindow::setUserLogin);

  //Transitions from ChatWindow
  connect(ui->chatWindow, &ChatWindow::toLogin,
          this, &ScreenController::setLoginWindow);
}
