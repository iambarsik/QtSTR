#ifndef <GEN_MODEL_NAME>_H
#define <GEN_MODEL_NAME>_H

//<MODEL_ID_SECTION>
//</MODEL_ID_SECTION>

#include <QDebug>

#include "../../templates/model/model.h"
#include "../../core/core.h"
#include "../../include/common.h"

//<DEFINE_COMMANDS_SECTION>
//</DEFINE_COMMANDS_SECTION>

class Q_DECL_EXPORT <GEN_MODEL_NAME>: public ModelQ
{
public:
    <GEN_MODEL_NAME>(CoreQ * core);

        // overrided methods
    void inputFromCore() override;
    void outputToCore() override;
    void init() override;
    void set() override;
    void update() override;
    void ReadCommand(command_t command) override;
	
public:
	// write code here
	
	
	
private:
	// write code here
	
	
	

private:
//<VAR_SECTION>
//</VAR_SECTION>

};

#endif // <GEN_MODEL_NAME>_H