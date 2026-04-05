#include <raylib.h>
#include <stdbool.h>

#define ROWS 30
#define COLS 30

const int screenWidth = 800;
const int screenHeight = 800;

const int cellWidth = 20;
const int cellHeight = 20;

typedef struct Cell
{
    int i;
    int j;
    int state;
} Cell;

void CellDraw(Cell cell, float offsetX, float offsetY);
bool IndexIsValid(int i, int j);
void ToggleCellState(Cell grid[ROWS][COLS], int i, int j);
void ClearGrid(Cell grid[ROWS][COLS]);

int CountNeighbors(Cell grid[ROWS][COLS], int r, int c);
void Update(Cell grid[ROWS][COLS]);

int main(void)
{
    // Window creations
    InitWindow(screenWidth, screenHeight, "game of life - window");
    SetTargetFPS(60);

    // Initialize grid
    Cell grid[ROWS][COLS];
    for (int i = 0; i < COLS; i++) 
    {
        for(int j = 0; j < ROWS; j++)
        {
            grid[i][j] = (Cell)
            {
                .i = i, 
                .j = j,
                .state = 0
            };
        }
    }

    bool simulationActive = false;
    float timer = 0;

    float offsetY = (float)((screenHeight - (cellHeight * ROWS)) / 2) + 50; 
    float offsetX = (float)(screenWidth - (cellWidth * COLS)) / 2; 

    // Main game loop
    while (!WindowShouldClose()) 
    {
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Vector2 mPos = GetMousePosition();
            int indexI = (mPos.x - offsetX) / cellWidth;
            int indexJ = (mPos.y - offsetY) / cellHeight;

            if(IndexIsValid(indexI, indexJ))
            {
                ToggleCellState(grid, indexI, indexJ);
            }
        }

        if(IsKeyPressed(KEY_SPACE))
        {
            simulationActive = !simulationActive;
        }
        if (IsKeyPressed(KEY_C)) 
        {
            ClearGrid(grid);
        }

        timer += GetFrameTime();
        
        // Update grid according to cell state
        if(simulationActive && timer >= 0.25f)
        {
            Update(grid);
            timer = 0.0f;
        }

        // Draw visuals
        BeginDrawing();

            ClearBackground(GRAY);

            for (int i = 0; i < COLS; i++) 
            {
                for(int j = 0; j < ROWS; j++)
                    CellDraw(grid[i][j], offsetX, offsetY);
            }
        
        DrawText(TextFormat("Simulation active: %b, Timer:%.2f", simulationActive, timer), 10, 10, 20, RED);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

// Visuals
void CellDraw(Cell cell, float offsetX, float offsetY)
{
    float posX = (cell.i * cellWidth) + offsetX;
    float posy = (cell.j * cellHeight) + offsetY;

    if(cell.state == 1)
        DrawRectangle(posX, posy, cellWidth, cellHeight, YELLOW);
    else
        DrawRectangle(posX, posy, cellWidth, cellHeight, BLACK);

    DrawRectangleLines(posX, posy, cellWidth, cellHeight, DARKGRAY);
}

bool IndexIsValid(int i, int j)
{
    return i >= 0 && i < COLS && j >=0 && j< ROWS;
}

void ToggleCellState(Cell grid[ROWS][COLS], int i, int j)
{
    if(grid[i][j].state == 0)
        grid[i][j].state = 1;
    else
        grid[i][j].state = 0;
}

// Game logic
int CountNeighbors(Cell grid[ROWS][COLS], int r, int c)
{
    int count = 0;

    for(int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++) 
        {
            if (i == 0 && j == 0)
                continue;

            int nr = r+i;
            int nc = c+j;

            if (nr >= 0 && nr < ROWS && nc >= 0 && nc < COLS) 
                count += grid[nr][nc].state;
        }
    }

    return count;
}

void Update(Cell grid[ROWS][COLS])
{
    Cell nextGrid[ROWS][COLS];

    for (int r = 0; r < ROWS; r++) 
    {
        for (int c = 0; c < COLS; c++) 
        {
            int neighbors = CountNeighbors(grid, r, c);

            if(grid[r][c].state == 1)
                nextGrid[r][c].state = (neighbors == 2 || neighbors == 3);
            else
                nextGrid[r][c].state = (neighbors == 3);
        }
    } 

    for (int r = 0; r < ROWS; r++) 
    {
        for (int c = 0; c < COLS; c++) 
        {
            grid[r][c].state = nextGrid[r][c].state;
        }
    }
}

void ClearGrid(Cell grid[ROWS][COLS])
{
    for(int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++) 
            grid[i][j].state = 0;
}

