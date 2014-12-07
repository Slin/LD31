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
		RN::World("GenericSceneManager"), _hmd(nullptr), _ball(nullptr), _triggerCount(0)
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
		RN::Model *sky = RN::Model::WithSkyCube("textures/sky_up.png", "textures/sky_down.png", "textures/sky_left.png", "textures/sky_right.png", "textures/sky_front.png", "textures/sky_back.png");
		for(int i = 0; i < sky->GetMeshCount(0); i++)
		{
			sky->GetMaterialAtIndex(0, i)->SetAmbientColor(RN::Color(3.0f, 3.0f, 3.0f, 1.0f));
		}
		
		RN::ShadowParameter shadowParam;
		if(_hmd)
		{
			_camera = new RO::Camera(RN::Texture::Format::RGB16F);
			RO::Camera *tempCamera = static_cast<RO::Camera *>(_camera);
			tempCamera->SetHMD(_hmd);
			tempCamera->GetLeftCamera()->SetSky(sky);
			tempCamera->GetRightCamera()->SetSky(sky);
			tempCamera->GetLeftCamera()->SetClipFar(200.0f);
			tempCamera->GetRightCamera()->SetClipFar(200.0f);
			
			//FullscreenEffects::GetSharedInstance()->CreateBloomPipeline(tempCamera->GetLeftCamera());
			FullscreenEffects::GetSharedInstance()->CreateGammaPipeline(tempCamera->GetLeftCamera());
			//FullscreenEffects::GetSharedInstance()->CreateBloomPipeline(tempCamera->GetRightCamera());
			FullscreenEffects::GetSharedInstance()->CreateGammaPipeline(tempCamera->GetRightCamera());
			
			shadowParam = RN::ShadowParameter(tempCamera->GetLeftCamera(), 512);
		}
		else
		{
			_camera = new RN::Camera(RN::Vector2(), RN::Texture::Format::RGB16F);
			RN::Camera *tempCamera = static_cast<RN::Camera *>(_camera);
			tempCamera->SetBlitShader(RN::Shader::WithFile("shader/rn_DrawFramebufferTonemap"));
			tempCamera->SetSky(sky);
			
			FullscreenEffects::GetSharedInstance()->CreateBloomPipeline(tempCamera);
			shadowParam = RN::ShadowParameter(tempCamera);
		}
		_camera->SetFlags(_camera->GetFlags()|RN::SceneNode::Flags::NoSave);
		_camera->SetPosition(RN::Vector3(0.0f, 1.0f, 0.0f));
		
		RN::Light *sun = new RN::Light(RN::Light::Type::DirectionalLight);
		sun->SetRotation(RN::Vector3(-45.0, -60.0, 0.0));
		sun->SetIntensity(1.5f);
		
		shadowParam.distanceBlendFactor = 0.01f;
		sun->ActivateShadows(shadowParam);
		
		RN::Renderer::GetSharedInstance()->SetHDRExposure(1.0f);
		RN::Renderer::GetSharedInstance()->SetHDRWhitePoint(2.5f);
		
		RN::Entity *levelEnt = new RN::Entity(RN::Model::WithFile("assets/ground/beachrayne.sgm"));
		RN::bullet::TriangleMeshShape *shape = new RN::bullet::TriangleMeshShape(levelEnt->GetModel());
		RN::bullet::RigidBody *body = new RN::bullet::RigidBody(shape, 0.0f);
		levelEnt->AddAttachment(body);
		
		_racket = new Racket();
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
		
		_racket->SetPosition(position);
		_racket->SetRotation(rotation);
		
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
			
			if(!_ball || !_ball->IsGrabbed())
			{
				if(_ball)
				{
					_ball->RemoveFromWorld();
					_ball->Release();
					_ball = nullptr;
				}
				_ball = new Ball();
				
				position = ballcontroller->GetPosition();
				rotation = ballcontroller->GetRotation();
				position.y += 1.0f;
				position += _positionOffset;
				_ball->RN::SceneNode::SetPosition(position);
			}
		}
		else if(_ball && _ball->IsGrabbed() && _triggerCount > 2)
		{
			_ball->Throw();
		}
		
		if(_ball && _ball->IsGrabbed())
		{
			position = ballcontroller->GetPosition();
			rotation = ballcontroller->GetRotation();
			position.y += 1.0f;
			position += _positionOffset;
		
			_ball->SetPosition(position, delta);
			_ball->SetRotation(rotation);
		}
	}
}
