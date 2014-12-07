//
//  LD31Opponent.cpp
//  VludumdaRe
//
//  Created by Nils Daumann on 07.12.14.
//  Copyright (c) 2014 Uberpixel. All rights reserved.
//

#include "LD31Opponent.h"
#include "LD31World.h"

namespace LD31
{
	Opponent::Opponent() : _active(true)
	{
		SetModel(RN::Model::WithFile("assets/blob/blob.sgm"));
		SetScale(RN::Vector3(0.8f));
	}
	
	Opponent::~Opponent()
	{
		
	}
	
	void Opponent::SetStartPosition(const RN::Vector3 &pos)
	{
		_startPosition = pos;
		SetPosition(pos);
	}
	
	void Opponent::Update(float delta)
	{
		Ball *ball = RN::World::GetActiveWorld()->Downcast<LD31::World>()->GetBall();
		
		if(ball && !ball->IsGrabbed() && ball->GetPosition().GetLength() > 4.0f)
		{
			if(_active)
			{
				RN::Vector3 targetPosition = ball->GetPosition();
				RN::bullet::RigidBody *ballBody = ball->GetAttachment<RN::bullet::RigidBody>();
			
				float timeToGround = -ballBody->GetLinearVelocity().y/9.81+sqrt((ballBody->GetLinearVelocity().y/9.81)*(ballBody->GetLinearVelocity().y/9.81) + targetPosition.y/(0.5*9.81));
				targetPosition += ballBody->GetLinearVelocity()*timeToGround*0.1f;
				targetPosition.y = 0.0f;
				SetPosition(targetPosition);
				
				if((ball->GetPosition()-GetPosition()).GetLength() < 2.0f)
				{
					_active = false;
					RN::Vector3 diffToPlayer = -GetPosition();
					ballBody->SetLinearVelocity(diffToPlayer.GetNormalized()*diffToPlayer.GetLength()*0.7f+RN::Vector3(0.0f, 9.81f*0.8f, 0.0f));
				}
			}
		}
		else
		{
			_active = true;
		}
	}
}
