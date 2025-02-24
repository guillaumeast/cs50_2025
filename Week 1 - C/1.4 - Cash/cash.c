#include <cs50.h>
#include <stdio.h>

// Prototypes
int find_greatest_coin(int change_owed, int start_index);

// Available coin values
int coin_values[] = {1, 5, 10, 25};

int main(void)
{
    // Ask change owed
    int change_owed;
    do
    {
        change_owed = get_int("Change owed: ");
    }
    while (change_owed < 0);

    // Output 0 if change_owed == 0
    if (change_owed == 0)
    {
        printf("0");
    }
    // Else, find greatest coin until change_owed = 0 and count the number of coins used
    else
    {
        int coins_count = 0;
        int start_index = 3;
        do
        {
            // Find greatest coin
            int greatest_coin_index = find_greatest_coin(change_owed, start_index);
            int greatest_coin_value = coin_values[greatest_coin_index];

            // Update number of coins used and remaining change_owed
            coins_count++;
            change_owed -= greatest_coin_value;

            // Update start index (coins greater than the current one are > change_owned
            // so we don't need to compute them again during next iterations)
            start_index = greatest_coin_index;
        }
        while (change_owed > 0);

        // Print number of coins used for change
        printf("%d\n", coins_count);
    }
}

// Return the index of the greatest coin value which is <= change_owned
// in range from index 0 to start_index of the coin_values array
int find_greatest_coin(int change_owed, int start_index)
{
    int greatest_coin_index = 0;
    for (int i = start_index; i >= 0; i--)
    {
        if (coin_values[i] <= change_owed)
        {
            greatest_coin_index = i;
            break;
        }
    }
    return greatest_coin_index;
}
