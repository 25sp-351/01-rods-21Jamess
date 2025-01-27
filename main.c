#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 100

// Define a structure to store the rod data
typedef struct {
    int lengths[MAX_LEN];
    int values[MAX_LEN];
    int cuts[MAX_LEN];
} RodData;

// Parameters: rod_data - store data input
// Parameters: count - store the number of items read

void read_input(RodData *rod_data, int *count) {
    char buffer[256];
    *count = 0;

    while (fgets(buffer, sizeof(buffer), stdin)) {
        int length, value;
        if (sscanf(buffer, "%d, %d", &length, &value) == 2) {
            if (length > 0 && value > 0) {
                rod_data->lengths[*count] = length;
                rod_data->values[*count]  = value;
                (*count)++;
            } else {
                fprintf(stderr,
                        "Invalid input: length and value must be positive.\n");
            }
        } else {
            fprintf(stderr, "Invalid input format: %s", buffer);
        }
    }
}
// Parameters: rod_data - pointer to rod data
// Parameters: count - number of avaliable rod lengths
// Parameters: remaining_length - remaining length of the rod
// Parameters: cuts - array to store the count of cuts for each length

int maximize_value(const RodData *rod_data, int count, int remaining_length,
                   int cuts[]) {
    if (remaining_length == 0)
        return 0;  // Base case: no remaining length

    int max_value = 0;

    for (int i = 0; i < count; i++) {
        if (rod_data->lengths[i] <= remaining_length) {
            // Make the cut and recurse
            int temp_cuts[MAX_LEN] = {0};
            int current_value =
                rod_data->values[i] +
                maximize_value(rod_data, count,
                               remaining_length - rod_data->lengths[i],
                               temp_cuts);

            if (current_value > max_value) {
                max_value = current_value;

                // Update the cuts array with the current choice
                for (int j = 0; j < count; j++)
                    cuts[j] = temp_cuts[j];
                cuts[i]++;
            }
        }
    }

    return max_value;
}

// Main function to read input, compute optimal cuts, and output results.
int main(int argc, char *argv[]) {
    // Check if the correct number of arguments is provided
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <rod_length>\n", argv[0]);
        return 1;
    }

    int rod_length = atoi(argv[1]);
    if (rod_length <= 0) {
        fprintf(stderr, "Invalid rod length. Must be a positive integer.\n");
        return 1;
    }

    // Declare and initialize variables
    RodData rod_data;
    int count;
    int total_value;
    int remaining_length;
    int used_length = 0;

    memset(rod_data.cuts, 0, sizeof(rod_data.cuts));  // Initialize cuts array

    // Read input data
    read_input(&rod_data, &count);

    // Compute the maximum value and track the cuts
    total_value = maximize_value(&rod_data, count, rod_length, rod_data.cuts);

    // Output the results
    printf("Cutting List:\n");
    for (int i = 0; i < count; i++) {
        if (rod_data.cuts[i] > 0) {
            printf("%d @ %d = %d\n", rod_data.cuts[i], rod_data.lengths[i],
                   rod_data.cuts[i] * rod_data.values[i]);
            used_length += rod_data.cuts[i] * rod_data.lengths[i];
        }
    }

    // Calculate the remainder
    remaining_length = rod_length - used_length;

    printf("Remainder: %d\n", remaining_length);
    printf("Value: %d\n", total_value);

    return 0;
}
