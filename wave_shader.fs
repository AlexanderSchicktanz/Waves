#version 330

in vec2 fragTexCoord;
out vec4 fragColor;

uniform sampler2D prevState;
uniform vec2 resolution;
uniform float time;

void main() {
    vec2 uv = fragTexCoord;
    vec2 pixelSize = 1.0 / resolution;

    // Fetch neighboring states
    float center = texture(prevState, uv).r;
    float left = texture(prevState, uv - vec2(pixelSize.x, 0)).r;
    float right = texture(prevState, uv + vec2(pixelSize.x, 0)).r;
    float top = texture(prevState, uv - vec2(0, pixelSize.y)).r;
    float bottom = texture(prevState, uv + vec2(0, pixelSize.y)).r;

    // Allow external updates to propagate through the shader
    if (center > 0.99) {
        fragColor = vec4(center, 0.0, 0.0, 1.0); // External injection
    } else {
        // Laplacian
        float laplacian = left + right + top + bottom - 4.0 * center;

        // Wave equation
        float nextValue = 2.0 * center - texture(prevState, uv).g + 0.3 * laplacian;
        nextValue *= 0.995; // Damping

        // Set red channel to the new state, green channel to the current state
        fragColor = vec4(nextValue, center, 0.0, 1.0);
    }
}
