#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <conio.h>




struct Map {
    std::string name = "null";
    int* data;
    int width;
    int height;
};

struct Entity {
    int x;
    int y;
    char name;
    int health;
    int damage;
    int oxygen = 3;
};

enum Tile_Type
{
    Ground = 0,
    Water = 1,
    Mountain = 2,
    Door = 3,
    Lava = 4,
    Tall_Grass = 5,
    Quicksand = 6,
};

struct Tile {
    char name;
    bool walkable = true;
    bool isSlow = false;
    bool victory = false;
};

// Tiles
const Tile ground = { '.' };
const Tile water = { '~' };
const Tile mountain = { 'A', false };
const Tile door = { 'E', true, false, true };
const Tile lava = { '=' };
const Tile tall_grass = { '#' };
const Tile quicksand = { 'O', true, true };


Tile const tiles[7] = { ground, water, mountain, door, lava, tall_grass, quicksand };


int GetCell(int x, int y, int columnMax)
{
    return y * columnMax + x;
}


void MapReset(Entity& player, Entity ennemies[]) {
    player.x = -2;
    player.y = -2;
    player.health = 1;
    player.oxygen = 3;

    for (int i = 0;i < 5;i++) {
        ennemies[i].x = -10;
        ennemies[i].y = -10;
    }
}


Map CreateMap(int currentMap, Entity& player, Entity ennemies[]) {

    Map map;

    std::string path = "maps/level_";
    path.append(std::to_string(currentMap));
    path.append(".txt");

    // Opening File
    std::ifstream level(path);


    // If error opening file
    if (!level.is_open()) {
        std::cerr << "No More Maps!";
        map.height = 0;
        map.width = 0;
        return map;
    }

    std::string line;
    int lineMax = 0;
    int columnMax = 0;

    // Def. Map Size
    getline(level, map.name);

    while (std::getline(level, line)) {
        columnMax++;
        lineMax = line.size();
    }

    map.height = lineMax;
    map.width = columnMax;
    map.data = new int[map.height * map.width];



    // Def. Map Data
    int n = -1;
    level.clear();
    level.seekg(0);
    int currentLine = 0;
    int currentEnnemy = 0;
    getline(level, map.name);

    while (std::getline(level, line)) {

        for (int i = 0;i < lineMax;i++) {

            n++;

            // Detects Player
            if (line[i] == 'H') {
                map.data[n] = 0;
                player.x = i;
                player.y = currentLine;
            }
            // Detects Ennemy
            else if (line[i] == 'X') {
                map.data[n] = 0;
                ennemies[currentEnnemy].x = i;
                ennemies[currentEnnemy].y = currentLine;
                currentEnnemy++;
            }

            for (int t = 0; t < 7; t++) {
                if (line[i] == tiles[t].name) {
                    map.data[n] = t;
                }
            }
        }
        currentLine++;
    }

    // Closing File
    level.close();

    return map;
}


void ShowMap(Map map, Entity player, Entity ennemies[], int currentMap) {

    for (int y = 0; y < map.width; ++y) {
        for (int x = 0; x < map.height; ++x) {

            int index = GetCell(x, y, map.height);

            // Detects Ennemies
            bool isEnnemy = false;
            int whenEnnemy;

            for (int i = 0; i < 5; i++) {
                if (x == ennemies[i].x and y == ennemies[i].y) {
                    // Display if not in tall grass
                    if (map.data[index] != Tall_Grass) {
                        isEnnemy = true;
                        whenEnnemy = i;
                    }
                }
            }

            // Display Ennemies
            if (isEnnemy) {
                std::cout << ennemies[whenEnnemy].name << ' ';
            }

            // Display Player (and if not in tall grass)
            else if (x == player.x and y == player.y) {
                if (map.data[index] != Tall_Grass) {
                    std::cout << player.name << ' ';
                }
                else {
                    std::cout << tiles[Tall_Grass].name << ' ';
                }
            }

            // Display Map
            else {
                std::cout << tiles[map.data[index]].name << ' '; // La ligne ultime ?
            }
        }
        std::cout << std::endl;
    }

    // External Informations
    std::cout << std::endl;
    std::cout << "Level " << currentMap << " : " << map.name << std::endl; // Level Name
    std::cout << "Coords : " << player.x << ' ' << player.y << std::endl; // Player Coords
    std::cout << "Health : " << player.health << "/1" << std::endl; // Player Health
    std::cout << "Oxygen : " << player.oxygen << "/3" << std::endl; // Player Oxygen
    std::cout << "Your turn : ";
}


bool IsInMap(int x, int y, Map map) {
    if (x < 0 or x > map.height - 1) {
        return false;
    }
    if (y < 0 or y > map.width - 1) {
        return false;
    }
    return true;
}


void Collision(Entity& player, Entity ennemies[]) {
    for (int i = 0; i < 5; i++) {
        if (player.x == ennemies[i].x and player.y == ennemies[i].y) {
            player.health -= ennemies[i].damage;
        }
    }
}


void OnTile(Entity& player, Map map, bool& victory, int currentMap) {
    int PlayerCell = GetCell(player.x, player.y, map.height);

    switch (map.data[PlayerCell])
    {
    case Ground:
        player.oxygen = 3;
        break;
    case Water:
        player.oxygen--;
        // When no oxygen
        if (player.oxygen == 0) {
            player.health = 0;
            player.x = -2;
            player.y = -2;
        }
        break;
    case Mountain:
        player.oxygen = 3;
        break;
    case Lava:
        player.oxygen = 3;
        player.health--;
        break;
    case Tall_Grass:
        player.oxygen = 3;
        break;
    case Quicksand:
        player.oxygen = 3;
        break;
    case Door:
        player.oxygen = 3;
        victory = true;
        break;
    }
}


