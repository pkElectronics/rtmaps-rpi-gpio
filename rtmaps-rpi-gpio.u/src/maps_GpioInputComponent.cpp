//////////////////////////////////////////////////////////////////////////////////////////
//     This file is part of the RTMaps RPI GPIO Package                                 //
//     Copyright (c) Pascal Wistinghausen. All rights belong to their respective owners //
//////////////////////////////////////////////////////////////////////////////////////////

#include "maps_GpioComponent.hpp"

// Use the macros to declare the inputs
MAPS_BEGIN_INPUTS_DEFINITION(ComponentClassInput)
MAPS_END_INPUTS_DEFINITION

// Use the macros to declare the outputs
MAPS_BEGIN_OUTPUTS_DEFINITION(ComponentClassInput)
    MAPS_OUTPUT("output_value", MAPS::Integer32, nullptr, nullptr, 1)
MAPS_END_OUTPUTS_DEFINITION

// Use the macros to declare the properties
MAPS_BEGIN_PROPERTIES_DEFINITION(ComponentClassInput)
    MAPS_PROPERTY_ENUM("GPIO","0|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|16|17|18|19|20|21|22|23|24|25|26|27|28|29|30|31|32|33|34|35|36|37|38|39|40|41|42|43|44|45|46|47|48|49|50|51|52|53|54|55|56|57", 0, false, false)
    MAPS_PROPERTY_ENUM("PINPull","UP|DOWN|NONE", 0, false, false)
MAPS_END_PROPERTIES_DEFINITION

// Use the macros to declare the actions
MAPS_BEGIN_ACTIONS_DEFINITION(ComponentClassInput)
    //MAPS_ACTION("a_name", ComponentClass::ActionName)
MAPS_END_ACTIONS_DEFINITION

// Use the macros to declare this component (ComponentName) behaviour
MAPS_COMPONENT_DEFINITION(ComponentClassInput, ComponentNameInput, ComponentVersion, 128, MAPS::Threaded, MAPS::Threaded, // @suppress("Invalid arguments")
    -1,  // inputs
    1,  // outputs
    2,  // properties
    -1)  // actions

namespace
{
}


void ComponentClassInput::Dynamic()
{
}

void ComponentClassInput::Birth()
{
	MAPSUInt8 gpio = GetIntegerProperty("GPIO");
	MAPSUInt8 pull = GetIntegerProperty("PINPull");
	
	unsigned int gpio_pull;
	
	switch (pull)
	{
	case 0:
		gpio_pull = PULL_UP;
		break;
		
	case 1:
		gpio_pull = PULL_DOWN;
		break;
		
	case 2:
	default:
		gpio_pull = PULL_NONE;
		break;
	}
	gpio_initialize();
	gpio_set(gpio, FUNC_IP, DRIVE_UNSET, gpio_pull);
}

void ComponentClassInput::Core()
{
	Rest(1000); //TODO Sample Rate einstellbar machen

	MAPSUInt8 gpio = GetIntegerProperty("GPIO");

	MAPSIOElt* ioEltOut = StartWriting(Output("output_value"));

	//Write the result in the MAPSIOElt object (MAPSIOElt::Integer32() returns a C++ reference to the contained integer).
	ioEltOut->Integer32() = gpio_get(gpio);

	//Transfer the timestamp (don't forget).
//	ioEltOut->Timestamp() = ioEltIn->Timestamp();

	//Writing in the ioEltOut object is complete.
	//The only thing left is to release it and make it available to downstream components :
	StopWriting(ioEltOut);

}

void ComponentClassInput::Death()
{
}
