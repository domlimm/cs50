#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
}
pair;

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
void lock_pairs(void);
void print_winner(void);
bool cyclic_check(int currentWinner, int currentLoser);

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
    int voter_count = get_int("Number of voters: ");

    // Query for votes
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

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++) {
        if (strcmp(candidates[i], name) == 0) {
            ranks[rank] = i;
            return true;
        }
    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    for (int j = 0; j < candidate_count; j++) {
        for (int i = j + 1; i < candidate_count; i++) {
            preferences[ranks[j]][ranks[i]]++;
        }
    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    // Preferences integers of stringed candidates
    // Candidates string
    // pairs - struct -> int winner, loser
    // i & j -> based on candidates
    for (int i = 0; i < candidate_count; i++) {
        for (int j = i + 1; j < candidate_count; j++) {
            if (preferences[i][j] > preferences[j][i]) {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i]) {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    // {winner, loser}
    int prevWinner, prevLoser, nextWinner, nextLoser;
    pair tempPair;

    for (int i = 0; i < pair_count; i++) {
        tempPair = pairs[i];
        prevWinner = pairs[i].winner, prevLoser = pairs[i].loser;
        nextWinner = pairs[i + 1].winner, nextLoser = pairs[i + 1].loser;

        if (preferences[prevWinner][prevLoser] < preferences[nextWinner][nextLoser]) {
            pairs[i] = pairs[i + 1];
            pairs[i + 1] = tempPair;
        }
    }

    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    int tempWinner, tempLoser;

    for (int i = 0; i < candidate_count; i++) {
        tempWinner = pairs[i].winner, tempLoser = pairs[i].loser;

        locked[tempWinner][tempLoser] = true;

        if (cyclic_check(tempWinner, tempLoser)) {
            locked[tempWinner][tempLoser] = false;
        }
    }

    return;
}

bool cyclic_check(int currentWinner, int currentLoser) {
    // i.e. 4 candidates -> 4 3 2 1 , 1 2 3 4
    // A > B > C > D > A

    // Returns true if there's a cycle*

    // I.e current A>B, check if there's a B>A.
    // If there is, a cycle is found.

    // printf("start [%i, %i] %s\n", currentLoser, currentWinner, locked[currentLoser][currentWinner] ? "true" : "false");

    if (locked[currentLoser][currentWinner]) {
        return true;
    }

    for (int i = 0; i < candidate_count; i++) {
        // If current loser has a win, recursively check if
        // this next loser has a win.
        // printf("for [%i, %i] %s\n", currentLoser, i, locked[currentLoser][i] ? "true" : "false");

        if (locked[currentLoser][i])
        {
            if (cyclic_check(currentWinner, i))
            {
                return true;
            }
        }
    }

    return false;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    // Simply iterate through entire locked and find highest occurence of T
    int highest_count = 0, true_in_row = 0;
    int tabulateStrength[candidate_count];
    int truey = 0;

    for (int i = 0; i < candidate_count; i++) {
        truey = 0;
        for (int j = 0; j < candidate_count; j++) {
            if (locked[i][j] == true) {
                true_in_row++;
                truey++;
            }
            // For only 1 winner, if locked for all except itself is T,
            // it's the source.
            if (locked[i][i] == false && truey == candidate_count - 1) {
                printf("%s\n", candidates[i]);
                truey = -1;
                break;
            }
        }
        tabulateStrength[i] = true_in_row;
    }

    // More than 1 winner of the same strength (no. of T)
    if (truey != -1) {
        for (int i = 0; i < candidate_count; i++) {
            if (tabulateStrength[i] > highest_count) {
                highest_count = tabulateStrength[i];
            }
        }

        for (int i = 0; i < candidate_count; i++) {
            if (tabulateStrength[i] == highest_count) {
                printf("%s\n", candidates[i]);
            }
        }
    }

    return;
}
