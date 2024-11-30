#version 330

// Input vertex attributes
in vec2 fragTexCoord;

// Input texture samplers
uniform sampler2D prevState;   // Texture storing the previous wave states
uniform sampler2D currState;   // Texture storing the current wave states

// Output fragment color
out vec4 fragColor;

void main() {
    // Get texture coordinates
    vec2 texCoord = fragTexCoord;

    // Fetch states from the current and previous textures
    float center = texture(currState, texCoord).r;
    float prev = texture(prevState, texCoord).r;

    // Get neighboring values (Laplacian)
    float left = texture(currState, texCoord + vec2(-1.0 / 500.0, 0.0)).r;
    float right = texture(currState, texCoord + vec2(1.0 / 500.0, 0.0)).r;
    float top = texture(currState, texCoord + vec2(0.0, -1.0 / 500.0)).r;
    float bottom = texture(currState, texCoord + vec2(0.0, 1.0 / 500.0)).r;

    // Compute Laplacian
    float laplacian = left + right + top + bottom - 4.0 * center;

    // Wave equation with damping
    float next = 2.0 * center - prev + 0.3 * laplacian;
    next *= 0.995; // Damping factor

    // Clamp negative values into a separate channel
    float positive = max(0.0, next);
    float negative = max(0.0, -next);

    // Store positive in red channel and negative in blue channel
    fragColor = vec4(positive, 0.0, negative, 1.0);
}
