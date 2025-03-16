#include "raylib.h"

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
    PhysicsBody wall;

    Color background;
    Color textColor;

    int collisions;
} GameState;

void UpdateDrawFrame(void* game);     // Update and Draw one frame

void reset(GameState* game, bool start) {
    if (!start) {
        DestroyPhysicsBody(game->bigRectangle);
        DestroyPhysicsBody(game->smallRectangle);
    }

    float size = 100.0f;
    game->collisions = 0;
    game->bigRectangleStart = (Rectangle){SCREEN_WIDTH / 2 + SCREEN_WIDTH / 4, SCREEN_WIDTH / 2 - size * 0.5f, 200, 200};
    game->smallRectangleStart = (Rectangle){SCREEN_WIDTH / 4, game->bigRectangleStart.y + game->bigRectangleStart.height - 150, 100, 100};

    game->bigRectangle = CreatePhysicsBodyRectangle((Vector2){game->bigRectangleStart.x, game->smallRectangleStart.y}, game->smallRectangleStart.width, game->smallRectangleStart.height, 1.0f);
    game->bigRectangle->freezeOrient = true;
    game->bigRectangle->isGrounded = true;
    game->bigRectangle->mass = 1.0f;
    game->bigRectangle->staticFriction = 0.0f;
    game->bigRectangle->useGravity = false;
    //game->bigRectangle->velocity = (Vector2) {-0.5f, 0.0f};
    game->bigRectangle->force.y = 0.0f;
    game->bigRectangle->force.x = -1000.0f;
    
    game->smallRectangle = CreatePhysicsBodyRectangle((Vector2){game->smallRectangleStart.x, game->smallRectangleStart.y}, game->smallRectangleStart.width, game->smallRectangleStart.height, 1.0f);
    game->smallRectangle->freezeOrient = true;
    game->smallRectangle->isGrounded = true;
    game->smallRectangle->mass = 10000.0f;
    game->smallRectangle->staticFriction = 0.0f;
    game->smallRectangle->useGravity = false;

    game->wall = CreatePhysicsBodyRectangle((Vector2) { -1.0f, 0.0f}, 1.0f, SCREEN_HEIGHT * 2.0f, 9999999.0f);
    game->wall->freezeOrient = true;
    game->wall->isGrounded = true;
    game->wall->useGravity = true;
    game->wall->mass = 999999999999.0f;
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
    game.textColor = (Color){248, 248, 242, 255};

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
    DestroyPhysicsBody(game.wall);
    ClosePhysics();
    CloseWindow();

    return 0;
}

void UpdateDrawFrame(void* userdata) {
    GameState* game = (GameState*)userdata;

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        //reset(game, false);
    }




    // Draw created physics bodies
    int bodiesCount = GetPhysicsBodiesCount();
    for (int i = 0; i < bodiesCount; i++) {
        PhysicsBody body = GetPhysicsBody(i);
        // if (body->position.x < game->smallRectangleStart.width / 2.0f && body->velocity.x <= 0.0f) {
        //     body->velocity.x *= -1.0f;
        // }



        // body->freezeOrient = true;
        // body->isGrounded=true;


        int vertexCount = GetPhysicsShapeVerticesCount(i);
        for (int j = 0; j < vertexCount; j++)  {
            Vector2 vertexA = GetPhysicsShapeVertex(body, j);
            int jj = (((j + 1) < vertexCount) ? (j + 1) : 0);   // Get next vertex or first to close the shape
            Vector2 vertexB = GetPhysicsShapeVertex(body, jj);
            DrawLineEx(vertexA, vertexB, 3.0f, i % 2 == 0 ? game->bigRectangleColor : game->smallRectangleColor);     // Draw a line between two vertex positions
        }
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

        DrawText(TextFormat("Collisions: %d", game->collisions), 10, 10, 24, game->textColor);
    }
    EndDrawing();
}