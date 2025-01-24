#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

void Login::on_buttonLogin_clicked()
{
    //Kirjaudu-painiketta painettu
    QJsonObject jsonObj;
    jsonObj.insert("username",ui->textUsername->text()); // Katso luento 2:sta 16:12 eteenpäin mitä tähän tulee
    jsonObj.insert("password",ui->textPassword->text()); // Katso luento 2:sta 16:12 eteenpäin mitä tähän tulee
}

void Login::loginSlot(QNetworkReply *reply)
{

}

