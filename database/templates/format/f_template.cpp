#include "<GEN_FORMAT_NAME>.h"
#include "ui_<GEN_FORMAT_NAME>.h"

<GEN_FORMAT_NAME>::<GEN_FORMAT_NAME>(CoreQ * core, QWidget *parent)
    : FormatQ(core, parent)
    , ui(new Ui::F_TEST)
{
    ui->setupUi(this);
}

<GEN_FORMAT_NAME>::~<GEN_FORMAT_NAME>()
{
    delete ui;
}

void <GEN_FORMAT_NAME>::update()
{
	// write code here
	
	
	
	
}
