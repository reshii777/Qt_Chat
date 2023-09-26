#include "LoginWindow.h"
#include "ui_LoginWindow.h"

#include <QMessageBox>

#include "SHA_1_Wrapper.h"
#include "InputChecker.h"


//public=======================================================================
LoginWindow::LoginWindow(QWidget* parent) :
  QDialog(parent), ui(new Ui::LoginWindow)
{
  ui->setupUi(this);
}



LoginWindow::~LoginWindow()
{
  delete ui;
}



void LoginWindow::setClient(Client* client)
{
  client_ = client;
}



//public slots=================================================================
void LoginWindow::handleServerResponse_IsPasswordCorrect()
{
  auto isCorrectPassword = client_->getResponse();
  disconnectNetworkSignal_IsPasswordCorrect();

  //Handle server response
  if (isCorrectPassword == "true"){
    auto login = ui->lineEdit_Login->text().toStdString();
    ui->lineEdit_Login->clear();
    ui->lineEdit_Password->clear();

    emit toChat(login);
  }
  else{
    QMessageBox::critical(this, "Error", "Password incorrect");
    return;
  }
}



//private slots================================================================
void LoginWindow::on_pushButton_Registration_clicked()
{
  ui->lineEdit_Login->clear();
  ui->lineEdit_Password->clear();

  emit toRegistration();
}



void LoginWindow::on_buttonBox_accepted()
{
  auto login = ui->lineEdit_Login->text().toStdString();
  //Check Login input
  if (login.empty()){
    QString message = "Please type Login";
    QMessageBox::critical(this, "Error", message);
    return;
  }
  if (InputChecker::isContainsForbidenChars(login)){
    QString message = "Login contains forbidden symbols. Allowed: 'a'-'z', 'A'-'Z', '0'-'9')";
    QMessageBox::critical(this, "Error", message);
    return;
  }
  if (InputChecker::isStartsByNumber(login)){
    QString message = "Login starts by number";
    QMessageBox::critical(this, "Error", message);
    return;
  }
  if (InputChecker::isTooShort(login)){
    QString message = "Login too short. Min length: " +
                      QString::number(InputChecker::getMinLength()) +
                      " characters";
    QMessageBox::critical(this, "Error", message);
    return;
  }


  auto password = ui->lineEdit_Password->text().toStdString();
  //Check Password input
  if (password.empty()){
    QString message = "Please type Password";
    QMessageBox::critical(this, "Error", message);
    return;
  }
  if (InputChecker::isTooShort(password)){
    QString message = "Password too short. Min length: " +
                      QString::number(InputChecker::getMinLength()) +
                      " characters";
    QMessageBox::critical(this, "Error", message);
    return;
  }


  //Request verification of the login-password from the server
  auto hashPassword = sha_1::hash(password);
  connectNetworkSignal_IsPasswordCorrect();
  client_->requestIsPasswordCorrect(QString::fromStdString(login),
                                    QString::fromStdString(hashPassword));
}



void LoginWindow::on_buttonBox_rejected()
{
  //Close application
  emit close();
}



//private======================================================================
void LoginWindow::connectNetworkSignal_IsPasswordCorrect()
{
  connect(client_, &Client::dataReceived,
          this, &LoginWindow::handleServerResponse_IsPasswordCorrect);
}



void LoginWindow::disconnectNetworkSignal_IsPasswordCorrect()
{
  disconnect(client_, &Client::dataReceived,
              this, &LoginWindow::handleServerResponse_IsPasswordCorrect);
}
