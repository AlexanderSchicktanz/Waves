#include "raylib.h"
#include <stdlib.h>
#include <math.h>

#define cellsize 5
#define gridsize 200

// Function to set a pixel in a texture
void SetPixelInTexture(Texture2D *texture, int x, int y, float value) {
    // Ensure x, y are within bounds
    if (x < 0 || x >= texture->width || y < 0 || y >= texture->height) return;

    // Retrieve the texture as an image
    Image image = LoadImageFromTexture(*texture);

    // Modify the pixel value (red channel for example)
    Color *pixels = LoadImageColors(image);
    pixels[x + y * texture->width] = (Color){(unsigned char)value, 0, 0, 255}; // R-channel

    // Update texture
    UpdateTexture(*texture, pixels);

    // Cleanup
    UnloadImageColors(pixels);
    UnloadImage(image);
}



int main(void) {
    const int windowWidth = cellsize * gridsize;
    const int windowHeight = cellsize * gridsize;

    // Initialize Raylib
    InitWindow(windowWidth, windowHeight, "Wave Simulation with Shaders");
    SetTargetFPS(60);

    // Load shader for wave calculations
    Shader waveShader = LoadShader(0, "wave_shader.fs");
    int resolutionLoc = GetShaderLocation(waveShader, "resolution");
    int timeLoc = GetShaderLocation(waveShader, "time");
    int prevStateLoc = GetShaderLocation(waveShader, "prevState");

    // Set resolution uniform
    Vector2 resolution = (Vector2){(float)gridsize, (float)gridsize};
    SetShaderValue(waveShader, resolutionLoc, &resolution, SHADER_UNIFORM_VEC2);

    // Create three Texture2D objects for states
    RenderTexture2D prevTexture = LoadRenderTexture(gridsize, gridsize);
    RenderTexture2D currentTexture = LoadRenderTexture(gridsize, gridsize);
    RenderTexture2D nextTexture = LoadRenderTexture(gridsize, gridsize);

    // Initialize the current texture with zero values
    BeginTextureMode(currentTexture);
    ClearBackground(BLACK);
    EndTextureMode();

    BeginTextureMode(prevTexture);
    ClearBackground(BLACK);
    EndTextureMode();

    float time = 0.0f;

    while (!WindowShouldClose()&&time<1.0f) {
        time += GetFrameTime();
        // Update shader values
        SetShaderValue(waveShader, timeLoc, &time, SHADER_UNIFORM_FLOAT);
        SetShaderValueTexture(waveShader, prevStateLoc, prevTexture.texture);

        // Apply shader to calculate the next state
        BeginTextureMode(nextTexture);
        BeginShaderMode(waveShader);
        DrawTexture(currentTexture.texture, 0, 0, WHITE);
        EndShaderMode();
        EndTextureMode();

        // Swap textures
        RenderTexture2D temp = prevTexture;
        prevTexture = currentTexture;
        currentTexture = nextTexture;
        nextTexture = temp;

        // Visualization
        // Set a red pixel at the center of the texture
SetPixelInTexture(&currentTexture.texture, gridsize / 2, gridsize / 2, 255.0f);

// Draw the texture
BeginDrawing();
ClearBackground(BLACK);
DrawTexturePro(currentTexture.texture, (Rectangle){0, 0, (float)gridsize, -(float)gridsize},
               (Rectangle){0, 0, windowWidth, windowHeight}, (Vector2){0, 0}, 0.0f, WHITE);
EndDrawing();

    }

    // Cleanup
    UnloadShader(waveShader);
    UnloadRenderTexture(prevTexture);
    UnloadRenderTexture(currentTexture);
    UnloadRenderTexture(nextTexture);
    CloseWindow();

    return 0;
}
