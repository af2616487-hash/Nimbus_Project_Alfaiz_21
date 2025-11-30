//Part 1: Data Structures and Input Handling
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 50
#define MAX_APPLIANCES 100
#define MAX_USAGE 1000


typedef struct {
    char name[MAX_NAME_LEN];
    int power_rating;  
} Appliance;


typedef struct {
    int appliance_id;
    char start_time[10];  
    char end_time[10];    
    int days_used;
} UsageEntry;


Appliance *appliances = NULL;
UsageEntry *usages = NULL;
int appliance_count = 0;
int usage_count = 0;


int get_valid_int(char *prompt, int min, int max) {
    int value;
    char buffer[20];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin)) {
            if (sscanf(buffer, "%d", &value) == 1 && value >= min && value <= max) {
                return value;
            }
        }
        printf("Invalid input! Enter number between %d-%d: ", min, max);
    }
}


void add_appliance() {
    if (appliance_count >= MAX_APPLIANCES) {
        printf("Max appliances reached!\n");
        return;
    }
    
    appliances = realloc(appliances, (appliance_count + 1) * sizeof(Appliance));
    
    Appliance *new_app = &appliances[appliance_count];
    printf("Enter appliance name: ");
    fgets(new_app->name, MAX_NAME_LEN, stdin);
    new_app->name[strcspn(new_app->name, "\n")] = 0;  
    
    new_app->power_rating = get_valid_int("Enter power rating (watts): ", 1, 5000);
    
    printf("Appliance '%s' added with ID: %d\n", new_app->name, appliance_count);
    appliance_count++;
}
