//
//  ___VARIABLE_classPrefix:identifier___Application.h
//  VludumdaRe
//
//  Created by Nils Daumann on 06.12.14.
//  Copyright (c) 2014 Uberpixel. All rights reserved.
//

#ifndef _____VARIABLE_classPrefix:identifier____APPLICATION_H__
#define _____VARIABLE_classPrefix:identifier____APPLICATION_H__

#include <Rayne/Rayne.h>
#include "___VARIABLE_classPrefix:identifier___World.h"

namespace ___VARIABLE_classPrefix:identifier___
{
	class Application : public RN::Application
	{
	public:
		Application();
		~Application() override;
		
		void Start() override;
		void WillExit() override;
	};
}

#endif /* _____VARIABLE_classPrefix:identifier____APPLICATION_H__ */
