#version 130	// GSLS version

// The fragment shader operates on each pixel in a given polygon

in vec2 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;

out vec4 color;		// Output a color to the screen

uniform sampler2D mySampler;

void main() {
	// Get the color from the texture sampler
	vec4 textureColor = texture(mySampler, fragmentUV);

	//color = textureColor * fragmentColor;


	color = fragmentColor * textureColor;	// Hardcore the color, RGB


}