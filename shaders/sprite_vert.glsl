#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

out vec2 texCoord;

uniform vec2 uPosition;
uniform vec2 uScale;
uniform float uRotation;
uniform vec2 uPivot;

uniform float uScaleDir;
uniform float uScalePerp;

uniform vec2 uDirection;

void main()
{
	float cosAngle = cos(uRotation);
	float sinAngle = sin(uRotation);
	mat2 rotation = mat2(cosAngle, -sinAngle,
						 sinAngle, cosAngle);
	vec2 dir = normalize(uDirection);

	// Find a perpendicular vector to create a basis
	vec2 perp = vec2(-dir.y, dir.x);

	// Basis matrix constructed from direction and perpendicular vectors
	mat2 basis = mat2(dir.x, perp.x,
					  dir.y, perp.y);

	vec2 centeredPos = aPos.xy - uPivot;
		
	// Now apply scaling around the pivot
	vec2 scaledPos = centeredPos * uScale;

	// Apply rotation around the pivot
	vec2 rotatedPos = rotation * scaledPos;

	vec2 localPos = transpose(basis) * rotatedPos;
	localPos.x *= uScaleDir;
	localPos.y *= uScalePerp;

	vec2 transformedPos = basis * localPos;

	// Move back to the original pivot and then apply the final position offset
	vec2 finalPos = transformedPos + uPivot + uPosition;

	gl_Position = vec4(finalPos, aPos.z, 1.0);
	texCoord = aTex;
}