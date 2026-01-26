#include <stdio.h>
#include <iostream>
#include <deque>
#include "include/raylib.h"
#include "include/raymath.h"
using namespace std;

Color bgPink = {255, 239, 246, 255};
Color borderPink = {255, 170, 210, 255};
Color jellyDarkBlue = {77, 165, 255, 220};
Color jellyBlue = {120, 200, 255, 220};
Color jellyWhite = {255, 255, 255, 90};
Color outlineDark = {150, 40, 90, 255};
Color textPink = {200, 60, 120, 255};
Color candyRed = {255, 80, 80, 255};

float cellSize = 30;
float cellCount = 25;
float offset = 75;

double lastUpdateTime = 0;

bool ElementInDeque(Vector2 element, deque<Vector2> deque)
{
    for (unsigned int i = 0; i < deque.size(); i++)
    {
        if (Vector2Equals(deque[i], element))
        {
            return true;
        }
    }
    return false;
}

bool eventTriggered(double interval)
{
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

class Snake
{
public:
    deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
    Vector2 direction = {1, 0};
    bool addSegment = false;

    void Draw()
    {
        for (int i = 0; i < body.size(); i++)
        {
            float x = body[i].x;
            float y = body[i].y;
            Rectangle segment = Rectangle{offset + x * cellSize, offset + y * cellSize, cellSize, cellSize};
            Color bodyColor = (i % 2 == 0) ? jellyDarkBlue : jellyBlue;
            DrawRectangleRounded(segment, 0.6, 8, bodyColor);
            Rectangle highlight = {segment.x + 4, segment.y + 4, segment.width - 8, segment.height - 8};

            DrawRectangleRounded(highlight, 0.4, 6, jellyWhite);
        }
    }

    bool Update()
    {
        Vector2 nextHead = Vector2Add(body[0], direction);

        if (nextHead.x < 0 || nextHead.x >= cellCount ||
            nextHead.y < 0 || nextHead.y >= cellCount)
        {
            return false;
        }

        body.push_front(nextHead);

        if (addSegment)
        {
            addSegment = false;
        }
        else
        {
            body.pop_back();
        }

        return true;
    }

    void Reset()
    {
        body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
        direction = {1, 0};
    }
};

class Food
{
public:
    vector<string> images = {"candy", "croissant", "gummy-bear", "strawberry"};
    Vector2 position;
    Texture2D texture;
    float bounceTime = 0.0;

    Food(deque<Vector2> snakebody)
    {
        GenerateFoodImg();
        position = GenerateRandomFood(snakebody);
    }

    ~Food()
    {
        UnloadTexture(texture);
    }

    void GenerateFoodImg()
    {
        int randPos = rand() % images.size();
        cout << "pos: " << randPos << " res: " << images[randPos];
        string path = "graphics/" + images[randPos] + ".png";
        Image image = LoadImage(path.c_str());
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
    }

    void Draw()
    {
        bounceTime += GetFrameTime();
        float bounce = sin(bounceTime * 6) * 3;
        float scale = 1.0 + sin(bounceTime * 6) * 0.05;

        Rectangle dest = {
            offset + position.x * cellSize + cellSize / 2,
            offset + position.y * cellSize + cellSize / 2 + bounce,
            cellSize * scale,
            cellSize * scale};

        Rectangle source = {0, 0, (float)texture.width, (float)texture.height};

        Vector2 origin = {dest.width / 2, dest.height / 2};

        DrawTexturePro(texture, source, dest, origin, 0.0, WHITE);
    }

    Vector2 GenerateRandomCell()
    {
        float x = GetRandomValue(0, cellCount - 1);
        float y = GetRandomValue(0, cellCount - 1);
        return Vector2{x, y};
    }

    Vector2 GenerateRandomFood(deque<Vector2> snakeBody)
    {
        float x = GetRandomValue(0, cellCount - 1);
        float y = GetRandomValue(0, cellCount - 1);
        Vector2 position = {x, y};
        while (ElementInDeque(position, snakeBody))
        {
            position = GenerateRandomCell();
        }
        GenerateFoodImg();
        return position;
    }
};

class Game
{
public:
    Snake snake = Snake();
    Food food = Food(snake.body);
    bool running = false;
    bool gameOver = false;
    int score = 0;
    Sound eatSound;
    Sound wallSound;

    Game()
    {
        eatSound = LoadSound("sounds/eat.mp3");
        wallSound = LoadSound("sounds/wall.mp3");
    }

    ~Game()
    {
        UnloadSound(eatSound);
        UnloadSound(wallSound);
        CloseAudioDevice();
    }

    void Draw()
    {
        food.Draw();
        snake.Draw();
        DrawGameOver();
    }

    void Update()
    {
        if (!running || gameOver)
            return;

        bool moved = snake.Update();

        if (!moved)
        {
            GameOver();
            return;
        }

        CheckCollisionWithFood();
        CheckCollisionWithTail();
    }

    void restart()
    {
        snake.Reset();
        food.position = food.GenerateRandomFood(snake.body);
        score = 0;
        gameOver = false;
        running = false;
    }

    void CheckCollisionWithFood()
    {
        if (Vector2Equals(snake.body[0], food.position))
        {
            food.position = food.GenerateRandomFood(snake.body);
            snake.addSegment = true;
            score++;
            PlaySound(eatSound);
        }
    }

    void CheckCollisionWithTail()
    {
        deque<Vector2> headlessBody = snake.body;
        headlessBody.pop_front();
        if (ElementInDeque(snake.body[0], headlessBody))
        {
            GameOver();
        }
    }

    void GameOver()
    {
        running = false;
        gameOver = true;
        PlaySound(wallSound);
    }

    void DrawGameOver()
    {
        if (!gameOver)
            return;

        DrawRectangle(
            offset,
            offset,
            cellSize * cellCount,
            cellSize * cellCount,
            Color{255, 170, 210, 200});

        const char *text = "GAME OVER";
        int fontSize = 40;
        int textWidth = MeasureText(text, fontSize);

        DrawText(text, offset + (cellSize * cellCount - textWidth) / 2,
                 offset + cellSize * cellCount / 2 - fontSize, fontSize,
                 candyRed);

        const char *subText = "Press ENTER to restart";
        int subSize = 20;
        int subWidth = MeasureText(subText, subSize);

        DrawText(
            subText, offset + (cellSize * cellCount - subWidth) / 2,
            offset + cellSize * cellCount / 2 + 10, subSize, textPink);
    }
};

int main()
{
    InitWindow(2 * offset + cellSize * cellCount, 2 * offset + cellCount * cellSize, "Snake Game");
    InitAudioDevice();
    SetTargetFPS(60);

    Game game = Game();

    while (WindowShouldClose() == false)
    {
        BeginDrawing();
        if (eventTriggered(0.15))
        {
            game.Update();
        }

        if (game.gameOver != true)
        {
            if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1)
            {
                game.snake.direction = {0, -1};
                game.running = true;
            }
            if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1)
            {
                game.snake.direction = {0, 1};
                game.running = true;
            }
            if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1)
            {
                game.snake.direction = {1, 0};
                game.running = true;
            }
            if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1)
            {
                game.snake.direction = {-1, 0};
                game.running = true;
            }
        }

        if (game.gameOver && IsKeyPressed(KEY_ENTER))
        {
            game.restart();
        }

        ClearBackground(bgPink);
        DrawRectangleLinesEx(Rectangle{offset - 5, offset - 5, cellSize * cellCount + 10, cellSize * cellCount + 10}, 5, borderPink);
        DrawText("Snake Game", offset - 5, 20, 40, textPink);
        const char *scoreText = TextFormat("Score: %i", game.score);
        int textWidth = MeasureText(scoreText, 40);
        DrawText(scoreText, cellSize * cellCount + offset - textWidth, 20, 40, textPink);
        game.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}