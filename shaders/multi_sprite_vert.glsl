
#version 330 core

// Instance attributes (per-instance data)
layout (location = 0) in vec2 iPosition;  // Instance position
layout (location = 1) in vec4 iUVRegion;  // Instance UV region (u1, v1, u2, v2)

// Uniform scaling factors (global scaling for all sprites)
uniform vec2 uUniformScale; // Separate scaling factors for x and y

out vec2 texCoord; // Output texture coordinate for the fragment shader

void main() {
	// Procedurally define quad vertex positions based on gl_VertexID
	vec2 quadVertices[4] = vec2[](
		vec2(-0.5, -0.5), // Bottom-left
		vec2( 0.5, -0.5), // Bottom-right
		vec2( 0.5,  0.5), // Top-right
		vec2(-0.5,  0.5)  // Top-left
	);
	vec2 localPos = quadVertices[gl_VertexID]; // Select vertex based on gl_VertexID

	// Apply global uniform scaling
	localPos *= uUniformScale;

	// Apply pivot and instance position
	vec2 finalPos = localPos + iPosition;

	// Output final vertex position in clip space
	gl_Position = vec4(finalPos, 0.0, 1.0);

	// Procedurally compute texture coordinates based on gl_VertexID
	vec2 texCoords[4] = vec2[](
		vec2(iUVRegion.x, iUVRegion.w),
		vec2(iUVRegion.z, iUVRegion.w),
		vec2(iUVRegion.z, iUVRegion.y),
		vec2(iUVRegion.x, iUVRegion.y)
	);
	texCoord = texCoords[gl_VertexID]; // Pass texture coordinates to fragment shader
}
