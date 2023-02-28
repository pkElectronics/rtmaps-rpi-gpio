////////////////////////////////////////////////////////////////////////////////
//     This file is part of RTMaps                                            //
//     Copyright (c) Intempora S.A. All rights reserved.                      //
////////////////////////////////////////////////////////////////////////////////

#include "maps_GpioComponent.hpp"

// Use the macros to declare the inputs
MAPS_BEGIN_INPUTS_DEFINITION(ComponentClassPWM)
    MAPS_INPUT("period", MAPS::FilterInteger32, MAPS::FifoReader)
    MAPS_INPUT("dutycycle", MAPS::FilterFloat64, MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

// Use the macros to declare the outputs
MAPS_BEGIN_OUTPUTS_DEFINITION(ComponentClassPWM)
MAPS_END_OUTPUTS_DEFINITION

// Use the macros to declare the properties
MAPS_BEGIN_PROPERTIES_DEFINITION(ComponentClassPWM)
    MAPS_PROPERTY_ENUM("PWMChannel", "0|1", 0, false, false)
    MAPS_PROPERTY_ENUM("PWMPolarity", "Normal|Inverted", 0, false, false)
MAPS_END_PROPERTIES_DEFINITION

// Use the macros to declare the actions
MAPS_BEGIN_ACTIONS_DEFINITION(ComponentClassPWM)
    //MAPS_ACTION("a_name", ComponentClass::ActionName)
MAPS_END_ACTIONS_DEFINITION

// Use the macros to declare this component (ComponentName) behaviour
MAPS_COMPONENT_DEFINITION(ComponentClassPWM,
	ComponentNamePWM,
	ComponentVersion,
	128,
	MAPS::Threaded,
	MAPS::Threaded, // @suppress("Invalid arguments")
	2,  // inputs
	-1,  // outputs
	2,  // properties
	-1)  // actions

	namespace
{
}


void ComponentClassPWM::Dynamic()
{
}

void ComponentClassPWM::Birth()
{
	MAPSUInt8 pwmchannel = GetIntegerProperty("PWMChannel");
	MAPSUInt8 polarity = GetIntegerProperty("PWMPolarity");
	
	//Initialize the _inputs member array.
	m_inputs[0] = &Input(0);
	m_inputs[1] = &Input(1);

	//Initialize the default values.
	m_duty = 0;
	m_period = 0;
	
	std::string pwmPath;
	
	if (pwmchannel == 0) pwmPath = Pwm0Path;
	else pwmPath = Pwm1Path;
	
	int fd;
	if ((fd = open("/sys/class/pwm/pwmchip0/export", O_WRONLY | O_SYNC | O_CLOEXEC)) >= 0)
	{
		int l = snprintf(buf, 150, "%d", pwmchannel);
		write(fd, buf, l); //enable the PWM channel
		close(fd);
		Rest(1000000); //wait for the kernel

		enable_fd = open((pwmPath + "/enable").c_str(), O_WRONLY | O_SYNC | O_CLOEXEC);
		polarity_fd = open((pwmPath + "/polarity").c_str(), O_WRONLY | O_SYNC | O_CLOEXEC);
		duty_fd = open((pwmPath + "/duty_cycle").c_str(), O_WRONLY | O_SYNC | O_CLOEXEC);
		period_fd = open((pwmPath + "/period").c_str(), O_WRONLY | O_SYNC | O_CLOEXEC);
		
		if (enable_fd == -1 || polarity_fd == -1 || duty_fd == -1 || period_fd == -1)
		{
			ReportError("Could not open one or more kernel files");
		}
		else
		{			
			if (!polarity) {
				int l = snprintf(buf, 150, "%s", "normal");
				write(polarity_fd, buf, l);
			}
			else{
				int l = snprintf(buf, 150, "%s", "inversed");
				write(polarity_fd, buf, l); 
			}
			write(enable_fd, "1", 1); //enable the channel

		}
		
	}
	else
	{
		ReportWarning("RaspberryPi PWM Subsystem is not enabled");
	}

	
}

void ComponentClassPWM::Core()
{
	int inputThatAnswered;
	int l = 0;
	MAPSFloat64 intermediate;

	MAPSIOElt* ioEltIn = StartReading(2, m_inputs, &inputThatAnswered);
	if (ioEltIn == NULL)
		return;

	MAPSTimestamp t = 0;
	switch (inputThatAnswered) {
	case 0: //We've received new data on the first input.
		m_period = ioEltIn->Integer32();
		
		l = snprintf(buf, 150, "%d", m_period);
		write(period_fd, buf, l); 
		
		t = ioEltIn->Timestamp();
		break;
		
	case 1: //We've received new data on the second input.
		m_duty = ioEltIn->Float64();
		intermediate = (MAPSFloat64) m_period;
		intermediate *= m_duty;
		intermediate /= 100.0;
		
		l = snprintf(buf, 150, "%.0f", intermediate);
		write(duty_fd, buf, l); 
		
		t = ioEltIn->Timestamp();
		break;
	default:
		Error("Unknown input.");
	}

	
	
}

void ComponentClassPWM::Death()
{
	write(enable_fd, "0", 1); //disable the channel

	MAPSUInt8 pwmchannel = GetIntegerProperty("PWMChannel");

	int fd;
	if ((fd = open("/sys/class/pwm/pwmchip0/unexport", O_WRONLY | O_SYNC | O_CLOEXEC)) >= 0)
	{
		int l = snprintf(buf, 150, "%d", pwmchannel);
		write(fd, buf, l); //enable the PWM channel
		close(fd);
	}
	
	close(enable_fd);
	close(duty_fd);
	close(period_fd);
	close(polarity_fd);
}
