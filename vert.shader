#version 330

in vec2 vertexTexCoord; // Texture coordinate input
out vec2 fragTexCoord;  // Pass texture coordinate to fragment shader

uniform mat4 mvp; // Model-View-Projection matrix (required by Raylib)

void main() {
    fragTexCoord = vertexTexCoord;
    gl_Position = mvp * vec4(vertexTexCoord, 0.0, 1.0); // Use vertexTexCoord as position
}
