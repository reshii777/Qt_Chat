#include "Client.h"

#include <QDataStream>


namespace{
  //Command type
  enum Command{
    IS_PASSWORD_CORRECT,
    IS_USER_REGISTERED,
    ADD_USER,
    ADD_COMMON_MESSAGE,
    ADD_PRIVATE_MESSAGE,
    GET_COMMON_MESSAGE,
    GET_PRIVATE_MESSAGE,
    GET_USER_LOGINS,
    REMOVE_USER
  };
}


//public=======================================================================
Client::Client(QString hostIP, int port)
{
  socket_ = new QTcpSocket(this);
  connect(socket_, &QTcpSocket::readyRead,
          this, &Client::getData);
  connect(socket_, &QTcpSocket::disconnected,
          socket_, &QTcpSocket::deleteLater);
  nextBlockSize_ = 0;
  response_.clear();
  socket_->connectToHost(hostIP, port);
}



void Client::requestIsPasswordCorrect(const QString& login,
                                      const QString& passwordHash)
{
  QString args = login + "|" + passwordHash;
  sendRequest(IS_PASSWORD_CORRECT, args);
}



void Client::requestIsUserRegistered(const QString& login)
{
  sendRequest(IS_USER_REGISTERED, login);
}



void Client::requestAddUser(const QString& login,
                            const QString& passwordHash)
{
  QString args = login + "|" + passwordHash;
  sendRequest(ADD_USER, args);
}



void Client::requestUserLogins()
{
  sendRequest(GET_USER_LOGINS, "");
}



void Client::requestCommonMessage()
{
  sendRequest(GET_COMMON_MESSAGE, "");
}



void Client::requestPrivateMessage(const QString& receiver)
{
  sendRequest(GET_PRIVATE_MESSAGE, receiver);
}



void Client::sendCommonMessage(const QString& sender,
                               const QString& message)
{
  QString args = sender + "|" + message;
  sendRequest(ADD_COMMON_MESSAGE, args);
}



void Client::sendPrivateMessage(const QString& sender,
                                const QString& receiver,
                                const QString& message)
{
  QString args = sender + "|" + receiver + "|" + message;
  sendRequest(ADD_PRIVATE_MESSAGE, args);
}



void Client::requestRemoveUser(const QString& login)
{
  sendRequest(REMOVE_USER, login);
}



QString Client::getResponse()
{
  auto result = response_;
  response_.clear();

  return result;
}



//private======================================================================
void Client::sendRequest(int commandType, const QString& args)
{
  if (socket_->state() != QAbstractSocket::ConnectedState){
    emit failConnection();
    return;
  }


  data_.clear();
  QDataStream out(&data_, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_5_6);

  //Clear first 2 byteBs
  out << quint16(0) << commandType << args;

  //Set read pointer to begin of message
  out.device()->seek(0);

  //Calculate and put size of message without first 2 bytes (quint16)
  out << quint16(data_.size() - sizeof(quint16));

  //Send data
  socket_->write(data_);
}



void Client::getData()
{
  QDataStream in(socket_);
  in.setVersion(QDataStream::Qt_5_6);
  if (in.status() == QDataStream::Ok){
    for(;;){
      if (nextBlockSize_ == 0){
        if (socket_->bytesAvailable() < 2){
          break;
        }
        in >> nextBlockSize_;
      }
      if (socket_->bytesAvailable() < nextBlockSize_){
        break;
      }
      in >> response_;
      nextBlockSize_ = 0;
    }
  }

  emit dataReceived();
}
