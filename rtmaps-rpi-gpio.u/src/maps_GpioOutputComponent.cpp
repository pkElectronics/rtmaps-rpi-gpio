////////////////////////////////////////////////////////////////////////////////
//     This file is part of RTMaps                                            //
//     Copyright (c) Intempora S.A. All rights reserved.                      //
////////////////////////////////////////////////////////////////////////////////

#include "maps_GpioComponent.hpp"

// Use the macros to declare the inputs
MAPS_BEGIN_INPUTS_DEFINITION(ComponentClassOutput)
    MAPS_INPUT("input_value", MAPS::FilterInteger32, MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

// Use the macros to declare the outputs
MAPS_BEGIN_OUTPUTS_DEFINITION(ComponentClassOutput)
MAPS_END_OUTPUTS_DEFINITION

// Use the macros to declare the properties
MAPS_BEGIN_PROPERTIES_DEFINITION(ComponentClassOutput)
    MAPS_PROPERTY_ENUM("GPIO", "0|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|16|17|18|19|20|21|22|23|24|25|26|27|28|29|30|31|32|33|34|35|36|37|38|39|40|41|42|43|44|45|46|47|48|49|50|51|52|53|54|55|56|57", 0, false, false)
MAPS_END_PROPERTIES_DEFINITION

// Use the macros to declare the actions
MAPS_BEGIN_ACTIONS_DEFINITION(ComponentClassOutput)
    //MAPS_ACTION("a_name", ComponentClass::ActionName)
MAPS_END_ACTIONS_DEFINITION

// Use the macros to declare this component (ComponentName) behaviour
MAPS_COMPONENT_DEFINITION(ComponentClassOutput,
	ComponentNameOutput,
	ComponentVersion,
	128,
	MAPS::Threaded,
	MAPS::Threaded, // @suppress("Invalid arguments")
	1,  // inputs
	-1,  // outputs
	1,  // properties
	-1)  // actions

	namespace
{
}


void ComponentClassOutput::Dynamic()
{
}

void ComponentClassOutput::Birth()
{
	MAPSUInt8 gpio = GetIntegerProperty("GPIO");
	
	unsigned int gpio_pull;
	gpio_initialize();
	gpio_set(gpio, FUNC_OP, DRIVE_LOW, PULL_UNSET);
}

void ComponentClassOutput::Core()
{
	MAPSIOElt* ioEltIn = StartReading(Input("input_value"));
	if (ioEltIn == NULL)
		return;

	//Get the integer contained in the MAPSIOElt object that has just been received.
	int valueIn = ioEltIn->Integer32(); 
	MAPSUInt8 gpio = GetIntegerProperty("GPIO");


	
	if (valueIn)
	{
		gpio_set(gpio, FUNC_OP, DRIVE_LOW, PULL_UNSET);
	}
	else
	{
		gpio_set(gpio, FUNC_OP, DRIVE_HIGH, PULL_UNSET);
	}
	
}

void ComponentClassOutput::Death()
{
}
