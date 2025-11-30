//Part 2: Energy and Cost Calculations

float calculate_hours_used(char *start, char *end) {
    int start_h, start_m, end_h, end_m;
    sscanf(start, "%d:%d", &start_h, &start_m);
    sscanf(end, "%d:%d", &end_h, &end_m);
    
    float start_total = start_h * 60 + start_m;
    float end_total = end_h * 60 + end_m;
    
    return (end_total - start_total) / 60.0;  
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
