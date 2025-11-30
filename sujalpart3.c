//Part 3: Reporting and Visualization
// Part 3: Top 3 highest consuming appliances
void show_top_consumers() {
    if (usage_count == 0) {
        printf("No usage data!\n");
        return;
    }
    
    float consumption[MAX_APPLIANCES];
    for (int i = 0; i < appliance_count; i++) {
        consumption[i] = get_monthly_consumption(i);
    }
    
    
    for (int i = 0; i < 3 && i < appliance_count; i++) {
        for (int j = i + 1; j < appliance_count; j++) {
            if (consumption[i] < consumption[j]) {
                float temp = consumption[i];
                consumption[i] = consumption[j];
                consumption[j] = temp;
                
                Appliance temp_app = appliances[i];
                appliances[i] = appliances[j];
                appliances[j] = temp_app;
            }
        }
    }
    
    printf("\n=== TOP 3 HIGHEST CONSUMING APPLIANCES ===\n");
    for (int i = 0; i < 3 && i < appliance_count; i++) {
        printf("%d. %s: %.2f kWh/month\n", i + 1, appliances[i].name, consumption[i]);
    }
}


void show_monthly_chart() {
    printf("\n=== MONTHLY CONSUMPTION CHART ===\n");
    for (int i = 0; i < appliance_count; i++) {
        float kwh = get_monthly_consumption(i);
        printf("%s: ", appliances[i].name);
        
        int bars = (int)(kwh * 2);  
        for (int j = 0; j < bars && j < 50; j++) {
            printf("█");
        }
        printf(" %.2f kWh\n", kwh);
    }
}
