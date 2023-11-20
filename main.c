#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "constants.h"
#include "error_handling.h"

typedef struct {
    char name[MAX_NAME_LENGTH];
    int wpm;
    float accuracy;
    char errorPatterns[MAX_EXERCISE_LENGTH];
} User;

typedef struct {
    char content[MAX_EXERCISE_LENGTH];
    int difficulty;
} Exercise;

Exercise* generateExercise(int userSkillLevel) {
    Exercise* exercise = malloc(sizeof(Exercise));
    if (!exercise) {
        HANDLE_ERROR("malloc");
    }
    exercise->content[0] = '\0';
    int numWords = userSkillLevel * 5;
    char words[NUM_WORDS][MAX_WORD_LENGTH] = {
            "apple", "banana", "cherry", "date", "elderberry",
            "fig", "grape", "honeydew", "iceberg", "jackfruit"
    };
    int i = 0;
    for (i = 0; i < numWords; i++) {
        int index = rand() % NUM_WORDS;
        strcat(exercise->content, words[index]);
        strcat(exercise->content, " ");
    }
    int size = strlen(exercise->content);
    exercise->content[size - 1] = '\0';
    exercise->difficulty = userSkillLevel;
    return exercise;
}

void calculateWpmAndAccuracy(User* user, char* typedText, char* exerciseText, time_t startTime, time_t endTime) {
    int typedLength = strlen(typedText);
    int exerciseLength = strlen(exerciseText);
    int correctChars = 0;

    for (int i = 0; i < typedLength && i < exerciseLength; i++) {
        if (typedText[i] == exerciseText[i]) {
            correctChars++;
        }
    }

    int timeTaken = endTime - startTime;
    user->wpm = (60 * correctChars / 5) / timeTaken;
    user->accuracy = 100.0 * (float)correctChars / exerciseLength;
}

void provideFeedback(User* user) {
    printf("Your current WPM is %d and your accuracy is %.2f%%\n", user->wpm, user->accuracy);
    if (user->accuracy < 80.0) {
        printf("You need to improve your accuracy. Keep practicing!\n");
    } if (user->wpm < 20) {
        printf("You need to improve your speed. Keep practicing!\n");
    } else {
        printf("Great job! Keep up the good work!\n");
    }
}

void displayInterface(User* user, Exercise* exercise) {
    printf("Welcome, %s! Here is your exercise:\n%s\n", user->name, exercise->content);
    char typedText[MAX_EXERCISE_LENGTH];
    printf("Please type the above text and press enter when done:\n");
    time_t startTime = time(NULL);
    fgets(typedText, MAX_EXERCISE_LENGTH, stdin);
    typedText[strcspn(typedText, "\n")] = 0;
    time_t endTime = time(NULL);
    calculateWpmAndAccuracy(user, typedText, exercise->content, startTime, endTime);
    provideFeedback(user);
}

void adaptSystem(User* user) {
    if (user->accuracy > 90.0 && user->wpm > 30) {
        printf("You're ready for the next level! Increasing difficulty...\n");
        int nextDifficulty = (user->wpm / 30) + 1;
        if (nextDifficulty > MAX_DIFFICULTY) {
            nextDifficulty = MAX_DIFFICULTY;
        }
        Exercise* nextExercise = generateExercise(nextDifficulty);
        displayInterface(user, nextExercise);
        free(nextExercise);
    } else {
        printf("Keep practicing to improve your WPM and accuracy!\n");
    }
}

int main() {
    srand(time(NULL));
    User user;
    strcpy(user.name, "Test User");
    Exercise* exercise = generateExercise(1);
    displayInterface(&user, exercise);
    adaptSystem(&user);
    free(exercise);
    return 0;
}