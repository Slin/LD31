//
//  LD31Ball.h
//  VludumdaRe
//
//  Created by Nils Daumann on 07.12.14.
//  Copyright (c) 2014 Uberpixel. All rights reserved.
//

#ifndef __VludumdaRe__LD31Ball__
#define __VludumdaRe__LD31Ball__

namespace LD31
{
	class Ball : public RN::Entity
	{
	public:
		Ball();
		~Ball();
		
		bool IsGrabbed() const;
		void Throw();
		
		void SetPosition(const RN::Vector3 &pos, float delta);
		
	private:
		bool _isGrabbed;
		std::deque<RN::Vector3> _speeds;
	};
}

#endif /* defined(__VludumdaRe__LD31Ball__) */
