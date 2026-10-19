#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>

#include <curses.h>



struct Map {
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
    Door = 3
};

struct Tile {
    char name;
    bool walkable = true;
    bool victory = false;
};

// Tiles
const Tile ground = { '.' };
const Tile water = { '~' };
const Tile mountain = { 'M', false };
const Tile door = { 'E', true, true };

Tile const tiles[4] = { ground, water, mountain, door };



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

    std::string s;
    int lineMax = 0;
    int columnMax = 0;

    // Def. Map Size
    while (std::getline(level, s)) {
        columnMax += 1;
        lineMax = s.size();
    }

    map.height = lineMax;
    map.width = columnMax;
    map.data = new int[map.height * map.width];



    // Def. Map Data
    std::string d;
    int n = -1;
    level.clear();
    level.seekg(0);
    int line = 0;
    int currentEnnemy = 0;

    while (std::getline(level, d)) {

        for (int i = 0;i < lineMax;i++) {

            n++;

            // Detects Player
            if (d[i] == 'H') {
                map.data[n] = 0;
                player.x = i;
                player.y = line;
            }
            // Detects Ennemy
            else if (d[i] == 'X') {
                map.data[n] = 0;
                ennemies[currentEnnemy].x = i;
                ennemies[currentEnnemy].y = line;
                currentEnnemy++;
            }

            for (int t = 0; t < 4; t++) {
                

                if (d[i] == tiles[t].name) {
                    map.data[n] = t;
                }
            }
        }
        line++;
    }

    // Closing File
    level.close();

    return map;
}


void ShowMap(Map map, Entity player, Entity enemies[], int currentMap) {

    for (int y = 0; y < map.width; ++y) {
        for (int x = 0; x < map.height; ++x) {

            int index = GetCell(x, y, map.height);

            // Detects Ennemies
            bool isenemy = false;
            int whenenemy;

            for (int i = 0; i < 5; i++) {
                if (x == enemies[i].x and y == enemies[i].y) {
                    isenemy = true;
                    whenenemy = i;
                }
            }

            // Display Ennemies
            if (isenemy) {
                std::cout << enemies[whenenemy].name << ' ';
            }

            // Display Player
            else if (x == player.x and y == player.y) {
                std::cout << player.name << ' ';
            }

            // Display Map
            else {
                for (int t = 0; t < 4; t++) {
                    if (map.data[index] == Tile_Type(t))
                        std::cout << tiles[t].name << ' ';
                }
            }
        }
        std::cout << std::endl;
    }

    // External Informations
    std::cout << std::endl;
    std::cout << "Level " << currentMap << std::endl;
    std::cout << "Coords : " << player.x << ' ' << player.y << std::endl;
    std::cout << "Health : " << player.health << "/1" << std::endl;
    std::cout << "Oxygen : " << player.oxygen << "/3" << std::endl;
    std::cout << "Next Order : ";
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


void UpdatePlayer(Entity& player, Map map, char order) {

    int xDir = 0;
    int yDir = 0;

    switch (order)
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
            player.x = newX;
            player.y = newY;
        }
    }
}


void UpdateEnnemies(Entity ennemies[], Map map) {


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
                    ennemies[i].x = newX;
                    ennemies[i].y = newY;
                }
            }
        }
    }
}


void Collision(Entity& player, Entity ennemies[]) {
    for (int i = 0; i < 5; i++) {
        if (player.x == ennemies[i].x and player.y == ennemies[i].y) {
            player.health -= ennemies[i].damage;
        }
    }
}


void OnTile(Entity& player, Map map, bool& victory) {
    int PlayerCell = GetCell(player.x, player.y, map.height);

    switch (map.data[PlayerCell])
    {
    case 0: // Ground
        player.oxygen = 3;
        break;
    case 1: // Water
        player.oxygen -= 1;

        if (player.oxygen == 0) {
            player.health = 0;
        }
        break;
    case 2: // Mountain
        player.oxygen = 3;
        break;
    case 3: // Exit
        player.oxygen = 3;
        victory = true;
        break;
    }
}


void GameOver(Entity player) {

    std::cout << '\n' << std::endl;
    std::cout << "Game Over" << std::endl;
    if (player.oxygen == 0) {
        std::cout << "You have drowned!" << std::endl;
    }
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

    // Creation and display first map
    map = CreateMap(currentMap, player, ennemies);
    if (map.height == 0 and map.width == 0) {
        return 5;
    }
    ShowMap(map, player, ennemies, currentMap);


    char order;
    bool victory = false;

    // Game Loop
    while (player.health != 0) {

        std::cin >> order;

        UpdatePlayer(player, map, order);

        UpdateEnnemies(ennemies, map);

        Collision(player, ennemies);

        OnTile(player, map, victory);



        system("cls");

        ShowMap(map, player, ennemies, currentMap);

        // Victory
        if (victory) {
            victory = false;

            // New level
            MapReset(player, ennemies);

            currentMap += 1;
            map = CreateMap(currentMap, player, ennemies);
            if (map.height == 0 and map.width == 0) {
                return 5;
            }
            system("cls");
            ShowMap(map, player, ennemies, currentMap);
        }
    }

    GameOver(player);
    return 0;
}
