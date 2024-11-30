#version 330

uniform sampler2D stateTexture; // Current state of the grid
uniform sampler2D changeTexture; // Current changes in the grid
uniform int gridSize;

out vec4 fragColor;

void main() {
    ivec2 coord = ivec2(gl_FragCoord.xy);
    vec4 state = texelFetch(stateTexture, coord, 0);
    vec4 change = texelFetch(changeTexture, coord, 0);

    // Read neighboring states
    vec4 left = texelFetch(stateTexture, coord + ivec2(-1, 0), 0);
    vec4 right = texelFetch(stateTexture, coord + ivec2(1, 0), 0);
    vec4 top = texelFetch(stateTexture, coord + ivec2(0, -1), 0);
    vec4 bottom = texelFetch(stateTexture, coord + ivec2(0, 1), 0);

    // Compute Laplacian (spatial second derivative)
    float laplacian = left.r + right.r + top.r + bottom.r - 4.0 * state.r;

    // Update wave equation with damping
    change.r = 2.0 * state.r + laplacian * 0.3;
    change.g = change.r * 0.995; // Damping

    // Pass the new change to the next shader
    fragColor = vec4(change.r, change.g, 0.0, 1.0);
}
