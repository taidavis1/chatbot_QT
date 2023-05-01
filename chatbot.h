
#ifndef CHATBOT_H
#define CHATBOT_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Chatbot; }
QT_END_NAMESPACE

class Chatbot : public QMainWindow

{
    Q_OBJECT

public:
    Chatbot(QWidget *parent = nullptr);
    ~Chatbot();

private slots:
    void on_btn_pressed();

private:
    Ui::Chatbot *ui;
};

#endif // CHATBOT_H
