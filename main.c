#include "raylib.h"
#include <stdlib.h>
#include <math.h>

#define cellsize 2
#define gridsize 500
#define particlesize 10

typedef struct{
    float x;
    float y;
    float vx;
    float vy;
    int mass;
    int frequency;
    int amplitude;
} particle;

int main(void)
{
    //Particles
    particle* particles = (particle*)malloc(particlesize * sizeof(particle));
    for(int i = 0; i < particlesize; i++){
        particles[i] = (particle){GetRandomValue(0, gridsize), GetRandomValue(0, gridsize), GetRandomValue(-1, 1), GetRandomValue(-1, 1), GetRandomValue(1, 10), GetRandomValue(1, 10), GetRandomValue(200, 1000)};
    }
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
        //Particles
        for(int i = 0; i < particlesize; i++){
            for(int j = 0; j < particlesize; j++){
                if(i != j){
                    float distance = sqrt(pow(particles[i].x - particles[j].x, 2) + pow(particles[i].y - particles[j].y, 2));
                    float force = 1 / (distance * distance);
                    float angle = atan2(particles[i].y - particles[j].y, particles[i].x - particles[j].x);
                    float dx = cos(angle) * force;
                    float dy = sin(angle) * force;
                    particles[i].vx -= dx*100 / particles[i].mass;
                    particles[i].vy -= dy*100 / particles[i].mass;
                }
            }
        }
        for (int i = 0; i < particlesize; i++) {
            particle* p = &particles[i];
            p->x += p->vx;
            p->y += p->vy;
            if (p->x < 0 || p->x > gridsize) {
                if (p->x < 0){
                    p->x=0;
                }
                if (p->x > gridsize-1){
                    p->x=gridsize;
                }
                
                p->vx = -p->vx;
            }
            if (p->y < 0 || p->y > gridsize) {
                if (p->y < 0){
                    p->y=0;
                }
                if (p->y > gridsize-1){
                    p->y=gridsize;
                }
                p->vy = -p->vy;
            }
            if(p->x > 0 && p->x < gridsize && p->y > 0 && p->y < gridsize){
                states[(int)p->x + (int)p->y * gridsize] += p->amplitude * sin(index / p->frequency);
            }
        }
        // Create oscillating sources
        //states[gridsize * 20 + 15] = 200.0f * (1 + sin(index / 5.0f));
        //states[gridsize * 20 + 35] = 200.0f * (1 + sin(index / 10.0f));
        //////states[gridsize*(gridsize/2) + (int)(gridsize*(.5+sin(index/50.0f)/2))] = 1000.0f * sin(index / 5.0f);
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
