////////////////////////////////////////////////////////////////////////////////
//     This file is part of RTMaps                                            //
//     Copyright (c) Intempora S.A. All rights reserved.                      //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <maps.hpp>
#include "rpigpio.h"
#include <string>

#define ComponentClassInput   maps_GpioInputComponent
#define ComponentNameInput    "RPIGpioInputComponent"

#define ComponentClassOutput   maps_GpioOutputComponent
#define ComponentNameOutput    "RPIGpioOutputComponent"

#define ComponentClassPWM  maps_GpioPWMComponent
#define ComponentNamePWM    "RPIGpioPWMComponent"


#define ComponentVersion "1.0.0"

class ComponentClassInput : public MAPSComponent
{
	MAPS_COMPONENT_STANDARD_HEADER_CODE(ComponentClassInput)
		
	gpio_chip * chip;

    void Dynamic() override;
};

class ComponentClassOutput : public MAPSComponent
{
	MAPS_COMPONENT_STANDARD_HEADER_CODE(ComponentClassOutput)
		
	gpio_chip * chip;

	void Dynamic() override;
};

class ComponentClassPWM : public MAPSComponent
{
	MAPS_COMPONENT_STANDARD_HEADER_CODE(ComponentClassPWM)
		
	std::string Pwm0Path = "/sys/class/pwm/pwmchip0/pwm0";
	std::string Pwm1Path = "/sys/class/pwm/pwmchip0/pwm1";
		

	MAPSInput*	m_inputs[2];

	int			m_period;
	MAPSFloat64			m_duty;
	
	char buf[150];

	
	
	int enable_fd;
	int period_fd;
	int duty_fd;
	int polarity_fd;
	
	void Dynamic() override;
};
