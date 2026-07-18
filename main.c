#include "raylib.h"
#include "function.h"
#include "constant.h"
#include <stdio.h>
#include <time.h>
#include <direct.h>

//------------------- Main Entry Point -------------------
int main(void) {
    _mkdir("database"); // Creates workspace folders automatically 

    SetConfigFlags(FLAG_WINDOW_MAXIMIZED);
    InitWindow(0, 0, "CINEMA BOOKING SYS");
    SetTargetFPS(60);
    SetRandomSeed((unsigned int)time(NULL));

    loadBookings();

    while (!WindowShouldClose() && !exitProgram) {
        BeginDrawing();
        ClearBackground(WHITE);

        Drawing(); // Triggers Rendering layer dispatchers
        Popup(NULL);

        EndDrawing();
        
    }
    CloseWindow();
    return 0;
}