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

typedef struct{
char name[50];
} maps;



Inventory* createInventory(int initialCapacity) {
    Inventory *inv = malloc(sizeof(Inventory));
    inv->items = calloc(initialCapacity, sizeof(Item));
    inv->capacity = initialCapacity;
    inv->itemcount = 0;
    return inv;
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
    printf("\nEnter your choice: ");
    scanf("%d", &startchoice);

    Character player;
    switch (startchoice) {
        case 1: case 2: case 3: case 4:
            player = freeCharacters[startchoice - 1];
            break;
        default:
            printf("Invalid choice, starting with Kevin.\n");
            player = freeCharacters[0];
            break;
    }
    
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
    
    printf("\nSelect Your Item (Durability Affects Health): ");
    scanf("%d", &startitemchoice);
    
    Item selecteditem;
    
    switch (startitemchoice) {
        case 1: case 2: case 3: case 4:
            selecteditem = freeitems[startitemchoice - 1];
            break;
        default:
            printf("Invalid choice, selecting Fire Blade.\n");
            selecteditem = freeitems[0];
            break;
    }
    
    
    
    
    printf("\nYou are starting with  %s and you are using %s Good Luck!\n", player.name,selecteditem.name);
    
    player.health += selecteditem.durability;
    
    if (selecteditem.ismagic) {
    player.mana += 20;
}

free(player.inventory->items);
    free(player.inventory);
    
    printf("\nUpdated Stats!\n");
    printf("--------------\n");
    printf("Your Health : %d\nYour Mana = %d \nYour Level %d" ,player.health,player.mana,player.level);



    
    
    for (int i = 0; i < player.inventory->itemcount; i++) {
    printf("%s\n", player.inventory->items[i].name);
}
    
    int experience = 0;
    int expToLevelUp = 50;
    int gold = 50;
    
    void openShop(Character *player);
    
    
    maps tmaps[4] = {
    {"Chilly 🤙"},
    {"Forest 🌴"},
    {"Desert 🏜️"},
    {"Island 🏝️"}
    };
    
    int mapchoice;
    
    printf("\n\nSelect Your Map");
    printf("\n-----------------\n");
    for (int z = 0; z < 4; z++) {
    printf("%d. %s\n", z + 1, tmaps[z].name);
}
printf("Your Choice: ");
scanf("%d",&mapchoice);

    
    FILE *f = fopen("save.txt", "w");
fprintf(f, "%s %d %d", player.name, player.health, player.level);
fclose(f);
return 0;
}
