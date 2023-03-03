#include "<GEN_FORMAT_NAME>.h"
#include "ui_<GEN_FORMAT_NAME>.h"

<GEN_FORMAT_NAME>::<GEN_FORMAT_NAME>(CoreQ * core, QWidget *parent)
    : FormatQ(core, FORMAT_ID, <GEN_FORMAT_INFO>, parent)
    , ui(new Ui::<GEN_FORMAT_NAME>)
{
    ui->setupUi(this);
	this->setWindowTitle("<GEN_FORMAT_TITLE>");
}

<GEN_FORMAT_NAME>::~<GEN_FORMAT_NAME>()
{
    delete ui;
}

void <GEN_FORMAT_NAME>::update()
{
	// write code here
	
	
	
	
}
