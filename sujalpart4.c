//Part 4: Main Menu and Cleanup
void menu() {
    int choice;
    do {
        printf("\n=== SMART ENERGY ANALYZER ===\n");
        printf("1. Add Appliance\n");
        printf("2. Add Usage Entry\n");
        printf("3. Top 3 Consumers\n");
        printf("4. Monthly Chart\n");
        printf("5. Cost Estimate\n");
        printf("0. Exit\n");
        printf("Choice: ");
        
        choice = get_valid_int("", 0, 5);
        
        switch (choice) {
            case 1: add_appliance(); break;
            case 2: add_usage(); break;
            case 3: show_top_consumers(); break;
            case 4: show_monthly_chart(); break;
            case 5: {
                float tariff = get_valid_int("Enter tariff (₹/kWh): ", 1, 20) * 0.01;
                float total = 0;
                for (int i = 0; i < appliance_count; i++) {
                    total += get_monthly_consumption(i);
                }
                printf("Monthly bill: ₹%.2f\n", total * tariff);
                break;
            }
            case 0: break;
        }
    } while (choice != 0);
}


int main() {
    appliances = malloc(MAX_APPLIANCES * sizeof(Appliance));
    usages = malloc(MAX_USAGE * sizeof(UsageEntry));
    
    printf("Smart Energy Consumption Analyzer v1.0\n");
    menu();
    
  
    free(appliances);
    free(usages);
    
    printf("Thank you for using Energy Analyzer!\n");
    return 0;
}
