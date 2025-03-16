/*******************************************************************************************
*
*   raylib [core] example - Basic window (adapted for HTML5 platform)
*
*   This example is prepared to compile for PLATFORM_WEB and PLATFORM_DESKTOP
*   As you will notice, code structure is slightly different to the other examples...
*   To compile it for PLATFORM_WEB just uncomment #define PLATFORM_WEB at beginning
*
*   This example has been created using raylib 1.3 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2015 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

//#define PHYSAC_NO_THREADS
//#define PHYSAC_MALLOC MemAlloc
//#define PHYSAC_FREE MemFree
#define PHYSAC_IMPLEMENTATION
#include "vendor/Physac/src/physac.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

typedef struct GameState {
    PhysicsBody bigRectangle;
    Color bigRectangleColor;
    PhysicsBody smallRectangle;
    Color smallRectangleColor;

    Rectangle bigRectangleStart;
    Rectangle smallRectangleStart;

    Color background;
} GameState;

void UpdateDrawFrame(void* game);     // Update and Draw one frame

void reset(GameState* game, bool start) {
    if (!start) {
        DestroyPhysicsBody(game->bigRectangle);
        DestroyPhysicsBody(game->smallRectangle);
    }

    float size = 100.0f;
    Rectangle bigRectangleStart = (Rectangle){SCREEN_WIDTH / 2 + SCREEN_WIDTH / 4, SCREEN_WIDTH / 2 - size * 0.5f, 200, 200};
    Rectangle smallRectangleStart = (Rectangle){SCREEN_WIDTH / 4, bigRectangleStart.y + bigRectangleStart.height - 150, 100, 100};
    game->bigRectangle = CreatePhysicsBodyRectangle((Vector2){bigRectangleStart.x, bigRectangleStart.y}, bigRectangleStart.width, bigRectangleStart.height, 1.0f);
    game->smallRectangle = CreatePhysicsBodyRectangle((Vector2){smallRectangleStart.x, smallRectangleStart.y}, smallRectangleStart.width, smallRectangleStart.height, 10.0f);

    PhysicsAddForce(game->bigRectangle, (Vector2){-5000.0f, 0});
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib [core] example - basic window");
    InitPhysics();
    SetPhysicsGravity(0.0f, 0.0f);
    GameState game;
    game.background = (Color){40, 42, 54, 255};
    game.bigRectangleColor = (Color){139, 233, 253, 255};
    game.smallRectangleColor = (Color){98, 114, 164, 255};

    reset(&game, true);

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop_arg(UpdateDrawFrame, &game, 0, 1);
#else
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        UpdateDrawFrame(&game);
    }
#endif

    DestroyPhysicsBody(game.bigRectangle);
    DestroyPhysicsBody(game.smallRectangle);
    ClosePhysics();
    CloseWindow();

    return 0;
}

void UpdateDrawFrame(void* userdata) {
    GameState* game = (GameState*)userdata;

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        //reset(game, false);
    }

    //SetPhysicsTimeStep(GetFrameTime());
    RunPhysicsStep();

    BeginDrawing();
    {
        ClearBackground(game->background);

        // Draw created physics bodies
        int bodiesCount = GetPhysicsBodiesCount();
        for (int i = 0; i < bodiesCount; i++) {
            PhysicsBody body = GetPhysicsBody(i);
            int vertexCount = GetPhysicsShapeVerticesCount(i);
            for (int j = 0; j < vertexCount; j++)  {
                Vector2 vertexA = GetPhysicsShapeVertex(body, j);
                int jj = (((j + 1) < vertexCount) ? (j + 1) : 0);   // Get next vertex or first to close the shape
                Vector2 vertexB = GetPhysicsShapeVertex(body, jj);
                DrawLineEx(vertexA, vertexB, 3.0f, i % 2 == 0 ? game->bigRectangleColor : game->smallRectangleColor);     // Draw a line between two vertex positions
            }
        }
    }
    EndDrawing();
}