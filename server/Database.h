/**
\file Database.h
\brief Provides queries to SQL database
*/


#ifndef DATABASE_H
#define DATABASE_H

#include <string>

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>


class Database: public QObject{
  Q_OBJECT

  public:
    Database();

    /**
    Check is password correct
    \param[in] login User login
    \param[in] passwordHash Password hash
    \return Is password correct
    */
    bool isPasswordCorrect(const std::string& login,
                           const std::string& passwordHash) const;

    /**
    Check is database contains user
    \param[in] login User login
    \return Is user exists
    */
    bool isUserRegistered(const std::string& login) const;

    /**
    Add new user to database
    \param[in] login User login
    \param[in] passwordHash Password hash
    \return Is add successful
    */
    bool addUser(const std::string& login,
                  const std::string& passwordHash);

    /**
    Add common message to database
    \param[in] sender Sender login
    \param[in] message Text of message
    \return Is add successful
    */
    bool addCommonMessage(const std::string& sender,
                          const std::string& message);

    /**
    Add private message to database
    \param[in] sender Sender login
    \param[in] receiver Receiver login
    \param[in] message Text of message
    \return Is add successful
    */
    bool addPrivateMessage(const std::string& sender,
                            const std::string& receiver,
                            const std::string& message);

    /**
    \return Common messages in form single string
    */
    std::string getCommonMessages() const;

    /**
    Get user private messages
    \param[in] receiver Receiver login
    \return Private messages in form single string
    */
    std::string getPrivateMessages(const std::string& receiver)const ;


    /**
    \return List of user logins
    */
    std::list<std::string> getLogins() const;

    /**
    Remove user from database
    \param[in] login User login
    \return Is remove successful
    */
    bool removeUser(const std::string& login);

    /**
    Remove message from database
    \param[in] message Message text
    \return Is remove successful
    */
    bool removeMessage(const std::string& message);

    /**
    Switch status of user ban
    \param[in] login User login
    */
    void switchBanUser(const std::string& login);


    QSqlDatabase* getDatabase();


  signals:
    void updateDatabase();

  private:
    /**
    \param[in] login User login
    \return Is user banned
    */
    bool getIsBanned(const std::string& login) const;

    QSqlDatabase database_;
    QSqlQuery* query_;
};

#endif // DATABASE_H
