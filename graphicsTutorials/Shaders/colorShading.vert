#version 130	// GSLS version

// First index position
in vec2 vertexPosition;		// Grab the position from the vertex buffer
in vec4 vertexColor;		// Use the color from the MainGame
in vec2 vertexUV;			// Texture coordinates

out vec4 fragmentColor;		// Send the color to the fragment shader
out vec2 fragmentPosition;	// Send the fragment position to the fragment shader
out vec2 fragmentUV;		// Pass the uv coordinates to fragment shader

uniform mat4 P;

void main() {
	// Send the vertices to the fragment shader
	gl_Position.xy = (P * vec4(vertexPosition, 0.0, 1.0)).xy;		// Set the x and y vertices
	gl_Position.z = 0;		// Set the z vertex
	gl_Position.w = 1;		// Indicate that the coordinates are normalized

	// Set the fragment color
	fragmentColor = vertexColor;

	// Set the fragment position
	fragmentPosition = vertexPosition;

	fragmentUV = vec2(vertexUV.x, 1-vertexUV.y);
}