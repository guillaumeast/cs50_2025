#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void merge_sort(int length, pair unsorted[]);
void lock_pairs(void);
bool is_cycle(int winner, int loser);
void print_winner(void);

// Main
int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }
    pair_count = 0;

    // Query for votes
    int voter_count = get_int("Number of voters: ");
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        // Record references
        record_preferences(ranks);
    }

    // Compute results
    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            int winner = ranks[i];
            int loser = ranks[j];
            preferences[winner][loser]++;
        }
    }
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                // Add pair
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;

                // Increment pair count
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                // Add pair
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;

                // Increment pair count
                pair_count++;
            }
        }
    }
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    merge_sort(pair_count, pairs);
}

void merge_sort(int length, pair unsorted[])
{
    // Split
    if (length > 1)
    {
        // Sort left half
        int left_length = length / 2;         // Compute length
        pair left[left_length];               // Create array
        for (int i = 0; i < left_length; i++) // Populate array
        {
            left[i] = unsorted[i];
        }
        merge_sort(left_length, left); // Sort array

        // Sort right half
        int right_length = length - left_length;   // Compute length
        pair right[right_length];                  // Create array
        for (int i = left_length; i < length; i++) // Populate array
        {
            right[i - left_length] = unsorted[i];
        }
        merge_sort(right_length, right); // Sort array

        // Merge sorted halves
        for (int i = 0, left_i = 0, right_i = 0; i < length; i++)
        {
            // If left is empty => copy right in unsorted
            if (left_i == left_length)
            {
                unsorted[i] = right[right_i];
                right_i++;
            }
            // If right is empty => copy left in unsorted
            else if (right_i == right_length)
            {
                unsorted[i] = left[left_i];
                left_i++;
            }
            // Else => copy the greatest one
            else
            {
                // Compute left strength
                int left_winner = left[left_i].winner;
                int left_loser = left[left_i].loser;
                int left_strength = preferences[left_winner][left_loser];

                // Compute right strength
                int right_winner = right[right_i].winner;
                int right_loser = right[right_i].loser;
                int right_strength = preferences[right_winner][right_loser];

                // Copy the biggest one in unsorted
                if (left_strength >= right_strength)
                {
                    unsorted[i] = left[left_i];
                    left_i++;
                }
                else
                {
                    unsorted[i] = right[right_i];
                    right_i++;
                }
            }
        }
    }
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        // Init
        int winner = pairs[i].winner;
        int loser = pairs[i].loser;

        // Check if locking this pair would create a cycle loop in graph
        if (is_cycle(winner, loser))
        {
            // Don't lock
            locked[winner][loser] = false;
        }
        else
        {
            // Lock
            locked[winner][loser] = true;
        }
    }
}

// Returns true if locked[winner][loser] will create a circular loop
// Aka returns true if loser (or one of its own losers) wins against winner
bool is_cycle(int winner, int loser)
{
    // Check all locked[i][j] pairs where i == loser
    // Aka check all loser's locked winnning pairs
    for (int j = 0; j < candidate_count; j++)
    {
        // For each locked pair[i][j] where i == loser
        if (locked[loser][j])
        {
            // If this locked[i][j] pair wins against winner
            // Aka if this pair loser wins against initial winner
            if (j == winner)
            {
                // Circular loop will be created! => Return true
                return true;
            }
            // Else => Check if any of the locked[i][j] pairs where i == actual j would cycle
            // Aka else => Check if any of the loser's losers wins against initial winner
            else
            {
                if (is_cycle(winner, j) == true)
                {
                    return true;
                }
            }
        }
    }
    // If none of the locked[i][j] pairs where i == loser wins against initial winner
    // Aka if none of the loser's locked winnning pairs make him win against initial winner
    return false;
}

// Print the winner of the election
void print_winner(void)
{
    // For each candidate
    for (int candidate = 0; candidate < candidate_count; candidate++)
    {
        int target_count = 0;

        // Compute each edge
        // Aka count the number of times this candidate is targeted by an edge
        for (int opponent = 0; opponent < candidate_count; opponent++)
        {
            if (candidate != opponent)
            {
                if (locked[opponent][candidate])
                {
                    target_count++;
                }
            }
        }
        // Check if candidate is the source of the graph
        // Aka check if candidate is not targeted by any edge
        if (target_count == 0)
        {
            printf("%s\n", candidates[candidate]);
            break;
        }
    }
}
