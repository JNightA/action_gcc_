
// v2
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void set_prop(const char *file_path, int target_value) {
    FILE *file = fopen(file_path, "w");
    if (file != NULL) {
        fprintf(file, "%d", target_value);
        fclose(file);
    }
}

int main() {
    char buffer[128];
    static char prev_status[128] = "";

    while (1) {
        FILE *fp = fopen("/sys/class/power_supply/battery/status", "r");
        if (fp && fgets(buffer, sizeof(buffer), fp)) {
            buffer[strcspn(buffer, "\n")] = 0;
            if (strcmp(buffer, prev_status) != 0) {
                if (strstr(buffer, "Discharging")) {
                    set_prop("/sys/class/power_supply/battery/constant_charge_current", 6000000);
                } else {
                    set_prop("/sys/class/power_supply/battery/constant_charge_current", 24000000);
                }
                strncpy(prev_status, buffer, sizeof(prev_status) - 1);
                prev_status[sizeof(prev_status) - 1] = '\0';
            }
            fclose(fp);
        }
        sleep(1);
    }
    return 0;
}
