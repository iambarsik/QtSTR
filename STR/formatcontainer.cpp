#include "formatcontainer.h"
#include "ui_formatcontainer.h"
#include <QMessageBox>

formatcontainer::formatcontainer(QList<STRFormat<STRformat_enum>> formatList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::formatcontainer)
{
    this->formatList = formatList;
    ui->setupUi(this);
    this->setWindowTitle("Контейнер форматов");
    initData();    
    this->setWindowIcon(QIcon(":/images/ico.ico"));
}

formatcontainer::~formatcontainer()
{
    delete ui;
}

void formatcontainer::initData()
{
    if(formatList.isEmpty())    {
        qDebug() << "FCNT :: format list for container is empty";
        return;
    }

    for(int f = 0; f < formatList.size(); f++)  {

        if(FormContainer.isEmpty()) {
            FormatItem fItem;
            fItem.title = formatList[f].title;
            fItem.name = formatList[f].name;
            SystemItem sItem;
            sItem.title = formatList[f].system;
            sItem.formList.push_back(fItem);
            ObjectItem oItem;
            oItem.title = formatList[f].object;
            oItem.sysList.push_back(sItem);
            FormContainer.push_back(oItem);
            continue;
        }
        bool bFoundObject = false;
        for(int c = 0; c < FormContainer.size(); c++)  {
            if(formatList[f].object == FormContainer[c].title)  {
                bFoundObject = true;
                bool bFoundSystem = false;
                for(int s = 0; s < FormContainer[c].sysList.size(); s++)  {
                    if(formatList[f].system == FormContainer[c].sysList[s].title)   {
                        bFoundSystem = true;
                        FormContainer[c].sysList[s].formList.push_back({formatList[f].title, formatList[f].name});
                    }
                }
                if(bFoundSystem == false)   {
                    FormatItem fItem;
                    fItem.title = formatList[f].title;
                    fItem.name = formatList[f].name;
                    SystemItem sItem;
                    sItem.title = formatList[f].system;
                    sItem.formList.push_back(fItem);
                    FormContainer[c].sysList.push_back(sItem);
                }
            }
        }
        if(bFoundObject == false)   {
            FormatItem fItem;
            fItem.title = formatList[f].title;
            fItem.name = formatList[f].name;
            SystemItem sItem;
            sItem.title = formatList[f].system;
            sItem.formList.push_back(fItem);
            ObjectItem oItem;
            oItem.title = formatList[f].object;
            oItem.sysList.push_back(sItem);
            FormContainer.push_back(oItem);
            continue;
        }
    }

    /*
    // sort if needs
    for(int c = 0; c < FormContainer.size(); c++)  {
        std::sort(FormContainer.begin(),FormContainer.end(),[](const ObjectItem &v1, const ObjectItem &v2){
           return v1.title < v2.title;
        });
    }
    */

    // Debug output
    for(int c = 0; c < FormContainer.size(); c++)  {
        qDebug() << "Object name : " << FormContainer[c].title;
        for(int s = 0; s < FormContainer[c].sysList.size(); s++)  {
            qDebug() << "\tSystem name : " << FormContainer[c].sysList[s].title;
            for(int f = 0; f < FormContainer[c].sysList[s].formList.size(); f++)  {
                qDebug() << "\t\tFormat name : " << FormContainer[c].sysList[s].formList[f].title;
            }
        }
    }

    ui->cbObject->clear();

    for(int c = 0; c < FormContainer.size(); c++)  {
        ui->cbObject->addItem(FormContainer[c].title);
    }
    ui->cbObject->setCurrentIndex(0);
    updateSystems();
}

void formatcontainer::updateSystems()
{
    ui->listSystem->clear();
    int object = ui->cbObject->currentIndex();
    for(int i = 0; i < FormContainer[object].sysList.size(); i++)    {
        ui->listSystem->addItem(FormContainer[object].sysList[i].title);
    }
    if(ui->listSystem->count() > 0)
        ui->listSystem->setCurrentRow(0);
    updateFormats();
}

void formatcontainer::updateFormats()
{
    ui->listFormats->clear();
    int object = ui->cbObject->currentIndex();
    int system = ui->listSystem->currentRow();
    if(system < 0)  {
        if(ui->listSystem->count() > 0)  {
            system = 0;
        } else {
            return;
        }
    }
    for(int i = 0; i < FormContainer[object].sysList[system].formList.size(); i++)    {
        ui->listFormats->addItem(FormContainer[object].sysList[system].formList[i].title);
    }
    if(ui->listFormats->count() > 0)
        ui->listFormats->setCurrentRow(0);
}

void formatcontainer::on_cbObject_currentIndexChanged(int index)
{
    updateSystems();
    Q_UNUSED(index);
}

void formatcontainer::on_listSystem_currentRowChanged(int currentRow)
{
    updateFormats();
    Q_UNUSED(currentRow);
}

void formatcontainer::on_bCancel_clicked()
{
    this->close();
}

void formatcontainer::on_bOpen_clicked()
{
    int object = ui->cbObject->currentIndex();
    int system = ui->listSystem->currentRow();
    int format = ui->listFormats->currentRow();
    if(object < 0 || system < 0 || format < 0)  {
        QMessageBox::warning(this, "Ошибка!", "Не выбран ни один формат!", QMessageBox::Yes);
        return;

    }
    STRformat_enum name = FormContainer[object].sysList[system].formList[format].name;
    emit signalOpenFormat(name);
}
