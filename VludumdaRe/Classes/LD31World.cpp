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
		RN::World("GenericSceneManager"), _hmd(nullptr), _ball(nullptr), _triggerCount(0), _shadows(false)
	{
		RN::bullet::PhysicsWorld *attachment = new RN::bullet::PhysicsWorld();
		attachment->SetStepSize(1.0/240.0, 200);
		btContactSolverInfo &solverInfo = attachment->GetBulletDynamicsWorld()->getSolverInfo();
		solverInfo.m_numIterations = 50;
		AddAttachment(attachment);
	}
	
	World::~World()
	{}
	
	void World::SetHMD(RO::HMD *hmd, bool shadows)
	{
		_shadows = shadows;
		
		if(_hmd)
			return;
		
		_hmd = hmd;
	}
	
	Ball *World::GetBall() const
	{
		return _ball;
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
			_camera = new RO::Camera(_hmd, 1.0f, RN::Texture::Format::RGB16F);
			RO::Camera *tempCamera = static_cast<RO::Camera *>(_camera);
			tempCamera->SetSky(sky);
			tempCamera->SetClipFar(1000.0f);
			
			FullscreenEffects::GetSharedInstance()->CreateGammaPipeline(tempCamera->GetLeftCamera());
			FullscreenEffects::GetSharedInstance()->CreateGammaPipeline(tempCamera->GetRightCamera());
			
			shadowParam = RN::ShadowParameter(tempCamera->GetLeftCamera(), 1024);
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
		
		shadowParam.distanceBlendFactor = 0.05f;
		shadowParam.maxShadowDist = 0.08f;
		if(_shadows)
			sun->ActivateShadows(shadowParam);
		
		RN::Renderer::GetSharedInstance()->SetHDRExposure(1.0f);
		RN::Renderer::GetSharedInstance()->SetHDRWhitePoint(2.5f);
		
		RN::Entity *levelEnt = new RN::Entity(RN::Model::WithFile("assets/ground/beachrayne.sgm"));
		RN::bullet::TriangleMeshShape *shape = new RN::bullet::TriangleMeshShape(levelEnt->GetModel());
		RN::bullet::RigidBody *body = new RN::bullet::RigidBody(shape, 0.0f);
		levelEnt->AddAttachment(body);
		levelEnt->SetPosition(RN::Vector3(3.0f, 0.0f, 0.0f));
		levelEnt->GetModel()->GetMaterialAtIndex(0, 0)->SetBlending(true);
		levelEnt->GetModel()->GetMaterialAtIndex(0, 13)->SetShader(RN::Shader::WithFile("shaders/rn_Ground"));
		
		new RN::Entity(RN::Model::WithFile("assets/water/water.sgm"));
		
		_racket = new Racket();
		_opponent = new Opponent();
		_opponent->SetStartPosition(RN::Vector3(0.0f, 0.5, -18.0f));

		if(_hmd)
		{
			RN::Timer::ScheduledTimerWithDuration(std::chrono::seconds(6), [this] {
				_hmd->DismissSafetyWarning();
			}, false);
		}
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
		
		if(_hmd && ballcontroller->GetButtonJustPressed(SIXENSE_BUTTON_START))
		{
			RN::Vector3 headPosition = _camera->Downcast<RO::Camera>()->GetHead()->GetPosition();
			_positionOffset = headPosition-ballcontroller->GetPosition();
			_positionOffset += RN::Vector3(0.11f, 0.0f, 0.0f);
		}
		
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
