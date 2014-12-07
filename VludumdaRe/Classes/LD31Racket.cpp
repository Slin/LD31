//
//  LD31Racket.cpp
//  VludumdaRe
//
//  Created by Nils Daumann on 07.12.14.
//  Copyright (c) 2014 Uberpixel. All rights reserved.
//

#include "LD31Racket.h"
#include <RBRigidBody.h>
#include <RBPhysicsWorld.h>

namespace LD31
{
	Racket::Racket()
	{
		RN::Entity *racketEnt = new RN::Entity(RN::Model::WithFile("assets/racket/racket-texturedrayne.sgm"));
		racketEnt->GetModel()->GetMaterialAtIndex(0, 0)->SetDiscard(true);
		RN::bullet::CompoundShape *racketshape = new RN::bullet::CompoundShape();
		racketshape->Autorelease();
		racketshape->AddChild(RN::bullet::CylinderShape::WithHalfExtents(RN::Vector3(0.3, 0.02, 0.3)), RN::Vector3(0.0f, 0.0f, -0.6f), RN::Vector3(0.0f, 0.0f, 0.0f));
		
		RN::bullet::RigidBody *racketBodyTemp = RN::bullet::RigidBody::WithShape(racketshape, 0.1f);
		racketEnt->AddAttachment(racketBodyTemp);
		racketBodyTemp->GetBulletRigidBody()->forceActivationState(DISABLE_DEACTIVATION);
		
		RN::bullet::RigidBody *ghostBody = RN::bullet::RigidBody::WithShape(RN::bullet::SphereShape::WithRadius(0.001f), 0.0f);
		AddAttachment(ghostBody);
		btTransform noOffset;
		noOffset.setIdentity();
		btFixedConstraint *constraint = new btFixedConstraint(*racketBodyTemp->GetBulletRigidBody(), *ghostBody->GetBulletRigidBody(), noOffset, noOffset);
		RN::bullet::PhysicsWorld::GetSharedInstance()->GetBulletDynamicsWorld()->addConstraint(constraint);
	}
	
	Racket::~Racket()
	{
		
	}
}