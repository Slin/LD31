//
//  ___VARIABLE_classPrefix:identifier___World.h
//  VludumdaRe
//
//  Created by Nils Daumann on 06.12.14.
//  Copyright (c) 2014 Uberpixel. All rights reserved.
//

#ifndef _____VARIABLE_classPrefix:identifier____WORLD_H__
#define _____VARIABLE_classPrefix:identifier____WORLD_H__

#include <Rayne/Rayne.h>

namespace ___VARIABLE_classPrefix:identifier___
{
	class World : public RN::World
	{
	public:
		World();
		~World() override;
		
		void LoadOnThread(RN::Thread *thread, RN::Deserializer *deserializer) override;
		void Update(float delta) override;
		
	private:
		RN::Camera *_camera;
	};
}

#endif /* _____VARIABLE_classPrefix:identifier____WORLD_H__ */
