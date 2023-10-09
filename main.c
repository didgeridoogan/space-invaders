#include <stdio.h>   //För in- och utmatning
#include <stdlib.h>  //För minneshantering
#include <string.h>  //För strings
#include "raylib.h"  //För Raylib
#include "raymath.h" //För .. Raymath

// Variabler namn ----------------------------------------------------

const int width = 700;
const int height = 600;
const int lives = 3;
const int maxProjectiles = 10;
Color currentColor = YELLOW;    // Sätt färgen till gul by default
bool isSpacebarPressed = false; // Ha koll på om spacebar är pressed
float lastShot = 0.0f;
float timeSinceLS = 0.0f;
int score = 0;
bool hasWon = false; // Har spelaren vunnit?

Texture2D starBackground;
Texture2D spaceship;
Texture2D alien;

Vector2 player_position;
Vector2 recPosition = {(float)width / 2, (float)height - 100};
Vector2 recSize = {(float)width / 12, (float)height / 13};

float alienMoveTimer = 0.0f;        // Checkar när aliens ska flytta sig
int alienDirection = 1;             // 1 för att röra sig åt höger, -1 för att röra sig åt vänster
float alienHorizontalSpeed = 10.0f; // Horisontell rörelsehastighet

// Variabler namn ----------------------------------------------------

// Alien struct ------------------------------------------------------

typedef struct Alien
{
    Rectangle rect;
    int minX;
    int maxX;
    int life;
    Color color;
} Alien;

Alien aliens[] = {
    // RAD 1

    (Alien){
        .rect = (Rectangle){120, 120, 44, 44},
        .minX = 80,
        .maxX = 140,
        .color = RED,
        .life = 300},
    (Alien){
        .rect = (Rectangle){220, 120, 44, 44},
        .minX = 80,
        .maxX = 140,
        .color = GREEN,
        .life = 300},
    (Alien){
        .rect = (Rectangle){320, 120, 44, 44},
        .minX = 80,
        .maxX = 140,
        .color = BLUE,
        .life = 300},
    (Alien){
        .rect = (Rectangle){420, 120, 44, 44},
        .minX = 80,
        .maxX = 140,
        .color = YELLOW,
        .life = 300},
    (Alien){
        .rect = (Rectangle){520, 120, 44, 44},
        .minX = 80,
        .maxX = 140,
        .color = ORANGE,
        .life = 300},

    // RAD 2

    (Alien){
        .rect = (Rectangle){120, 220, 44, 44},
        .color = RED,
        .life = 230},
    (Alien){
        .rect = (Rectangle){220, 220, 44, 44},
        .color = GREEN,
        .life = 230},
    (Alien){
        .rect = (Rectangle){320, 220, 44, 44},
        .color = BLUE,
        .life = 230},
    (Alien){
        .rect = (Rectangle){420, 220, 44, 44},
        .color = YELLOW,
        .life = 230},
    (Alien){
        .rect = (Rectangle){520, 220, 44, 44},
        .color = ORANGE,
        .life = 230},

    // RAD 3

    (Alien){
        .rect = (Rectangle){120, 320, 44, 44},
        .color = RED,
        .life = 150},
    (Alien){
        .rect = (Rectangle){220, 320, 44, 44},
        .color = GREEN,
        .life = 150},
    (Alien){
        .rect = (Rectangle){320, 320, 44, 44},
        .color = BLUE,
        .life = 150},
    (Alien){
        .rect = (Rectangle){420, 320, 44, 44},
        .color = YELLOW,
        .life = 150},
    (Alien){
        .rect = (Rectangle){520, 320, 44, 44},
        .color = ORANGE,
        .life = 150},
};

int numAliens = 15;

// Alien struct ------------------------------------------------------

// Stjärnbackgrund struct --------------------------------------------

typedef struct Star
{
    Rectangle rect;
    Color color;
} Star;

