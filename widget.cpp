#include "widget.h"
#include <QPushButton>
#include <QFrame>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Creater Json");

        frame = new QFrame(this);
        frame->setFrameStyle(QFrame::Box | QFrame::Sunken);

        framelayout = new QVBoxLayout(frame);

        startButton = new QPushButton("Start");
        fileButton = new QPushButton("Open File");

        framelayout->addWidget(startButton);
        framelayout->addWidget(fileButton);

        layout = new QVBoxLayout(this);
        layout->addWidget(frame);

        setLayout(layout);

        connect(fileButton, &QPushButton::clicked, this, &Widget::selectFile);
        connect(startButton, &QPushButton::clicked, this, &Widget::startFile);

        setMinimumSize(300,100);
}

Widget::~Widget()
{
}

void Widget::selectFile()
{
    jsonFile = QFileDialog::getOpenFileName(this, "Выберите файл", "", "*.json");
}

void Widget::startFile()
{
    if (jsonFile.isEmpty()) {

           qDebug()<<" Ошибка! Выберите Json файл"; //
        } else {
            creatingFolders(jsonFile);
        }
}

void Widget::creatingFolders(const QString &fileJ)
{
    QFile file(fileJ);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug()<<" Ошибка! Выберите Json файл"<< file.errorString();
            return;
        }

    // Читаем
      QByteArray jsonData = file.readAll();
      file.close();

     // Парсим Json
       QJsonDocument doc = QJsonDocument::fromJson(jsonData);

       QJsonObject jsonObj = doc.object();

     // Извлекаем dataset_folder
       QString datasetFolder = jsonObj["dataset_folder"].toString();


       QDir dir(datasetFolder);
       if (dir.exists()) {
           dir.removeRecursively(); // Удаляем рекурсивно
       }
       dir.mkpath(datasetFolder);


       // Обработка вложенных папок
        QJsonArray includeFolders = jsonObj["include_folders"].toArray();
           for(const QJsonValue& folderValue : includeFolders){
               QJsonObject folderObj = folderValue.toObject();
               QString folderPath = datasetFolder + "/" + folderObj["folder_name"].toString();
               QDir().mkpath(folderPath);

               // Создание файлов в папке
                QJsonArray includeFiles = folderObj["include_files"].toArray();
                for(const QJsonValue& fileValue : includeFiles){
                   QFile newFile(folderPath + "/" + fileValue.toString());
                   newFile.open(QIODevice::WriteOnly);
                   newFile.close();
               }
           }


  QMessageBox::information(this, "Информация", "Создание файлов завершено!");


}

