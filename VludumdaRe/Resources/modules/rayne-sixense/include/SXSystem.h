//
//  SXSystem.h
//  rayne-sixense
//
//  Created by Nils Daumann on 06.12.14.
//  Copyright (c) 2014 Uberpixel. All rights reserved.
//

#ifndef __rayne_sixense__SXSystem__
#define __rayne_sixense__SXSystem__

#include <Rayne/Rayne.h>

#include <sixense.h>
#include <sixense_math.hpp>
#include <sixense_utils/derivatives.hpp>
#include <sixense_utils/button_states.hpp>
#include <sixense_utils/event_triggers.hpp>
#include <sixense_utils/controller_manager/controller_manager.hpp>

namespace SX
{
	class Controller;
	
	class System : public RN::ISingleton<System>
	{
	public:
		System();
		~System() override;
		
		void Update();
		Controller *GetController(int base, int id) const;
		
	private:
		std::vector<Controller*> _controllers;
		
		RNDeclareSingleton(System)
	};
}

#endif /* defined(__rayne_sixense__SXSystem__) */
