#include <iostream>
#include <cstdlib>
#include <ctime>



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



int getCell(int x, int y, int columnMax)
{
    return y * columnMax + x;
}


void afficherMap(int map[], Entity player, Entity enemies[5]) {

    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 20; ++x) {

            int index = getCell(x, y, 20);

            // Detection enemis
            bool isenemy = false;
            int whenenemy;

            for (int i = 0; i < 5; i++) {
                if (x == enemies[i].x and y == enemies[i].y) {
                    isenemy = true;
                    whenenemy = i;
                }
            }

            // Affichage ennemis
            if (isenemy) {
                std::cout << enemies[whenenemy].name << ' ';
            }

            // Affichage joueur
            else if (x == player.x and y == player.y) {
                std::cout << player.name << ' ';
            }

            // Affichage Map
            else {
                for (int t = 0; t < 4; t++) {
                    if (map[index] == Tile_Type(t))
                        std::cout << tiles[t].name << ' ';
                }
            }
        }
        std::cout << std::endl;
    }

    // Affichage externe
    std::cout << std::endl;
    std::cout << "Coords : " << player.x << ' ' << player.y << std::endl;
    std::cout << "Health : " << player.health << "/1" << std::endl;
    std::cout << "Next Order : ";
}


bool IsInMap(int x, int y) {
    if (x < 0 or x > 19) {
        return false;
    }
    if (y < 0 or y > 9) {
        return false;
    }
    return true;
}


void UpdatePlayer(Entity& player, int map[], char order) {

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

    if (IsInMap(newX, newY)) {

        int tileType = map[getCell(newX, newY, 20)];
        if (tiles[tileType].walkable)
        {
            player.x = newX;
            player.y = newY;
        }
    }
}


void UpdateEnnemies(Entity ennemies[], int map[]) {


    int random;
    bool isOtherEnnemy = false;

    for (int i = 0;i < 5;i++) {

        int xDir = 0;
        int yDir = 0;

        // Direction Aléatoire
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

        // Detection autres ennemis
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

            if (IsInMap(newX, newY)) {

                int tileType = map[getCell(newX, newY, 20)];
                if (tiles[tileType].walkable)
                {
                    ennemies[i].x = newX;
                    ennemies[i].y = newY;
                }
            }
        }
    }
}


void Collision(Entity& player, Entity ennemies[5]) {
    for (int i = 0; i < 5; i++) {
        if (player.x == ennemies[i].x and player.y == ennemies[i].y) {
            player.health -= ennemies[i].damage;
        }
    }
}


int main()
{
    srand(time(0));

    int rowMax = 10;
    int columnMax = 20;

    int map[200] = {
        0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,3,0,
        0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,2,
        0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,2,
        0,0,0,0,0,0,0,0,0,2,2,1,1,1,1,0,0,0,0,2,
        1,1,1,0,0,0,0,0,0,2,2,1,1,0,0,0,0,0,0,0,
        0,0,1,1,1,1,0,0,0,2,2,1,1,1,1,1,1,1,1,1,
        0,0,0,0,0,1,1,0,0,0,1,1,1,1,0,0,0,0,0,0,
        0,0,0,0,0,0,1,0,0,0,0,1,1,0,0,0,0,0,0,2,
        0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,2,2,2,2,2
    };

    int currentMap = 0;

    Entity player = { 2, 3, 'H', 1 };

    Entity ennemies[5] = {
        {1, 1, 'X', 1, 1},
        {9, 2, 'X', 1, 1},
        {2, 8, 'X', 1, 1},
        {17, 3, 'X', 1, 1},
        {16, 7, 'X', 1, 1}
    };

    char order;
    bool victory = false;

    afficherMap(map, player, ennemies);

    while (player.health != 0) {

        std::cin >> order;

        UpdatePlayer(player, map, order);

        UpdateEnnemies(ennemies, map);

        Collision(player, ennemies);



        system("cls");

        afficherMap(map, player, ennemies);

        if (victory) {
            std::cout << "vicotry" << std::endl;;
            victory = false;
            currentMap += 1;
            break;
        }
    }

    std::cout << "end game" << std::endl;
    return 0;
}
