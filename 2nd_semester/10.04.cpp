#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

namespace gol {

constexpr int kRows = 10;
constexpr int kCols = 10;

using Grid = std::vector<std::vector<int>>;

Grid make_empty_grid()
{
    return Grid(kRows, std::vector<int>(kCols, 0));
}

bool in_bounds(int r, int c)
{
    return r >= 0 && r < kRows && c >= 0 && c < kCols;
}

int count_live_neighbors(const Grid& g, const int r, const int c)
{
    static constexpr int dr[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
    static constexpr int dc[8] = { -1,  0,  1,-1, 1,-1, 0, 1 };

    int count = 0;
    for (int k = 0; k < 8; ++k)
    {
        const int nr = r + dr[k];
        if (const int nc = c + dc[k]; in_bounds(nr, nc) && g[nr][nc] == 1)
        {
            ++count;
        }
    }
    return count;
}

Grid step(const Grid& g)
{
    Grid next = make_empty_grid();

    for (int r = 0; r < kRows; ++r)
    {
        for (int c = 0; c < kCols; ++c)
        {
            const int neighbors = count_live_neighbors(g, r, c);
            if (g[r][c] == 1)
            {
                next[r][c] = (neighbors == 2 || neighbors == 3) ? 1 : 0;
            }
            else
            {
                next[r][c] = (neighbors == 3) ? 1 : 0;
            }
        }
    }

    return next;
}

void print(const Grid& g, const int iteration)
{
    std::cout << "Iteration " << iteration << "\n";
    std::cout << "  ";
    for (int c = 0; c < kCols; ++c) std::cout << c;
    std::cout << "\n";

    for (int r = 0; r < kRows; ++r)
    {
        std::cout << r << " ";
        for (int c = 0; c < kCols; ++c)
        {
            std::cout << (g[r][c] ? '#' : '.');
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

Grid make_manual_initial_pattern()
{
    Grid g = make_empty_grid();

    // Glider (top-left-ish)
    // Coords: (row, col)
    g[1][2] = 1;
    g[2][3] = 1;
    g[3][1] = 1;
    g[3][2] = 1;
    g[3][3] = 1;

    // Blinker (middle-ish)
    g[6][4] = 1;
    g[6][5] = 1;
    g[6][6] = 1;

    return g;
}

} // namespace gol

int main()
{
    using namespace gol;

    Grid grid = make_manual_initial_pattern();

    constexpr int iterations = 25;

    for (int i = 0; i < iterations; ++i)
    {
        constexpr int delay_ms = 150;
        print(grid, i);
        grid = step(grid);
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
    }

    print(grid, iterations);
}