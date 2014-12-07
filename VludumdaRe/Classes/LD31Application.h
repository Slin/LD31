//
//  LD31_Application.h
//  VludumdaRe
//
//  Created by Nils Daumann on 06.12.14.
//  Copyright (c) 2014 Uberpixel. All rights reserved.
//

#ifndef __LD31_APPLICATION_H__
#define __LD31_APPLICATION_H__

#include <Rayne/Rayne.h>
#include "LD31World.h"

namespace LD31
{
	class Application : public RN::Application
	{
	public:
		Application();
		~Application() override;
		
		void Start() override;
		void WillExit() override;
		
	private:
		int _setupStep;
	};
}

#endif /* LD31_APPLICATION_H__ */
