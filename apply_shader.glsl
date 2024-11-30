#version 330

uniform sampler2D stateTexture; // Current state of the grid
uniform sampler2D changeTexture; // Computed changes in the grid
uniform int gridSize;

out vec4 fragColor;

void main() {
    ivec2 coord = ivec2(gl_FragCoord.xy);
    vec4 state = texelFetch(stateTexture, coord, 0);
    vec4 change = texelFetch(changeTexture, coord, 0);

    // Apply the change to the state
    float newState = state.r + change.r;

    // Output updated state
    fragColor = vec4(newState, 0.0, 0.0, 1.0);
}
