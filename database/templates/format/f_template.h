#ifndef <GEN_FORMAT_NAME>_H
#define <GEN_FORMAT_NAME>_H

//<FORMAT_ID_SECTION>

#include <QWidget>

#include "../../templates/format/format.h"
#include "../../core/core.h"
#include "../../include/common.h"

QT_BEGIN_NAMESPACE
namespace Ui { class <GEN_FORMAT_NAME>; }
QT_END_NAMESPACE

class Q_DECL_EXPORT <GEN_FORMAT_NAME> : public FormatQ
{
    Q_OBJECT

public:
    <GEN_FORMAT_NAME>(CoreQ * core, QWidget *parent = nullptr);
    ~<GEN_FORMAT_NAME>();

        // overrided methods
    void update() override;

private:
    Ui::<GEN_FORMAT_NAME> *ui;
	
	
	// write code here
signals:


public:


public slots:	


private:


private slots:
	
	
};
#endif // F_TEST_H
