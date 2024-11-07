
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

out vec2 texCoord;

uniform vec2 uPosition;
uniform vec2 uScale;
uniform float uRotation;
uniform vec2 uPivot;

void main()
{
	float cosAngle = cos(uRotation);
	float sinAngle = sin(uRotation);
	mat2 rotation = mat2(cosAngle, -sinAngle,
						 sinAngle, cosAngle);

	vec2 scaledPos = aPos.xy * uScale;
	vec2 centeredPos = scaledPos - uPivot;

	vec2 rotatedPos = rotation * centeredPos;

	vec2 finalPos = rotatedPos + uPivot + uPosition;

	gl_Position = vec4(finalPos, aPos.z, 1.0);
	texCoord = aTex;
}
