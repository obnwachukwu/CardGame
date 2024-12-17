#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define strdup _strdup

#define MAX_PLAYERS 4
#define CARDS_PER_PLAYER 13
#define DECK_SIZE 52

// Define the structure for a card
typedef struct {
    const char* name;
    int value;
    int isPlayed; // New field to track if the card has been played
} Card;

void shuffleDeck(Card deck[]) {
    for (int i = DECK_SIZE - 1; i > 0; i--) {
        int j = rand() % (i + 1);  // Get a random index between 0 and i
        // Swap cards at index i and j
        Card temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

void printPlayerCards(char* playerName, Card playerCards[], int numCards) {
    printf("Cards for %s:\n", playerName);
    for (int i = 0; i < numCards; i++) {
        if (!playerCards[i].isPlayed) { // Only show cards that haven't been played
            printf("%d: %s\n", i + 1, playerCards[i].name);  // Display card index and name
        }
    }
}

int getPlayerCardChoice(Card playerCards[], int numCards) {
    int cardChoice;
    printf("Please select a card by entering its number (1-%d): ", numCards);
    scanf("%d", &cardChoice);

    // Validate the input (ensure the card choice is within a valid range)
    while (cardChoice < 1 || cardChoice > numCards || playerCards[cardChoice - 1].isPlayed) {
        if (playerCards[cardChoice - 1].isPlayed) {
            printf("You have already played this card! ");
        }
        printf("Invalid choice! Please select a card between 1 and %d: ", numCards);
        scanf("%d", &cardChoice);
    }

    return cardChoice - 1;  // Convert to zero-based index
}

void saveGameState(Card playerCards[MAX_PLAYERS][CARDS_PER_PLAYER], int numOfPlayers, int round) {
    FILE* file = fopen("saved_game.txt", "w");  // Open a file for writing in text mode
    if (file == NULL) {
        printf("Error opening file for saving.\n");
        return;
    }

    // Save the number of players and current round
    fprintf(file, "%d\n", numOfPlayers);
    fprintf(file, "%d\n", round);

    // Save each player's cards and played status
    for (int i = 0; i < numOfPlayers; i++) {
        fprintf(file, "Player %d:\n", i + 1);
        for (int j = 0; j < CARDS_PER_PLAYER; j++) {
            // Write card information to file (name, value, isPlayed)
            fprintf(file, "%s,%d,%d\n", playerCards[i][j].name, playerCards[i][j].value, playerCards[i][j].isPlayed);
        }
    }

    fclose(file);
    printf("Game saved successfully!\n");
}

int loadGameState(Card playerCards[MAX_PLAYERS][CARDS_PER_PLAYER], int* numOfPlayers, int* round) {
    FILE* file = fopen("saved_game.txt", "r");  // Open the file for reading in text mode
    if (file == NULL) {
        printf("No saved game found!\n");
        return 0;  // Indicate that no saved game was found
    }

    // Load the number of players and round
    fscanf(file, "%d\n", numOfPlayers);
    fscanf(file, "%d\n", round);

    // Load each player's cards and played status
    for (int i = 0; i < *numOfPlayers; i++) {
        char playerName[30]; // Temporary variable for player name
        fscanf(file, "Player %d:\n", &i);  // Read player label (Player 1, Player 2, etc.)

        for (int j = 0; j < CARDS_PER_PLAYER; j++) {
            // Read card information from file (name, value, and played status)
            char cardName[50];  // Temporary buffer for card name
            int value, isPlayed;

            // Read card details (name, value, isPlayed)
            fscanf(file, "%49[^,],%d,%d\n", cardName, &value, &isPlayed);

            // Assign the card details to the playerCards array
            playerCards[i][j].name = strdup(cardName);  // Copy the card name
            playerCards[i][j].value = value;
            playerCards[i][j].isPlayed = isPlayed;
        }
    }

    fclose(file);
    printf("Game loaded successfully!\n");
    return 1;  // Indicate that the game was loaded successfully
}

void printGameStatus(Card playerCards[MAX_PLAYERS][CARDS_PER_PLAYER], int numOfPlayers) {
    printf("\n--- Game Status ---\n");
    for (int i = 0; i < numOfPlayers; i++) {
        printf("Player %d's cards:\n", i + 1);
        for (int j = 0; j < CARDS_PER_PLAYER; j++) {
            printf("%s (Value: %d) - %s\n", playerCards[i][j].name, playerCards[i][j].value,
                playerCards[i][j].isPlayed ? "Played" : "Not Played");
        }
        printf("\n");
    }
}

void main() {
    const char* cardNames[] = {
        "2 of Clubs", "3 of Clubs", "4 of Clubs", "5 of Clubs", "6 of Clubs", "7 of Clubs", "8 of Clubs", "9 of Clubs", "10 of Clubs", "Jack of Clubs", "Queen of Clubs", "King of Clubs", "Ace of Clubs",
        "2 of Diamonds", "3 of Diamonds", "4 of Diamonds", "5 of Diamonds", "6 of Diamonds", "7 of Diamonds", "8 of Diamonds", "9 of Diamonds", "10 of Diamonds", "Jack of Diamonds", "Queen of Diamonds", "King of Diamonds", "Ace of Diamonds",
        "2 of Hearts", "3 of Hearts", "4 of Hearts", "5 of Hearts", "6 of Hearts", "7 of Hearts", "8 of Hearts", "9 of Hearts", "10 of Hearts", "Jack of Hearts", "Queen of Hearts", "King of Hearts", "Ace of Hearts",
        "2 of Spades", "3 of Spades", "4 of Spades", "5 of Spades", "6 of Spades", "7 of Spades", "8 of Spades", "9 of Spades", "10 of Spades", "Jack of Spades", "Queen of Spades", "King of Spades", "Ace of Spades"
    };

    int cardValues[] = {
        2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
        2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
        2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
        2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14
    };

    Card deck[DECK_SIZE];
    Card playerCards[MAX_PLAYERS][CARDS_PER_PLAYER];
    int numOfPlayers = 0, round = 1;

    srand(time(NULL));  // Seed for random number generator

    // Ask the user whether they want to load the game or start fresh
    printf("Do you want to load the last saved game? (1 for yes, 0 for no): ");
    int loadChoice;
    scanf("%d", &loadChoice);

    if (loadChoice == 1) {
        // Load the saved game state
        if (!loadGameState(playerCards, &numOfPlayers, &round)) {
            printf("Starting a new game...\n");
        }
    }
    else {
        // Start a new game
        printf("Please enter the number of players (2 - 4): ");
        scanf("%d", &numOfPlayers);

        // Ensure valid number of players
        if (numOfPlayers < 2 || numOfPlayers > MAX_PLAYERS) {
            printf("Invalid number of players! The game supports 2 to 4 players.\n");
            return;
        }

        // Initialize deck of cards
        for (int i = 0; i < DECK_SIZE; i++) {
            deck[i].name = cardNames[i];
            deck[i].value = cardValues[i];
            deck[i].isPlayed = 0; // Mark all cards as not played initially
        }

        // Shuffle the deck
        shuffleDeck(deck);

        // Distribute cards to each player
        for (int i = 0; i < numOfPlayers; i++) {
            for (int j = 0; j < CARDS_PER_PLAYER; j++) {
                playerCards[i][j] = deck[i * CARDS_PER_PLAYER + j];  // Distribute cards to players
            }
        }
    }

    int choice;

    // Start the rounds
    while (1) {
        printf("\n--- Round %d ---\n", round);

        // Each player plays their turn
        for (int i = 0; i < numOfPlayers; i++) {
            char playerName[30];
            printf("Please enter name for Player%d: ", i + 1);
            scanf("%s", playerName);

            // Print player's cards (only unplayed cards)
            printPlayerCards(playerName, playerCards[i], CARDS_PER_PLAYER);

            // Ask the player to choose a card to play
            int cardChoice = getPlayerCardChoice(playerCards[i], CARDS_PER_PLAYER);

            // Mark the chosen card as played
            playerCards[i][cardChoice].isPlayed = 1;

            // Print the chosen card
            printf("%s has selected to play the card: %s\n", playerName, playerCards[i][cardChoice].name);
        }

        // Offer options after each round
        printf("\n1. Play another round\n");
        printf("2. Save the game\n");
        printf("3. Output game status\n");
        printf("4. Exit the game without saving\n");
        printf("Please select an option: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            round++;  // Increment the round
            break;
        case 2:
            saveGameState(playerCards, numOfPlayers, round); // Save the game state
            break;
        case 3:
            printGameStatus(playerCards, numOfPlayers);  // Output the game status
            break;
        case 4:
            printf("Exiting the game without saving.\n");
            return;
        default:
            printf("Invalid option!\n");
            break;
        }
    }
}
