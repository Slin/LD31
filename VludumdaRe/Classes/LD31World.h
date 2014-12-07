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

namespace LD31
{
	class World : public RN::World
	{
	public:
		World();
		~World() override;
		
		void SetHMD(RO::HMD *hmd);
		
		void LoadOnThread(RN::Thread *thread, RN::Deserializer *deserializer) override;
		void Update(float delta) override;
		
	private:
		RN::SceneNode *_camera;
		RN::Entity *_hand;
		RN::SceneNode *_handGhost;
		RN::Entity *_ball;
		RN::Entity *_activeBall;
		RN::bullet::RigidBody *_racketBody;
		std::deque<RN::Vector3> _ballSpeeds;
		int _triggerCount;
		RO::HMD *_hmd;
		RN::Vector3 _positionOffset;
	};
}

#endif /* __LD31_WORLD_H__ */
