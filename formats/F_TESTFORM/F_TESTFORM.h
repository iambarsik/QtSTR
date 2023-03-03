#ifndef F_TESTFORM_H
#define F_TESTFORM_H

#define FORMAT_ID    1000

#include <QWidget>
#include <QTimer>

#include "../../templates/format/format.h"
#include "../../core/core.h"
#include "../../include/common.h"

QT_BEGIN_NAMESPACE
namespace Ui { class F_TESTFORM; }
QT_END_NAMESPACE

class Q_DECL_EXPORT F_TESTFORM : public FormatQ
{
    Q_OBJECT

public:
    F_TESTFORM(CoreQ * core, QWidget *parent = nullptr);
    ~F_TESTFORM();

        // overrided methods
    void update() override;

signals:


private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::F_TESTFORM *ui;
};
#endif // F_TEST_H
