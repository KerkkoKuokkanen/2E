
#version 330 core

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D tex0;
uniform float uniformAlpha;

void main()
{
	FragColor = texture(tex0, texCoord);
	FragColor.a = FragColor.a * uniformAlpha;
}
