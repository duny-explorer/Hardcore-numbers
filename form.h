#ifndef FORM_H
#define FORM_H
#include "ui_form.h"
#include "printscene.h"

namespace Ui {
class Form;
}

class Form : public QMainWindow {
    Q_OBJECT

public:
    explicit Form(QMainWindow *parent = 0);
    ~Form();

private:
    Ui::Form *ui;
    printScene *scene;

private slots:
    void clickedButton();

};
#endif // FORM_H
