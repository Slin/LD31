//
//  LD31Opponent.h
//  VludumdaRe
//
//  Created by Nils Daumann on 07.12.14.
//  Copyright (c) 2014 Uberpixel. All rights reserved.
//

#ifndef __VludumdaRe__LD31Opponent__
#define __VludumdaRe__LD31Opponent__

namespace LD31
{
	class Opponent : public RN::Entity
	{
	public:
		Opponent();
		~Opponent();
		
		void SetStartPosition(const RN::Vector3 &pos);
		void Update(float delta) override;
		
	private:
		RN::Vector3 _startPosition;
		bool _active;
	};
}

#endif /* defined(__VludumdaRe__LD31Opponent__) */
