//
//  rn_Texture1.vsh
//  Rayne
//
//  Copyright 2014 by Überpixel. All rights reserved.
//  Unauthorized use is punishable by torture, mutilation, and vivisection.
//

#version 400
precision highp float;

#include <shader/rn_Matrices.vsh>
#include <shader/rn_Shadow.vsh>

in vec3 attPosition;
in vec3 attNormal;
in vec2 attTexcoord0;

out vec3 vertPosition;
out vec2 vertTexcoord;

out vec3 vertNormal;

void main()
{
	vertTexcoord = vec2(attTexcoord0);
	vertPosition = (matModel * vec4(attPosition, 1.0)).xyz;
	vertNormal = (matModel * vec4(attNormal, 0.0)).xyz;
	gl_Position = matProjViewModel * vec4(attPosition, 1.0);

	#if defined(RN_LIGHTING)
	#if defined(RN_DIRECTIONAL_SHADOWS)
		rn_ShadowDirectional0(vec4(vertPosition, 1.0));
	#endif
	#if defined(RN_SPOT_SHADOWS)
		rn_ShadowSpot(vec4(vertPosition, 1.0));
	#endif
	#endif
}
