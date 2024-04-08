#include "simulation.hpp"
#include <raylib.h>
#include <iostream>

void Simulation::DrawGrid() {
    grid.DrawGrid(running);
}

void::Simulation::Randomize() {
    // Randomize the grid
    grid.Randomize();
}

void::Simulation::Clear() {
    // Clear the grid
    grid.Clear();
}

void::Simulation::SetRandomRate(int value) {
    // Set the random rate of the grid
    grid.SetRandomRate(value);
}

void::Simulation::SetMaterialType(int value) {
    // Set the shape index of the grid
    grid.SetMaterialType(value);
}

void::Simulation::DrawMaterial(int x, int y) {
    // Draw the material near the specified coordinates
    grid.DrawMaterial(x, y, running);
}

void Simulation::SetCell(int x, int y, int value) {
    // Set the value of the cell at (x, y) in the grid
    grid.SetCell(x, y, value);
}

int Simulation::CountLiveNeighbours(int x, int y) {
    // Define the number of live neighbours
    int liveNeighbours = 0;

    // Define the coordinates of the neighbours in a TOROIDAL grid
    int neighbourCoordinates[8][2] = {
        {(x-1 + grid.GetRows()) % grid.GetRows(), (y-1 + grid.GetColumns()) % grid.GetColumns()},
        {x, (y-1 + grid.GetColumns()) % grid.GetColumns()},
        {(x+1) % grid.GetRows(), (y-1 + grid.GetColumns()) % grid.GetColumns()},
        {(x-1 + grid.GetRows()) % grid.GetRows(), y},
        {(x+1) % grid.GetRows(), y},
        {(x-1 + grid.GetRows()) % grid.GetRows(), (y+1) % grid.GetColumns()},
        {x, (y+1) % grid.GetColumns()},
        {(x+1) % grid.GetRows(), (y+1) % grid.GetColumns()}
    };

    // Loop through the neighbours
    for (int i = 0; i < 8; i++) {
        // Get the coordinates of the neighbour and its value
        liveNeighbours += grid.GetCell(neighbourCoordinates[i][0], neighbourCoordinates[i][1]);
    }

    // Return the number of live neighbours
    return liveNeighbours;
}

void Simulation::Update() {
    // Create a new grid to store the updated values
    // todo 
    // Order of logics is important 
    // Apply Sand Logic
    UpdateSandLogic();
}

// -------------------------------------------- SAND LOGIC --------------------------------------------

void Simulation::UpdateSandLogic() {
    int number_of_sand_grains = 0;
    // Create a new grid to store the updated values
    Grid newGrid = grid;
    // Loop through the cells in the grid
    // for (int i = 0; i < grid.GetRows(); i++) {
    //     for (int j = grid.GetColumns()-1; j >= 0; j--) {
    //         if (grid.GetCell(i, j) ==  SAND_TYPE_1 || grid.GetCell(i, j) ==  SAND_TYPE_2 || grid.GetCell(i, j) ==  SAND_TYPE_3) {
    //             UpdateSandLogicOnCell(i, j, newGrid);
    //             number_of_sand_grains++;
    //         }
    //     }
    // }
    int firstHalf = grid.GetColumns()/2;
    for (int i = 0; i < grid.GetRows(); i++) {
        for (int j = 0; j < firstHalf; j++) {
            if (grid.GetCell(i, j) ==  SAND_TYPE_1 || grid.GetCell(i, j) ==  SAND_TYPE_2 || grid.GetCell(i, j) ==  SAND_TYPE_3) {
                UpdateSandLogicOnCell(i, j, newGrid);
                number_of_sand_grains++;
            }
        }
    }
    for (int i = 0; i < grid.GetRows(); i++) {
        for (int j = grid.GetColumns()-1; j >= firstHalf; j--) {
            if (grid.GetCell(i, j) ==  SAND_TYPE_1 || grid.GetCell(i, j) ==  SAND_TYPE_2 || grid.GetCell(i, j) ==  SAND_TYPE_3) {
                UpdateSandLogicOnCell(i, j, newGrid);
                number_of_sand_grains++;
            }
        }
    }
    // Update the grid with the new values
    grid = newGrid;
    std::cout << "Number of sand grains: " << number_of_sand_grains << std::endl;
}

void Simulation::UpdateSandLogicOnCell(int x, int y, Grid& newGrid) {
    // If the cell is sand
    // Check if the cell is not at the limit of the grid
    if (!newGrid.IsAtBottomLimit(x, y) || newGrid.IsEmpty(x + 1, y)) {
        // Check if the cell below is empty or below right or below left
        // Reset initial cell & 
        // Apply random sand type to future cell
        if (newGrid.IsEmpty(x + 1, y)) {
            newGrid.SetCell(x, y, EMPTY_TYPE);
            newGrid.SetCell(x + 1, y, GetRandomSandValue());
        } else if (newGrid.IsEmpty(x + 1, y - 1)) {
            newGrid.SetCell(x, y, EMPTY_TYPE);
            newGrid.SetCell(x + 1, y - 1, GetRandomSandValue());
        } else if (newGrid.IsEmpty(x + 1, y + 1)) {
            newGrid.SetCell(x, y, EMPTY_TYPE);
            newGrid.SetCell(x + 1, y + 1, GetRandomSandValue());
        }
    } 
}

int Simulation::GetRandomSandValue() {
    // Get a random sand type
    int randomSand = GetRandomValue(0, 2);
    if (randomSand == 0) {
        return SAND_TYPE_1;
    } else if (randomSand == 1) {
        return SAND_TYPE_2;
    } else {
        return SAND_TYPE_3;
    }
    return SAND_TYPE_1;
}