
#version 330 core
layout(location = 0) in vec2 aPos;  // 2D position of the vertex

void main() {
	// Convert 2D position to a vec4 with z = 0 and w = 1
	gl_Position = vec4(aPos, 0.0, 1.0);
}
