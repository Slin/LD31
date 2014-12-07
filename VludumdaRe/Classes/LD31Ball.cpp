//
//  LD31Ball.cpp
//  VludumdaRe
//
//  Created by Nils Daumann on 07.12.14.
//  Copyright (c) 2014 Uberpixel. All rights reserved.
//

#include "LD31Ball.h"
#include <RBRigidBody.h>

namespace LD31
{
	Ball::Ball() : _isGrabbed(true)
	{
		static std::once_flag onceFlagModel;
		static RN::Model *ballModel;
		
		std::call_once(onceFlagModel, []() {
			ballModel = RN::Model::WithFile("assets/ball/ball.sgm");
			ballModel->Retain();
		});
		
		SetModel(ballModel);
	}
	
	Ball::~Ball()
	{
		
	}
	
	bool Ball::IsGrabbed() const
	{
		return _isGrabbed;
	}
	
	void Ball::Throw()
	{
		_isGrabbed = false;
		
		RN::bullet::Shape *ballshape = RN::bullet::SphereShape::WithRadius(0.05f);
		RN::bullet::RigidBody *body = RN::bullet::RigidBody::WithShape(ballshape, 0.1f);
		AddAttachment(body);
		body->SetCCDSweptSphereRadius(0.1f);
		body->SetCCDMotionThreshold(0.00001f);
		
		RN::Vector3 velocity;
		if(_speeds.size() > 0)
		{
			for(RN::Vector3 speed : _speeds)
			{
				velocity += speed;
			}
			velocity /= _speeds.size();
		}
		body->SetLinearVelocity(velocity/0.6f);
	}
	
	void Ball::SetPosition(const RN::Vector3 &pos, float delta)
	{
		if(_speeds.size() > 10.0f)
			_speeds.pop_front();
		_speeds.push_back((pos - GetPosition())/delta);
		
		RN::Entity::SetPosition(pos);
	}
}
