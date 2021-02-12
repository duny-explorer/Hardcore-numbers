#include "form.h"
#include "ui_form.h"

Form::Form(QMainWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);

    scene = new printScene();
    scene->setSceneRect(0,0, 280, 280);
    scene->addRect(0, 0, 280, 280, QPen(Qt::NoPen), QBrush(Qt::black));
    ui->canvas->setScene(scene);

    connect(ui->push, SIGNAL(clicked()), this, SLOT(clickedButton()));
}

Form::~Form()
{
    delete ui;
}

void Form::clickedButton() {
    ui -> result -> setText("YES");
    QImage image(scene->width(), scene->height(), QImage::Format_Mono);
    image.fill(1);
    QPainter painter(&image);
    scene->render(&painter);
    image.save("D:/result.jpg");

    scene = new printScene();
    scene->setSceneRect(0,0, 280, 280);
    scene->addRect(0, 0, 280, 280, QPen(Qt::NoPen), QBrush(Qt::black));
    ui->canvas->setScene(scene);
}
