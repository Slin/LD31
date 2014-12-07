//
//  rn_Texture1.fsh
//  Rayne
//
//  Copyright 2014 by Überpixel. All rights reserved.
//  Unauthorized use is punishable by torture, mutilation, and vivisection.
//

#version 400

#include <shader/rn_Lighting.fsh>

precision highp float;

uniform sampler2D mTexture0;
uniform sampler2D mTexture1;

#if defined(RN_FOG)
uniform vec2 fogPlanes;
uniform vec4 fogColor;
#endif

in vec2 vertTexcoord;
in vec3 vertPosition;
in vec3 vertNormal;

out vec4 fragColor0;

void main()
{
	vec4 color0 = texture(mTexture0, vertTexcoord.xy);
	vec4 color1 = texture(mTexture1, vertPosition.xz);
	
	vec3 normal = normalize(vertNormal);
	vec4 color = color0*color1*color1;
	
	rn_Lighting(color, vec4(0.0), normal, vertPosition);
	
#if defined(RN_FOG)
	float camdist = max(min((length(vertPosition-viewPosition)-fogPlanes.x)*fogPlanes.y, 1.0), 0.0);
	color = mix(color, fogColor, camdist);
#endif
	
	fragColor0 = color;
}
