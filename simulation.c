#include "raylib.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdio.h>

#define cellsize 5
#define gridsize 200
#define particlesize 10

int main(void)
{
    //
    // Allocate memory for states
    float* states = (float*)malloc(gridsize * gridsize * sizeof(float));
    float* prevStates = (float*)malloc(gridsize * gridsize * sizeof(float));
    float* nextStates = (float*)malloc(gridsize * gridsize * sizeof(float));

    // Initialize states
    for (int i = 0; i < gridsize * gridsize; i++) {
        states[i] = 0.0f;
        prevStates[i] = 0.0f;
        nextStates[i] = 0.0f;
    }

    // Window setup
    InitWindow(cellsize * gridsize, cellsize * gridsize, "Wave Simulation");
    SetWindowState(FLAG_WINDOW_UNDECORATED);
    SetTargetFPS(60);

    unsigned int index = 0;
    while (!WindowShouldClose()) {
        // Create oscillating sources
        //states[gridsize * 20 + 15] = 200.0f * (1 + sin(index / 5.0f));
        //states[gridsize * 20 + 35] = 200.0f * (1 + sin(index / 10.0f));
        states[gridsize*(gridsize/2) + (int)(gridsize*(.5+sin(index/50.0f)/2))] = 1000.0f * sin(index / 5.0f);
        //states[gridsize*(gridsize/3) + gridsize/2] = 1000.0f * sin(index / 5.0f);
        // Compute the next state
        for (int x = 0; x < gridsize; x++) {
            for (int y = 0; y < gridsize; y++) {
                float left = states[(x + gridsize - 1) % gridsize + y * gridsize];
                float right = states[(x + 1) % gridsize + y * gridsize];
                float top = states[x + ((y + gridsize - 1) % gridsize) * gridsize];
                float bottom = states[x + ((y + 1) % gridsize) * gridsize];
                float center = states[x + y * gridsize];

                // Laplacian (spatial second derivative)
                float laplacian = left + right + top + bottom - 4 * center;

                // Wave equation with reduced damping and faster propagation
                nextStates[x + y * gridsize] = 2 * center - prevStates[x + y * gridsize] + 0.3f * laplacian;
                nextStates[x + y * gridsize] *= 0.995f;  // Reduced damping factor
            }
        }

        // Swap state arrays
        float* temp = prevStates;
        prevStates = states;
        states = nextStates;
        nextStates = temp;

        // Visualization
        BeginDrawing();

        for (int x = 0; x < gridsize; x++) {
            for (int y = 0; y < gridsize; y++) {
                float value = states[x + y * gridsize];
                if (value > 255.0f) value = 255.0f;
                if (value < -255.0f) value = -255.0f;

                Color color;
                if (value > 0) {
                    color = (Color){(unsigned char)value, 0, 0, 255};  // Red for positive values
                } else if (value < 0) {
                    color = (Color){0, 0, (unsigned char)(-value), 255};  // Blue for negative values
                } else {
                    color = BLACK;
                }

                DrawRectangle(x * cellsize, y * cellsize, cellsize, cellsize, color);
            }
        }

        EndDrawing();
        index++;
    }

    // Cleanup
    CloseWindow();
    free(states);
    free(prevStates);
    free(nextStates);

    return 0;
}
