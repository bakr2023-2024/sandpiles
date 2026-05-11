#include <raylib.h>
#include <cstring>
#include <string>
int main(int argc, char **argv)
{
    int hex0 = 0x000000, hex1 = 0xff0000, hex2 = 0x00ff00, hex3 = 0x0000ff;
    if (argc == 5)
        hex3 = std::stoi(argv[4], nullptr, 16);
    if (argc >= 4)
        hex2 = std::stoi(argv[3], nullptr, 16);
    if (argc >= 3)
        hex1 = std::stoi(argv[2], nullptr, 16);
    if (argc >= 2)
        hex0 = std::stoi(argv[1], nullptr, 16);
    int sw = 250, sh = 250;
    InitWindow(sw, sh, "Sandpiles");
    int *grid = new int[sw * sh]();
    grid[(sh / 2) * sw + (sw / 2)] = 10000000;
    int *next = new int[sw * sh]();
    int *pixels = new int[sw * sh]();
    Texture2D tex = LoadTextureFromImage(GenImageColor(sw, sh, BLACK));
    bool isStop = false;
    bool isStable = false;
    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_S))
            isStop = !isStop;
        if (!isStop && !isStable)
        {
            for (int i = 0; i < 100; i++)
            {
                isStable = true;
                std::memset(next, 0, sw * sh * sizeof(int));
                for (int y = 0; y < sh; y++)
                {
                    for (int x = 0; x < sw; x++)
                    {
                        int idx = y * sw + x;
                        int val = grid[idx];
                        if (val >= 4)
                        {
                            isStable = false;
                            next[idx] += (val - 4);
                            if (x + 1 < sw)
                                next[idx + 1] += 1;
                            if (x - 1 >= 0)
                                next[idx - 1] += 1;
                            if (y + 1 < sh)
                                next[idx + sw] += 1;
                            if (y - 1 >= 0)
                                next[idx - sw] += 1;
                        }
                        else
                            next[idx] += val;
                    }
                }
                int *temp = grid;
                grid = next;
                next = temp;
                if (isStable)
                    break;
            }
            for (int y = 0; y < sh; y++)
            {
                for (int x = 0; x < sw; x++)
                {
                    int idx = y * sw + x;
                    int rgb = grid[idx] == 3 ? hex3 : grid[idx] == 2 ? hex2
                                                  : grid[idx] == 1   ? hex1
                                                                     : hex0;
                    pixels[idx] = 0xff000000 | rgb;
                }
            }
            UpdateTexture(tex, pixels);
        }
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(tex, 0, 0, WHITE);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}