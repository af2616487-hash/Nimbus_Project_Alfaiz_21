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
    if (!appliances) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    
    Appliance *new_app = &appliances[appliance_count];
    printf("Enter appliance name: ");
    fgets(new_app->name, MAX_NAME_LEN, stdin);
    new_app->name[strcspn(new_app->name, "\n")] = 0;
    
    new_app->power_rating = get_valid_int("Enter power rating (watts): ", 1, 5000);
    
    printf("Appliance '%s' added with ID: %d\n", new_app->name, appliance_count);
    appliance_count++;
}


float calculate_hours_used(char *start, char *end) {
    int start_h, start_m, end_h, end_m;
    sscanf(start, "%d:%d", &start_h, &start_m);
    sscanf(end, "%d:%d", &end_h, &end_m);
    
    int start_total = start_h * 60 + start_m;
    int end_total = end_h * 60 + end_m;
    
    int diff = end_total - start_total;
    if (diff < 0) diff += 24 * 60;
    
    return diff / 60.0;
}


void add_usage() {
    if (appliance_count == 0) {
        printf("Add appliances first!\n");
        return;
    }
    
    printf("Available appliances:\n");
    for (int i = 0; i < appliance_count; i++) {
        printf("%d. %s (%dW)\n", i, appliances[i].name, appliances[i].power_rating);
    }
    
    int app_id = get_valid_int("Select appliance ID: ", 0, appliance_count - 1);
    
    usages = realloc(usages, (usage_count + 1) * sizeof(UsageEntry));
    if (!usages) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    UsageEntry *new_usage = &usages[usage_count];
    
    new_usage->appliance_id = app_id;
    printf("Enter start time (HH:MM): ");
    fgets(new_usage->start_time, 10, stdin);
    new_usage->start_time[strcspn(new_usage->start_time, "\n")] = 0;
    
    printf("Enter end time (HH:MM): ");
    fgets(new_usage->end_time, 10, stdin);
    new_usage->end_time[strcspn(new_usage->end_time, "\n")] = 0;
    
    new_usage->days_used = get_valid_int("Days used: ", 1, 31);
    
    float hours = calculate_hours_used(new_usage->start_time, new_usage->end_time);
    float kwh = (appliances[app_id].power_rating * hours * new_usage->days_used) / 1000.0;
    
    printf("Session energy: %.2f kWh\n", kwh);
    usage_count++;
}


float get_monthly_consumption(int app_id) {
    float total = 0;
    for (int i = 0; i < usage_count; i++) {
        if (usages[i].appliance_id == app_id) {
            float hours = calculate_hours_used(usages[i].start_time, usages[i].end_time);
            total += (appliances[app_id].power_rating * hours * usages[i].days_used) / 1000.0;
        }
    }
    return total;
}


void show_top_consumers() {
    if (usage_count == 0) {
        printf("No usage data!\n");
        return;
    }
    
    float consumption[MAX_APPLIANCES];
    int index[MAX_APPLIANCES];
    for (int i = 0; i < appliance_count; i++) {
        consumption[i] = get_monthly_consumption(i);
        index[i] = i;
    }
    
    
    for (int i = 0; i < 3 && i < appliance_count; i++) {
        for (int j = i + 1; j < appliance_count; j++) {
            if (consumption[j] > consumption[i]) {
                float temp_c = consumption[i];
                consumption[i] = consumption[j];
                consumption[j] = temp_c;
                int temp_idx = index[i];
                index[i] = index[j];
                index[j] = temp_idx;
            }
        }
    }
    
    printf("\n=== TOP 3 HIGHEST CONSUMING APPLIANCES ===\n");
    for (int i = 0; i < 3 && i < appliance_count; i++) {
        printf("%d. %s: %.2f kWh/month\n", i + 1, appliances[index[i]].name, consumption[i]);
    }
}


void show_monthly_chart() {
    if (appliance_count == 0) {
        printf("No appliances added!\n");
        return;
    }
    printf("\n=== MONTHLY CONSUMPTION CHART ===\n");
    for (int i = 0; i < appliance_count; i++) {
        float kwh = get_monthly_consumption(i);
        printf("%-15s: ", appliances[i].name);
        
        int bars = (int)(kwh * 2);
        for (int j = 0; j < bars && j < 50; j++) {
            printf("█");
        }
        printf(" %.2f kWh\n", kwh);
    }
}



void show_cost_estimate() {
    if (usage_count == 0) {
        printf("No usage data!\n");
        return;
    }
    float tariff = get_valid_int("Enter tariff (₹/kWh): ", 1, 100);
    tariff /= 100.0;
    
    float total = 0;
    for (int i = 0; i < appliance_count; i++) {
        total += get_monthly_consumption(i);
    }
    
    printf("Estimated monthly bill: ₹%.2f\n", total * tariff);
}

void menu() {
    int choice;
    do {
        printf("\n=== SMART ENERGY ANALYZER ===\n");
        printf("1. Add Appliance\n");
        printf("2. Add Usage Entry\n");
        printf("3. Show Top 3 Consumers\n");
        printf("4. Show Monthly Consumption Chart\n");
        printf("5. Show Cost Estimate\n");
        printf("0. Exit\n");
        printf("Choice: ");
        
        choice = get_valid_int("", 0, 5);
        
        switch (choice) {
            case 1:
                add_appliance();
                break;
            case 2:
                add_usage();
                break;
            case 3:
                show_top_consumers();
                break;
            case 4:
                show_monthly_chart();
                break;
            case 5:
                show_cost_estimate();
                break;
            case 0:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 0);
}

int main() {
    appliances = NULL;
    usages = NULL;
    appliance_count = 0;
    usage_count = 0;
    
    printf("Smart Energy Consumption Analyzer\n");
    
    menu();
    
    
    free(appliances);
    free(usages);
    
    return 0;
}
