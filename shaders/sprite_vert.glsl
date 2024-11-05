
#version 330 core

layout (location = 0) in vec3 aPos;  // Vertex position
layout (location = 1) in vec2 aTex;  // Texture coordinate

out vec2 texCoord;  // Output texture coordinate for fragment shader

uniform vec2 uPosition;  // Position of the image on the screen (x, y)
uniform vec2 uScale;     // Width and height scale factors
uniform float uRotation; // Rotation angle in radians
uniform vec2 uPivot;     // Custom pivot point for rotation

void main()
{
    // Create a 2D rotation matrix
    float cosAngle = cos(uRotation);
    float sinAngle = sin(uRotation);
    mat2 rotation = mat2(cosAngle, -sinAngle,
                         sinAngle, cosAngle);

    // Step 1: Scale and translate to pivot point
    vec2 scaledPos = aPos.xy * uScale;
    vec2 centeredPos = scaledPos - uPivot;

    // Step 2: Apply rotation around pivot point
    vec2 rotatedPos = rotation * centeredPos;

    // Step 3: Translate back from pivot and move to screen position
    vec2 finalPos = rotatedPos + uPivot + uPosition;

    gl_Position = vec4(finalPos, aPos.z, 1.0); // Set final position
    texCoord = aTex;  // Pass texture coordinates to fragment shader
}