Star stars[] = {
    (Star){
        .rect = (Rectangle){24, 30, 3, 4},
        .color = GRAY},
    (Star){
        .rect = (Rectangle){29, 33, 2, 3},
        .color = GRAY},
    (Star){
        .rect = (Rectangle){56, 260, 4, 4},
        .color = LIGHTGRAY},
    (Star){
        .rect = (Rectangle){202, 85, 2, 3},
        .color = LIGHTGRAY},
    (Star){
        .rect = (Rectangle){72, 102, 3, 4},
        .color = GRAY},
    (Star){
        .rect = (Rectangle){102, 230, 2, 3},
        .color = GRAY},
    (Star){
        .rect = (Rectangle){77, 92, 4, 3},
        .color = GRAY},
    (Star){
        .rect = (Rectangle){76, 93, 2, 3},
        .color = LIGHTGRAY},
    (Star){
        .rect = (Rectangle){402, 202, 2, 3},
        .color = GRAY},
    (Star){
        .rect = (Rectangle){500, 403, 2, 3},
        .color = GRAY},
    (Star){
        .rect = (Rectangle){330, 313, 2, 3},
        .color = GRAY},
    (Star){
        .rect = (Rectangle){589, 340, 3, 4},
        .color = GRAY},
    (Star){
        .rect = (Rectangle){389, 503, 2, 3},
        .color = LIGHTGRAY},
    (Star){
        .rect = (Rectangle){277, 260, 3, 2},
        .color = GRAY},
    (Star){
        .rect = (Rectangle){289, 245, 2, 3},
        .color = GRAY},
    (Star){
        .rect = (Rectangle){72, 102, 3, 4},
        .color = GRAY},
    (Star){
        .rect = (Rectangle){102, 230, 2, 3},
        .color = GRAY},
    (Star){
        .rect = (Rectangle){77, 92, 4, 3},
        .color = LIGHTGRAY},
    (Star){
        .rect = (Rectangle){76, 93, 2, 3},
        .color = GRAY},
    (Star){
        .rect = (Rectangle){402, 202, 2, 3},
        .color = LIGHTGRAY},
    (Star){
        .rect = (Rectangle){500, 403, 2, 3},
        .color = GRAY},
    (Star){
        .rect = (Rectangle){330, 313, 2, 3},
        .color = GRAY},
    (Star){
        .rect = (Rectangle){499, 213, 2, 3},
        .color = LIGHTGRAY},
    (Star){
        .rect = (Rectangle){567, 103, 2, 3},
        .color = GRAY},
};

int numStars = 24;

// Stjärnbackgrund struct --------------------------------------------

// Projektil struct --------------------------------------------------

typedef struct Projectile
{
    Rectangle rect;
    Color color;
    bool active; // Indikerar om projektilen är "aktiv" elr inte
} Projectile;

// Projektil struct --------------------------------------------------