void UpdateEntities(Entity& player, Entity ennemies[], Map map, bool& victory, int currentMap) {

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    char imput = _getch();
    int xDir = 0;
    int yDir = 0;
    int slow;

    // Player Part
    switch (imput)
    {
    case 'd':
        xDir = 1;
        break;
    case 'q':
        xDir = -1;
        break;
    case 'z':
        yDir = -1;
        break;
    case 's':
        yDir = 1;
        break;
    }

    int newX = player.x + xDir;
    int newY = player.y + yDir;

    if (IsInMap(newX, newY, map)) {

        int tileType = map.data[GetCell(newX, newY, map.height)];
        if (tiles[tileType].walkable)
        {
            // If the tile is slow, 1 out of 2 chance of moving
            slow = rand() % 2;
            int currentTileType = map.data[GetCell(player.x, player.y, map.height)];
            if (tiles[currentTileType].isSlow) {
                switch (slow)
                {
                case 0:
                    player.x = newX;
                    player.y = newY;
                    break;
                case 1:
                    break;
                }
            }
            else {
                player.x = newX;
                player.y = newY;
            }
        }
    }

    // Ennemies Part
    int random;
    bool isOtherEnnemy = false;

    for (int i = 0;i < 5;i++) {

        int xDir = 0;
        int yDir = 0;

        // Random direction
        random = rand() % 4;

        switch (random)
        {
        case 0:
            xDir = 1;
            break;
        case 1:
            xDir = -1;
            break;
        case 2:
            yDir = -1;
            break;
        case 3:
            yDir = 1;
            break;
        }

        // Detects if there is another ennemy
        for (int k = 0; k < 5; k++) {
            if (k != i) {
                if (ennemies[k].x == ennemies[i].x + xDir and ennemies[k].y == ennemies[i].y + yDir) {
                    isOtherEnnemy = true;
                }
            }
        }
        isOtherEnnemy = false;

        if (!isOtherEnnemy) {
            if (xDir != 0 && yDir != 0)
                std::cout << "AIIIIIIIIIIIIIIIIIIIEEEEEEEEEE" << std::endl;
            int newX = ennemies[i].x + xDir;
            int newY = ennemies[i].y + yDir;

            if (IsInMap(newX, newY, map)) {

                int tileType = map.data[GetCell(newX, newY, map.height)];
                if (tiles[tileType].walkable)
                {
                    // If the tile is slow, 1 out of 2 chance of moving
                    slow = rand() % 2;
                    int currentTileType = map.data[GetCell(ennemies[i].x, ennemies[i].y, map.height)];
                    if (tiles[currentTileType].isSlow) {
                        switch (slow)
                        {
                        case 0:
                            ennemies[i].x = newX;
                            ennemies[i].y = newY;
                            break;
                        case 1:
                            break;
                        }
                    }
                    else {
                        ennemies[i].x = newX;
                        ennemies[i].y = newY;
                    }
                }
            }
        }
    }

    Collision(player, ennemies);

    OnTile(player, map, victory, currentMap);

}


void StartingScreen() {

    // Opening File
    std::ifstream start("menu/start.txt");

    std::string line;

    // Display starting screen
    while (std::getline(start, line)) {
        std::cout << line << std::endl;
    }

    // Closing File
    start.close();

    char idk;
    idk = getchar();

    system("cls");
}


void GameOver(Entity player) {

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Ending Animation
    for (int i = 1;i < 5;i++) {

        system("cls");

        std::string path = "menu/end";
        path.append(std::to_string(i));
        path.append(".txt");


        // Opening File
        std::ifstream ending(path);

        std::string line;
        // Display ending screen
        while (std::getline(ending, line)) {
            std::cout << line << std::endl;
        }

        // Closing File
        ending.close();

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << std::endl;
    // Ending Explanation
    if (player.oxygen == 0) {
        std::cout << "You have drowned!" << std::endl;
    }
    else {
        std::cout << "You have died!" << std::endl;
    }

}


void Victory() {
    system("cls");

    // Opening File
    std::ifstream start("menu/vicotry.txt");

    std::string line;

    // Display starting screen
    while (std::getline(start, line)) {
        std::cout << line << std::endl;
    }

    // Closing File
    start.close();

    std::cout << "You have conquered the world!" << std::endl;

}


int main()
{
    srand(time(0));

    // Def. variables
    Map map;

    int currentMap = 1;

    Entity player = { -2, -2, 'H', 1 };

    Entity ennemies[5] = {
        {-10, -10, 'X', 1, 1},
        {-10, -10, 'X', 1, 1},
        {-10, -10, 'X', 1, 1},
        {-10, -10, 'X', 1, 1},
        {-10, -10, 'X', 1, 1}
    };
    
    StartingScreen();

    // Creation and display first map
    map = CreateMap(currentMap, player, ennemies);
    if (map.name == "null") {
        Victory();
        return 5;
    }
    ShowMap(map, player, ennemies, currentMap);

    bool victory = false;

    // Game Loop
    while (player.health != 0) {

        UpdateEntities(player, ennemies, map, victory, currentMap);

        system("cls");

        ShowMap(map, player, ennemies, currentMap);

        // Victory
        if (victory) {
            victory = false;

            // New level
            MapReset(player, ennemies);

            currentMap++;
            map = CreateMap(currentMap, player, ennemies);
            if (map.name == "null") {
                Victory();
                return 5;
            }
            system("cls");
            ShowMap(map, player, ennemies, currentMap);
        }
    }
    GameOver(player);
    return 0;

}
