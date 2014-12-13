//
//  LD31_Application.cpp
//  VludumdaRe
//
//  Created by Nils Daumann on 06.12.14.
//  Copyright (c) 2014 Uberpixel. All rights reserved.
//

#include "LD31Application.h"
#include <SXSystem.h>
#include <ROSystem.h>

namespace LD31
{
	Application::Application()
	: _setupStep(0)
	{
		
	}
	
	Application::~Application()
	{
		
	}
	
	
	void Application::Start()
	{
		SetTitle("VludumdaRe");
		
		RN::Texture::SetDefaultAnisotropyLevel(16);
		
#ifdef WIN32
		RO::HMD *hmd = RO::System::GetSharedInstance()->GetHMD(0);
		if(hmd)
		{
			hmd->SetAsDisplay(false);
		}

		RN::World *world = new World();
		world->Downcast<LD31::World>()->SetHMD(hmd, true);
		RN::WorldCoordinator::GetSharedInstance()->LoadWorld(world->Autorelease());
#else
		RN::Vector2 screensize = RN::Window::GetSharedInstance()->GetSize();

		RN::UI::Widget *widget = new RN::UI::Widget(RN::UI::Widget::Style::Borderless);
		RN::UI::Label *text = new RN::UI::Label();
		text->SetText(RNCSTR("IMPORTANT:\nYour Oculus Rifts USB cable may not be connected yet!\nYour Hydra controllers need to be placed onto the base station and it has to be plugged in.\nThe Hydras cable connecions should be pointing away from you.\n\nPress space to continue."));
		text->SetLineBreak(RN::UI::LineBreakMode::WordWrapping);
		text->SetNumberOfLines(0);
		text->SetAlignment(RN::UI::TextAlignment::Center);
		text->SetFrame(RN::Rect(10.0f, 50.0f, screensize.x-20.0f, screensize.y-100.0f));
		widget->GetContentView()->AddSubview(text->Autorelease());
		widget->GetContentView()->SetBackgroundColor(RN::UI::Color::White());
		widget->Open();
		widget->SetFrame(RN::Rect(0.0f, 0.0f, screensize.x, screensize.y));
		widget->Autorelease();
		
		RN::MessageCenter::GetSharedInstance()->AddObserver(kRNInputEventMessage, [this, text, widget](RN::Message *message) {
			
			RN::Event *event = message->Downcast<RN::Event>();
			if(event->GetType() == RN::Event::Type::KeyDown)
			{
				char key = event->GetCharacter();
				bool shadows = false;
				switch(key)
				{
					case 's':
						shadows = true;
					case ' ':
						if(_setupStep == 0)
						{
							SX::System::GetSharedInstance();
							text->SetText(RNCSTR("Now plugin your Oculus Rift and wait until it is detected by your system!\nTo calibrate your hand position to your head position in game,\nmove any controller to your right ear and press its start button.\nThe controller without the racket, creates a ball when\nyou press its trigger and releases it if you release the trigger.\n\nPress space to continue.\nOr S to continue with shadows.\n\n\nIf your racket gets stuck in the ground, hold the controller up over your head and shake."));
						}
						else if(_setupStep == 1)
						{
							RO::HMD *hmd = RO::System::GetSharedInstance()->GetHMD(0);
							if(hmd)
							{
								hmd->SetAsDisplay(false);
							}
							
							widget->Close();
							RN::World *world = new World();
							world->Downcast<LD31::World>()->SetHMD(hmd, shadows);
							RN::WorldCoordinator::GetSharedInstance()->LoadWorld(world->Autorelease());
						}
						_setupStep += 1;
						break;
				}
			}
			
		}, this);
#endif
	}
	
	void Application::WillExit()
	{}
}