int main()
{

    // Projektil position --------------------------------------------

    Projectile projectiles[maxProjectiles] = {0};
    player_position.x = width / 2;
    player_position.y = height / 2;

    // Projektil position --------------------------------------------

    // Fönstersettings -----------------------------------------------

    InitWindow(width, height, "Space Invaders");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    bool exitWindowRequested = false; // Flaggan för att requesta att fönstret ska stänga
    bool exitWindow = false;          // Flaggan som sätter att fönstret ska stänga

    // Fönstersettings -----------------------------------------------

    // Loadea textures -----------------------------------------------

    starBackground = LoadTexture("./bakgrund.png");
    spaceship = LoadTexture("./spaceship.png");
    alien = LoadTexture("./alien.png");

    // Loadea textures -----------------------------------------------

    while (!WindowShouldClose())
    {
        if (WindowShouldClose() || IsKeyPressed(KEY_ESCAPE))
            exitWindowRequested = true;

        if (exitWindowRequested)
        {
            if (IsKeyPressed(KEY_Y))
                break;
            if (IsKeyPressed(KEY_N))
                exitWindowRequested = false;
        }

        // Uppdatera saker ------------------------------------------------

        if (IsKeyDown(KEY_RIGHT) || (IsKeyDown(KEY_D)))
            if (recPosition.x + recSize.x < width) // Kolla om den nuddar höger kant, stoppa isf
                recPosition.x += 3.5f;

        if (IsKeyDown(KEY_LEFT) || (IsKeyDown(KEY_A)))
            if (recPosition.x > 0) // Kolla vänster kant
                recPosition.x -= 3.5f;

        lastShot -= GetFrameTime();
        if (IsKeyDown(KEY_SPACE)) // Skjut projectiler när spacebar är pressed
        {
            if (lastShot <= 0.0f) // Läser av från senaste gången skottet sköts
            {
                lastShot = 0.2f;          // Hur långt emellan man får skjuta
                isSpacebarPressed = true; // Flagga spacebar nedtryckt
                for (int i = 0; i < maxProjectiles; i++)
                {
                    if (!projectiles[i].active)
                    {
                        projectiles[i].active = true;
                        projectiles[i].rect.x = recPosition.x + recSize.x / 2 - 2; // Anpassa positionen
                        projectiles[i].rect.y = recPosition.y;
                        projectiles[i].rect.width = 5; // Storlek på skotten
                        projectiles[i].rect.height = 10;
                        projectiles[i].color = currentColor;
                        break;
                    }
                }
            }
        }

        // Alien rörelse -------------------------------------------------

        alienMoveTimer += GetFrameTime();
        if (alienMoveTimer >= 0.9f) // Sätt in tid mellan hopp
        {
            // Flytta aliens
            for (int i = 0; i < numAliens; i++)
            {
                if (aliens[i].life > 0) // Om alien lever...
                {
                    aliens[i].rect.x += alienDirection * alienHorizontalSpeed;
                    if (aliens[i].rect.x < 0)
                    {
                        aliens[i].rect.x = 0; // Begränsa rörelsen till vänster
                        alienDirection = 1;   // Byt till höger
                    }

                    else if (aliens[i].rect.x + aliens[i].rect.width > width)
                    {
                        aliens[i].rect.x = width - aliens[i].rect.width; // Begränsa rörelsen till höger
                        alienDirection = -1;                             // Byt till vänsteer
                    }
                }
            }

            // Återställ timern
            alienMoveTimer = 0.0f;
        }

        // Alien rörelse -----------------------------------------------

        // Projektiler -------------------------------------------------

        for (int i = 0; i < maxProjectiles; i++)
        {
            if (projectiles[i].active)
            {
                projectiles[i].rect.y -= 8; // Justera hastighet här
                if (projectiles[i].rect.y + projectiles[i].rect.height < 0)
                {
                    projectiles[i].active = false;
                }
                else
                {
                    // Kolla kollision med aliens
                    for (int j = 0; j < numAliens; j++)
                    {
                        if (aliens[j].life > 0 && CheckCollisionRecs(projectiles[i].rect, aliens[j].rect))
                        {
                            // Träffar, ta bort projektilen och minska aliens liv
                            projectiles[i].active = false;
                            aliens[j].life -= 660; // Justera hur mycket liv som minskas vid en träff
                            numAliens -= 1;

                            if (aliens[j].life <= 0)
                            {
                                // Alien död :(, lägg till poäng i score countern
                                score += 100;
                            }
                        }
                    }
                }
            }
        }

        // Projektiler ------------------------------------------------

        // Uppdatera saker --------------------------------------------

        // Rendering --------------------------------------------------

        BeginDrawing();
        ClearBackground(BLACK);

        {

            Rectangle src = {
                .x = 0,
                .y = 0,
                .width = 700,
                .height = 600};

            Rectangle dest = {
                .x = 0,
                .y = 0,
                .width = 700,
                .height = 600};

            // DrawTexture(starBackground, 0, 0, WHITE); // Ett simplare sätt att rita ut bilden på
            DrawTexturePro(starBackground, src, dest, Vector2Zero(), 0.0f, WHITE);

            // Stjärnorna ovanpå stjärnorna ------------------------------------

            for (int i = 0; i < numStars; i++)
            {
                DrawRectangleRec(stars[i].rect, stars[i].color); // Rita ut dom
            }

            // Stjärnorna ovanpå stjärnorna ------------------------------------

            // Spaceship -------------------------------------------------------

            DrawTextureV(spaceship, recPosition, WHITE);

            for (int i = 0; i < maxProjectiles; i++) // Rita projektiler
            {
                if (projectiles[i].active)
                {
                    DrawRectangleRec(projectiles[i].rect, projectiles[i].color);
                }
            }

            // Spaceship -----------------------------------------------------

            // Aliensarna ----------------------------------------------------

            for (int i = 0; i < numAliens; i++) // Rendera aliens, men bara de som har liv kvar
            {
                if (aliens[i].life > 0)
                {
                    DrawTexture(alien, aliens[i].rect.x, aliens[i].rect.y, WHITE);
                }
            }

            // Aliensarna ----------------------------------------------------

            // Info counters -----------------------------------------------

            DrawText(TextFormat("Lives: %d", lives), 40, 40, 25, WHITE);
            DrawText(TextFormat("%d", score), width / 2, 40, 35, WHITE);

            // Om alla aliens är döda

            if (numAliens < 6)
            {
                DrawRectangle(190, 140, 350, 250, PURPLE);
                DrawRectangle(210, 160, 310, 210, BLACK);
                DrawText("     Grattis!\nDu klarade det!", 250, 220, 30, PURPLE);
                DrawText(TextFormat("Score: %d", score), width / 2 - 80, 300, 35, WHITE);
            }

            // Info counters -----------------------------------------------

            // Om användaren vill stänga fönstret -------------------------

            if (exitWindowRequested)
            {
                DrawRectangle(190, 140, 350, 250, PURPLE);
                DrawRectangle(210, 160, 310, 210, BLACK);
                DrawText("Vill du avsluta\n programmet?\n     [Y/N]", 250, 200, 30, PURPLE);
            }

            // Om användaren vill stänga fönstret -------------------------

            EndDrawing();

            // Rendering --------------------------------------------------
        }
    }
    return 0;
}