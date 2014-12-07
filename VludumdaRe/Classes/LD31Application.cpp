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
		
		RN::UI::Widget *widget = new RN::UI::Widget(RN::UI::Widget::Style::Borderless);
		RN::UI::Label *text = new RN::UI::Label();
		text->SetText(RNCSTR("IMPORTANT: Your Oculud Rift needs to be plugged in on startup, UNPLUG your Oculus Rift USB cable now! Your Hydra controllers need to be placed onto the base station and it has to be plugged in.\n\nPress space to continue."));
		text->SetLineBreak(RN::UI::LineBreakMode::WordWrapping);
		text->SetNumberOfLines(0);
		text->SetFrame(RN::Rect(10.0f, 10.0f, 280.0f, 280.0f));
		widget->GetContentView()->AddSubview(text->Autorelease());
		widget->GetContentView()->SetBackgroundColor(RN::UI::Color::White());
		widget->Open();
		widget->SetFrame(RN::Rect(0.0f, 0.0f, 300.0f, 300.0f));
		widget->Autorelease();
		
		RN::MessageCenter::GetSharedInstance()->AddObserver(kRNInputEventMessage, [this, text, widget](RN::Message *message) {
			
			RN::Event *event = message->Downcast<RN::Event>();
			if(event->GetType() == RN::Event::Type::KeyDown)
			{
				char key = event->GetCharacter();
				switch(key)
				{
					case ' ':
						if(_setupStep == 0)
						{
							SX::System::GetSharedInstance();
							text->SetText(RNCSTR("Now plugin your Oculus Rift and wait until it is detected by your system or continue without!\n\nPress space to continue."));
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
							world->Downcast<LD31::World>()->SetHMD(hmd);
							RN::WorldCoordinator::GetSharedInstance()->LoadWorld(world->Autorelease());
						}
						_setupStep += 1;
						break;
				}
			}
			
		}, this);
	}
	
	void Application::WillExit()
	{}
}
