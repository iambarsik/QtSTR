#include "<GEN_MODEL_NAME>.h"

<GEN_MODEL_NAME>::<GEN_MODEL_NAME>(CoreQ * core)
    : ModelQ(core, MODEL_ID)
{

}

void <GEN_MODEL_NAME>::inputFromCore()
{
//<FROM_CORE_SECTION>	
//</FROM_CORE_SECTION>	
}

void <GEN_MODEL_NAME>::outputToCore()
{
//<TO_CORE_SECTION>	
//</TO_CORE_SECTION>	
}

void <GEN_MODEL_NAME>::init()
{
	// write code here
	
	
	
	
//<TO_CORE_METHOD>	
/*<::db_generated::>*/ outputToCore();
}

void <GEN_MODEL_NAME>::set()
{
    // write code here
	
	
	
	
//<TO_CORE_METHOD>
/*<::db_generated::>*/ outputToCore();    
}

void <GEN_MODEL_NAME>::update()
{
	// write code here
	
	
	
	
    
}

void <GEN_MODEL_NAME>::ReadCommand(command_t command)
{
//<COMMAND_SWITCH_SECTION>	
    switch(command.code)    {
//<COMMAND_SWITCH_TAG>	
        default:
            qDebug() << "Unknown command in model " << MODEL_NAME;
        break;
    }
    qDebug() << "Got command " << command.code << "in model " << MODEL_NAME;
//</COMMAND_SWITCH_SECTION>	
//<TO_CORE_METHOD>
/*<::db_generated::>*/ outputToCore();
}
