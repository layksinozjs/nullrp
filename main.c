#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

typedef struct {
    char name[50];
    int power;
    int durability;
    bool ismagic;
} Item;

typedef struct {
    Item *items;
    int capacity;
    int itemcount;
} Inventory;

typedef struct {
    char name[50];
    int health;
    int mana;
    int level;
    Inventory *inventory;
} Character;

typedef struct {
    char name[50];
    int health;
    int attack;
} Enemy;

typedef struct {
    char name[50];
} maps;

Inventory* createInventory(int initialCapacity) {
    Inventory *inv = malloc(sizeof(Inventory));
    inv->items = calloc(initialCapacity, sizeof(Item));
    inv->capacity = initialCapacity;
    inv->itemcount = 0;
    return inv;
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void saveGame(Character *player, maps selectedMap) {
    FILE *f = fopen("save.txt", "w");
    if (f == NULL) {
        printf("Save file could not be created.\n");
        return;
    }

    fprintf(f, "NAME:%s\n", player->name);
    fprintf(f, "HEALTH:%d\n", player->health);
    fprintf(f, "MANA:%d\n", player->mana);
    fprintf(f, "LEVEL:%d\n", player->level);
    fprintf(f, "MAP:%s\n", selectedMap.name);
    fprintf(f, "ITEMCOUNT:%d\n", player->inventory->itemcount);
    for (int i = 0; i < player->inventory->itemcount; i++) {
        Item item = player->inventory->items[i];
        fprintf(f, "ITEM:%s,%d,%d,%d\n", item.name, item.power, item.durability, item.ismagic);
    }

    fclose(f);
    printf("Game saved successfully.\n");
}

int main() {
    Item freeitems[4] = {
        {"Fire Blade", 25, 35, false},
        {"Lominal Axe", 35, 25, false},
        {"Bow of Magic", 18, 40, true},
        {"Blessed Katana", 20, 40, false}
    };

    Character freeCharacters[4] = {
        {"Kevin", 90, 10, 1, NULL},
        {"Londa", 75, 30, 1, NULL},
        {"Huzk", 100, 5, 1, NULL},
        {"Garret", 130, 0, 1, NULL}
    };

    printf("   O\n");
    printf("  /|\\ Welcome To NullRP\n");
    printf("  / \\\n");
    printf("------------------------\n");

    printf("Select Your Character:\n");
    for(int i = 0; i < 4; i++){
        printf("%d. %s - Health: %d, Mana: %d, Level: %d\n",
               i + 1,
               freeCharacters[i].name,
               freeCharacters[i].health,
               freeCharacters[i].mana,
               freeCharacters[i].level);
    }

    int startchoice = 0;
    int inputStatus;

    do {
        printf("\nEnter your choice: ");
        inputStatus = scanf("%d", &startchoice);
        while(getchar() != '\n'); // input temizliği
        if (inputStatus != 1 || startchoice < 1 || startchoice > 4) {
            printf("Invalid choice select between 1-4.\n");
        }
    } while (inputStatus != 1 || startchoice < 1 || startchoice > 4);

    Character player = freeCharacters[startchoice - 1];
    player.inventory = createInventory(10);

    printf("\nAvailable Items:\n");
    for (int j = 0; j < 4; j++) {
        printf("%d. %s - Power: %d, Durability: %d, Magic: %s\n",
               j + 1,
               freeitems[j].name,
               freeitems[j].power,
               freeitems[j].durability,
               freeitems[j].ismagic ? "Yes" : "No");
    }

    int startitemchoice;

    do {
        printf("\nSelect Your Item (Durability Affects Health): ");
        inputStatus = scanf("%d", &startitemchoice);
        while(getchar() != '\n');
        if (inputStatus != 1 || startitemchoice < 1 || startitemchoice > 4) {
            printf("Invalid choice select between 1-4.\n");
        }
    } while (inputStatus != 1 || startitemchoice < 1 || startitemchoice > 4);

    Item selecteditem = freeitems[startitemchoice - 1];
    player.inventory->items[player.inventory->itemcount++] = selecteditem;

    printf("\nYou are starting with  %s and you are using %s Good Luck!\n", player.name, selecteditem.name);

    player.health += selecteditem.durability;
    if (selecteditem.ismagic) {
        player.mana += 20;
    }

    printf("\nUpdated Stats!\n");
    printf("--------------\n");
    printf("Your Health : %d\nYour Mana = %d \nYour Level %d\n" ,player.health,player.mana,player.level);

    for (int i = 0; i < player.inventory->itemcount; i++) {
        printf("%s\n", player.inventory->items[i].name);
    }

    maps tmaps[4] = {
        {"Chilly 🤙"},
        {"Forest 🌴"},
        {"Desert 🏜️"},
        {"Island 🏝️"}
    };

    maps selectedMap;

    printf("\n\nSelect Your Map");
    printf("\n-----------------\n");
    for (int z = 0; z < 4; z++) {
        printf("%d. %s\n", z + 1, tmaps[z].name);
    }

    int mapchoice;

    do {
        printf("Your Choice (1-4): ");
        inputStatus = scanf("%d", &mapchoice);
        while(getchar() != '\n');
        if (inputStatus != 1 || mapchoice < 1 || mapchoice > 4) {
            printf("Invalid choice select between 1-4.\n");
        }
    } while (inputStatus != 1 || mapchoice < 1 || mapchoice > 4);

    selectedMap = tmaps[mapchoice - 1];

    printf("\nTeleporting you to %s...", selectedMap.name);
    clearScreen();
    printf("\033[2J\033[H");

    int experience = 0;
    int expToLevelUp = 50;
    int gold = 50;

    // Main Menu
    saveGame(&player,selectedMap);
    printf("\n   O   Main Menu\n");
    printf("  /|\\  %s / %s\n", player.name, selecteditem.name);
    printf("  / \\  %s\n", selectedMap.name);
    printf("------------------------\n");
    printf("\nStats:\n");
    printf("--------------\n");
    printf("Your Health : %d\nYour Mana = %d \nYour Level %d\n" ,player.health,player.mana,player.level);
    printf("\nInventory:\n");

    for (int i = 0; i < player.inventory->itemcount; i++) {
        printf("- %s (Power: %d, Durability: %d, Magic: %s)\n",
            player.inventory->items[i].name,
            player.inventory->items[i].power,
            player.inventory->items[i].durability,
            player.inventory->items[i].ismagic ? "Have Magic" : "Not Magic");
    }

    int choice = 0;
    do {
        printf("\n===== Menu =====\n");
        printf("1. View Stats\n");
        printf("2. View Inventory\n");
        printf("3. Save Game\n");
        printf("4. Exit Game\n");
        printf("5. Get into a fight\n");
        printf("6. Go for a loot\n");
        printf("7. Gamble\n");
        printf("Your Choice: ");
        inputStatus = scanf("%d", &choice);
        while(getchar() != '\n');

        switch (choice) {
            case 1:
                printf("\n--- Stats ---\n");
                printf("Name: %s\n", player.name);
                printf("Health: %d\n", player.health);
                printf("Mana: %d\n", player.mana);
                printf("Level: %d\n", player.level);
                break;
            case 2:
                printf("\n--- Inventory ---\n");
                for (int i = 0; i < player.inventory->itemcount; i++) {
                    printf("- %s (Power: %d, Durability: %d, Magic: %s)\n",
                           player.inventory->items[i].name,
                           player.inventory->items[i].power,
                           player.inventory->items[i].durability,
                           player.inventory->items[i].ismagic ? "Yes" : "No");
                }
                break;
            case 3:
                saveGame(&player, selectedMap);
                break;
            case 4:
                printf("dont forget to come back!\n");
                free(player.inventory->items);
                free(player.inventory);
                return 0;
            case 5:
                printf("Fight feature coming soon!\n");
                break;
            case 6:
                printf("Looting not yet implemented.\n");
                break;
            case 7:
                printf("Gambling will be added in the next version.\n");
                break;
            default:
                printf("Invalid choice, please select between 1-7.\n");
        }
    } while (1);

    return 0;
}
