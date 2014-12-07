//
//  LD31_World.cpp
//  VludumdaRe
//
//  Created by Nils Daumann on 06.12.14.
//  Copyright (c) 2014 Uberpixel. All rights reserved.
//

#include "LD31World.h"
#include "LD31FullscreenEffects.h"

#include <SXSystem.h>
#include <SXController.h>

namespace LD31
{
	World::World() :
		RN::World("GenericSceneManager"), _hmd(nullptr), _ball(nullptr), _triggerCount(0), _activeBall(nullptr)
	{
		RN::bullet::PhysicsWorld *attachment = new RN::bullet::PhysicsWorld();
		attachment->SetStepSize(1.0/240.0, 200);
		btContactSolverInfo &solverInfo = attachment->GetBulletDynamicsWorld()->getSolverInfo();
		solverInfo.m_numIterations = 100;
		AddAttachment(attachment);
	}
	
	World::~World()
	{}
	
	void World::SetHMD(RO::HMD *hmd)
	{
		if(_hmd)
			return;
		
		_hmd = hmd;
	}
	
	void World::LoadOnThread(RN::Thread *thread, RN::Deserializer *deserializer)
	{
		if(_hmd)
		{
			_camera = new RO::Camera(RN::Texture::Format::RGB16F);
			RO::Camera *tempCamera = static_cast<RO::Camera *>(_camera);
			tempCamera->SetHMD(_hmd);
			
			FullscreenEffects::GetSharedInstance()->CreateBloomPipeline(tempCamera->GetLeftCamera());
			FullscreenEffects::GetSharedInstance()->CreateGammaPipeline(tempCamera->GetLeftCamera());
			FullscreenEffects::GetSharedInstance()->CreateBloomPipeline(tempCamera->GetRightCamera());
			FullscreenEffects::GetSharedInstance()->CreateGammaPipeline(tempCamera->GetRightCamera());
		}
		else
		{
			_camera = new RN::Camera(RN::Vector2(), RN::Texture::Format::RGB16F);
			RN::Camera *tempCamera = static_cast<RN::Camera *>(_camera);
			tempCamera->SetBlitShader(RN::Shader::WithFile("shader/rn_DrawFramebufferTonemap"));
			
			FullscreenEffects::GetSharedInstance()->CreateBloomPipeline(tempCamera);
		}
		_camera->SetFlags(_camera->GetFlags()|RN::SceneNode::Flags::NoSave);
		_camera->SetPosition(RN::Vector3(0.0f, 1.0f, 0.0f));
		
		RN::Renderer::GetSharedInstance()->SetHDRExposure(1.0f);
		RN::Renderer::GetSharedInstance()->SetHDRWhitePoint(2.5f);
		
		RN::Entity *levelEnt = new RN::Entity(RN::Model::WithFile("assets/ground/beachrayne.sgm"));
		RN::bullet::TriangleMeshShape *shape = new RN::bullet::TriangleMeshShape(levelEnt->GetModel());
		RN::bullet::RigidBody *body = new RN::bullet::RigidBody(shape, 0.0f);
		levelEnt->AddAttachment(body);
		
		_hand = new RN::Entity(RN::Model::WithFile("assets/racket/racket-texturedrayne.sgm"));
		_hand->GetModel()->GetMaterialAtIndex(0, 0)->SetDiscard(true);
		RN::bullet::CompoundShape *racketshape = new RN::bullet::CompoundShape();
		racketshape->Autorelease();
		racketshape->AddChild(RN::bullet::CylinderShape::WithHalfExtents(RN::Vector3(0.3, 0.02, 0.3)), RN::Vector3(0.0f, 0.0f, -0.6f), RN::Vector3(0.0f, 0.0f, 0.0f));
		
		RN::bullet::RigidBody *racketBodyTemp = RN::bullet::RigidBody::WithShape(racketshape, 0.1f);
		_hand->AddAttachment(racketBodyTemp);
		racketBodyTemp->GetBulletRigidBody()->forceActivationState(DISABLE_DEACTIVATION);
		
		_handGhost = new RN::SceneNode();
		_racketBody = RN::bullet::RigidBody::WithShape(RN::bullet::SphereShape::WithRadius(0.001f), 0.0f);
		_handGhost->AddAttachment(_racketBody);
		btTransform noOffset;
		noOffset.setIdentity();
		btFixedConstraint *constraint = new btFixedConstraint(*racketBodyTemp->GetBulletRigidBody(), *_racketBody->GetBulletRigidBody(), noOffset, noOffset);
		RN::bullet::PhysicsWorld::GetSharedInstance()->GetBulletDynamicsWorld()->addConstraint(constraint);
	}

	void World::Update(float delta)
	{
		SX::System::GetSharedInstance()->Update();
		SX::Controller *controller = SX::System::GetSharedInstance()->GetController(0, 0);
		RN::Vector3 position = controller->GetPosition();
		RN::Quaternion rotation = controller->GetRotation();
		rotation *= RN::Vector3(0.0f, 45.0f, 0.0f);
		position.y += 1.0f;
		position += _positionOffset;
		
		_handGhost->SetPosition(position);
		_handGhost->SetRotation(rotation);
		
		if(_hmd && controller->GetButtonJustPressed(SIXENSE_BUTTON_START))
		{
			RN::Vector3 headPosition = _camera->Downcast<RO::Camera>()->GetHead()->GetPosition();
			_positionOffset = headPosition-controller->GetPosition();
			_positionOffset += RN::Vector3(0.11f, 0.0f, 0.0f);
		}
		
		SX::Controller *ballcontroller = SX::System::GetSharedInstance()->GetController(0, 1);
		_triggerCount += 1;
		
		if(ballcontroller->GetTriggerJustPressed())
		{
			_triggerCount = 0;
			
			if(!_ball)
			{
				while(!_ballSpeeds.empty())
				{
					_ballSpeeds.pop_front();
				}
				
				if(_activeBall)
				{
					_activeBall->RemoveFromWorld();
					_activeBall->Release();
					_activeBall = nullptr;
				}
				_ball = new RN::Entity(RN::Model::WithFile("assets/ball/ball.sgm"));
			}
		}
		else if(_ball && _triggerCount > 2)
		{
			RN::bullet::Shape *ballshape = RN::bullet::SphereShape::WithRadius(0.05f);
			RN::bullet::RigidBody *body = RN::bullet::RigidBody::WithShape(ballshape, 0.1f);
			_ball->AddAttachment(body);
			body->SetCCDSweptSphereRadius(0.1f);
			body->SetCCDMotionThreshold(0.00001f);
			
			RN::Vector3 velocity;
			for(RN::Vector3 speed : _ballSpeeds)
			{
				velocity += speed;
			}
			velocity /= _ballSpeeds.size();
			body->SetLinearVelocity(velocity/0.7f);
			
			_activeBall = _ball;
			_ball = nullptr;
		}
		
		if(_ball)
		{
			position = ballcontroller->GetPosition();
			rotation = ballcontroller->GetRotation();
			position.y += 1.0f;
			position += _positionOffset;
			
			if(_ballSpeeds.size() > 10.0f)
				_ballSpeeds.pop_front();
			_ballSpeeds.push_back((position - _ball->GetPosition())/delta);
		
			_ball->SetPosition(position);
			_ball->SetRotation(rotation);
		}
	}
}
