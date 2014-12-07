//
//  SXController.h
//  rayne-sixense
//
//  Created by Nils Daumann on 06.12.14.
//  Copyright (c) 2014 Uberpixel. All rights reserved.
//

#ifndef __rayne_sixense__SXController__
#define __rayne_sixense__SXController__

#include "SXSystem.h"

namespace SX
{
	class Controller
	{
	public:
		friend System;
		
		Controller(int base, int id);
		~Controller();
		
		const RN::Quaternion &GetRotation() const;
		const RN::Vector3 &GetPosition() const;
		
		int GetID() const;
		int GetBase() const;
		bool GetButtonJustPressed(uint16 button);
		bool GetTriggerJustPressed();
		bool GetTriggerJustReleased();
		
	private:
		void Update(sixenseControllerData &controllerData);
		
		sixenseControllerData _controllerData;
		
		int _id;
		int _base;
		bool _enabled;
		bool _alive;
		RN::Quaternion _rotation;
		RN::Vector3 _position;
	};
}

#endif /* defined(__rayne_sixense__SXController__) */
