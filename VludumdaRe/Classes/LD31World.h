//
//  LD31_World.h
//  VludumdaRe
//
//  Created by Nils Daumann on 06.12.14.
//  Copyright (c) 2014 Uberpixel. All rights reserved.
//

#ifndef __LD31_WORLD_H__
#define __LD31_WORLD_H__

#include <Rayne/Rayne.h>
#include <ROHMD.h>
#include <ROCamera.h>
#include <deque>
#include <RBPhysicsWorld.h>
#include <RBRigidBody.h>
#include "LD31Racket.h"
#include "LD31Ball.h"
#include "LD31Opponent.h"

namespace LD31
{
	class World : public RN::World
	{
	public:
		World();
		~World() override;
		
		void SetHMD(RO::HMD *hmd, bool shadows);
		
		void LoadOnThread(RN::Thread *thread, RN::Deserializer *deserializer) override;
		void Update(float delta) override;
		Ball *GetBall() const;
		
	private:
		RN::SceneNode *_camera;
		Racket *_racket;
		Ball *_ball;
		Opponent *_opponent;
		
		int _triggerCount;
		RO::HMD *_hmd;
		RN::Vector3 _positionOffset;
		
		bool _shadows;
	};
}

#endif /* __LD31_WORLD_H__ */
