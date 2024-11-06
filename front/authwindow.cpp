#include "authwindow.h"
#include "ui_authwindow.h"
#include <QMessageBox>

extern bool m_userAuthorised;

AuthWindow::AuthWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AuthWindow)
{
    ui->setupUi(this);
}

AuthWindow::~AuthWindow()
{
    delete ui;
}

void AuthWindow::on_pushButton_pressed()
{
    QString pass = ui->textEdit->toPlainText();

    if(pass == "IDDQD") {
        QMessageBox::information(this, "OK", "Вы авторизованы");
        m_userAuthorised = true;
        this->close();
    }
    else {
        QMessageBox::warning(this, "Ошибка", "Пароль не верный");
    }
}

