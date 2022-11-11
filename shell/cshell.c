#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#define maxStorage 50
#define maxEnvVarStorage 50
#define maxCmdLen 50
#define maxArgLen 50
#define maxTimeLen 100

struct Commands 
{
    char *commandName;
    char *time;
    int value;
};

struct EnvVar 
{
    char *name;
    char *value;
};

int characterCheck(char *str, char c){
    for(int i = 0; i < strlen(str); i++){
        if(str[i] == c){
            return 1;
        }
    }
    return -1;
}


int main( int argc, char** argv ){

    // Turn to -1 if exiting
    int flag = 1;

    // Keep track of where to add next EnvVar
    int envIndex = 0;

    //History
    struct Commands commandHistory[maxStorage];

    //EnvVar Array 
    struct EnvVar envVarStorage[maxEnvVarStorage];
    for (int m = 0; m < maxEnvVarStorage; m++) {
        envVarStorage[m].name = '\0';
        envVarStorage[m].value = '\0';
    }

    // Initializing Commands Array
    for(int t = 0; t < maxStorage; t++){
        commandHistory[t].commandName = NULL;
        commandHistory[t].time = NULL;
        commandHistory[t].value = -1;
    }

    // Script Mode
    if(argc > 1){
        // Run a loop to read the file line by line and execute each line
        char * fileName = argv[1];

        FILE* file = fopen(fileName, "r");
        char line[256];
        int i = 0;
        while(fgets(line, sizeof(line), file)) {
            size_t ln = strlen(line) - 1;

            // Check if command is blank
            if(ln < 1){
                continue;
            }
            // Changing \n to null terminating character to split into tokens
            if(*line && line[ln] == '\n'){
                line[ln] = '\0';
            }

            // Tokens
            char delim[] = " ";
            char *ptr = strtok(line, delim); // ptr points to extracted command from line
            
            // Command
            char command[maxCmdLen];
            strcpy(command, ptr); // Store token into command

            // Arguments 
            char arguments[maxArgLen][200];
            int k = 0;

            while(ptr != NULL) { // wat does this do lolol
                ptr = strtok(NULL, delim);
                if(ptr != NULL){
                    strcpy(arguments[k], ptr);
                    k++;
                }
            }
            
            // Check user command
            if(strcmp(command, "exit") == 0){
                printf("Bye! \n");
                flag = -1;
                return 0;
            } else if(strcmp(command, "print") == 0){
                for(int l = 0; l < k; l++){
                    if(arguments[l][0] == '$'){
                        for(int u = 0; u < maxEnvVarStorage; u++){
                            if(envVarStorage[u].name == NULL){
                                continue;
                            }
                            if(strcmp(envVarStorage[u].name, arguments[l]+1) == 0){
                                printf("%s ", envVarStorage[u].value);
                                break;
                            }
                        }
                    } else {
                        printf("%s ", arguments[l]);
                    }
                }
                printf("\n");
            } else if(strcmp(command, "log") == 0){
                // Log out the history
                for(int j = 0; j < maxStorage; j++){ 
                    if(commandHistory[j].commandName != NULL){
                        printf("%s \n", commandHistory[j].time);
                        printf("%s ", commandHistory[j].commandName);
                        printf("%d \n", commandHistory[j].value);
                    }
                }
            } else if(strcmp(command, "theme") == 0){
                if(k > 1){
                    printf("color must be red, green, or blue\n");
                    continue;
                }
                char *color = arguments[0];
                if(strcmp(color, "red") == 0){
                    printf("\033[0;31m");
                } else if(strcmp(color, "blue") == 0){
                    printf("\033[0;34m");
                } else if(strcmp(color, "green") == 0){
                    printf("\033[0;32m");
                } else {
                    printf("color must be red, green, or blue\n");
                    continue;
                }
            } else if(*command == '$'){
                char commandCopy[maxCmdLen];
                strcpy(commandCopy, command);
                char delimName[] = "=";
                char delimValue[] = "\0";
                char *namePtr = strtok(commandCopy, delimName);
                namePtr += 1;
                char *valuePtr = strtok(NULL, delimValue);

                if(namePtr == NULL || valuePtr == NULL || characterCheck(namePtr, ' ') == 1 || characterCheck(valuePtr, ' ') == 1) {
                    printf("Variable value expected \n");
                    continue;
                }


                // Check for existing $var name
                int isDup = 0;
                for(int x = 0; x < maxEnvVarStorage; x++) {
                    if(envVarStorage[x].name == NULL){
                        continue;
                    }
                    if(strcmp(envVarStorage[x].name, namePtr) == 0){
                        strcpy(envVarStorage[x].value, valuePtr);
                        isDup = 1;
                        break;
                    }
                }

                if (isDup == 0){ // No duplicate
                    envVarStorage[envIndex].name = malloc(100);
                    envVarStorage[envIndex].value = malloc(100);
                    strcpy(envVarStorage[envIndex].name, namePtr);
                    strcpy(envVarStorage[envIndex].value, valuePtr);
                    envIndex++;
                }                

            // Non built in command
            } else {
                // Create pipe

                int fds[2];
                pid_t pid;
                char buff[30];
                if(pipe(fds) == -1){
                    perror("Error: ");
                    printf("PIPE ERROR!");
                    continue;
                }
                memset(buff, 0, 30);

                // Create a new process with fork()
                int fc = fork();
                if(fc < 0){
                    printf("fork failed");
                    exit(1);
                } else if(fc==0){
                    // new child process
                    execlp(command, "", NULL);
                    printf("Not a recognized command \n");
                    // Write to pipe if error
                    close(fds[0]);
                    write(fds[1], "E", 1);
                    abort();
                } else {
                    int waitc = wait(NULL);
                    // Read from pipe check for error

                    close(fds[1]);
                    read(fds[0], buff, 1);
                    close(fds[0]);
                    if(buff[0] == 'E'){
                        continue;
                    }
                }
            }
            // Time stuff for log function
            time_t rawtime;
            struct tm *info;
            time( &rawtime );
            info = localtime( &rawtime );
            char* tempTime = asctime(info);
            size_t timeln = strlen(tempTime) - 1;
            if(*tempTime && tempTime[timeln] == '\n'){
                tempTime[timeln] = '\0';
            }
            // Track Command
            commandHistory[i].commandName = malloc(maxCmdLen);
            strcpy(commandHistory[i].commandName, command);
            commandHistory[i].time = malloc(150);
            strcpy(commandHistory[i].time, tempTime);
            commandHistory[i].value = 0;
            
            i++;
        }

        fclose(file);

        return 0;

    // Interactive Mode
    } else {
        // i = index for loop
        int i = 0;
        while(i < maxStorage){
            char userInput[1024];

            // Grab user input
            printf("Provide a shell command and arguments to execute. \n");
            fgets(userInput, 1024, stdin);
            size_t ln = strlen(userInput) - 1;

            // Check if command is blank
            if(ln < 1){
                continue;
            }
            // Changing \n to null terminating character to split into tokens
            if(*userInput && userInput[ln] == '\n'){
                userInput[ln] = '\0';
            }

            // Tokens
            char delim[] = " ";
            char *ptr = strtok(userInput, delim); // ptr points to extracted command from userInput
            
            // Command
            char command[maxCmdLen];
            strcpy(command, ptr); // Store token into command

            // Arguments 
            char arguments[maxArgLen][200];
            int k = 0;

            while(ptr != NULL) { // wat does this do lolol
                ptr = strtok(NULL, delim);
                if(ptr != NULL){
                    strcpy(arguments[k], ptr);
                    k++;
                }
            }
            
            // Check user command
            if(strcmp(command, "exit") == 0){
                printf("Bye! \n");
                flag = -1;
                return 0;
            } else if(strcmp(command, "print") == 0){
                for(int l = 0; l < k; l++){
                    if(arguments[l][0] == '$'){
                        for(int u = 0; u < maxEnvVarStorage; u++){
                            if(envVarStorage[u].name == NULL){
                                continue;
                            }
                            if(strcmp(envVarStorage[u].name, arguments[l]+1) == 0){
                                printf("%s ", envVarStorage[u].value);
                                break;
                            }
                        }
                    } else {
                        printf("%s ", arguments[l]);
                    }
                }
                printf("\n");
            } else if(strcmp(command, "log") == 0){
                // Log out the history
                for(int j = 0; j < maxStorage; j++){ 
                    if(commandHistory[j].commandName != NULL){
                        printf("%s \n", commandHistory[j].time);
                        printf("%s ", commandHistory[j].commandName);
                        printf("%d \n", commandHistory[j].value);
                    }
                }
            } else if(strcmp(command, "theme") == 0){
                if(k > 1){
                    printf("color must be red, green, or blue\n");
                    continue;
                }
                char *color = arguments[0];
                if(strcmp(color, "red") == 0){
                    printf("\033[0;31m");
                } else if(strcmp(color, "blue") == 0){
                    printf("\033[0;34m");
                } else if(strcmp(color, "green") == 0){
                    printf("\033[0;32m");
                } else {
                    printf("color must be red, green, or blue\n");
                    continue;
                }
            } else if(*command == '$'){
                char commandCopy[maxCmdLen];
                strcpy(commandCopy, command);
                char delimName[] = "=";
                char delimValue[] = "\0";
                char *namePtr = strtok(commandCopy, delimName);
                namePtr += 1;
                char *valuePtr = strtok(NULL, delimValue);

                if(namePtr == NULL || valuePtr == NULL || characterCheck(namePtr, ' ') == 1 || characterCheck(valuePtr, ' ') == 1) {
                    printf("Variable value expected \n");
                    continue;
                }


                // Check for existing $var name
                int isDup = 0;
                for(int x = 0; x < maxEnvVarStorage; x++) {
                    if(envVarStorage[x].name == NULL){
                        continue;
                    }
                    if(strcmp(envVarStorage[x].name, namePtr) == 0){
                        strcpy(envVarStorage[x].value, valuePtr);
                        isDup = 1;
                        break;
                    }
                }

                if (isDup == 0){ // No duplicate
                    envVarStorage[envIndex].name = malloc(100);
                    envVarStorage[envIndex].value = malloc(100);
                    strcpy(envVarStorage[envIndex].name, namePtr);
                    strcpy(envVarStorage[envIndex].value, valuePtr);
                    envIndex++;
                }                

            // Non built in command
            } else {
                // Create pipe
                int fds[2];
                pid_t pid;
                char buff[30];
                if(pipe(fds) == -1){
                    perror("Error: ");
                    printf("PIPE ERROR!");
                    continue;
                }
                memset(buff, 0, 30);

                // Create a new process with fork()
                int fc = fork();
                if(fc < 0){
                    printf("fork failed");
                    exit(1);
                } else if(fc==0){
                    // new child process
                    execlp(command, "", NULL);
                    printf("Not a recognized command \n");
                    // Write to pipe if error
                    close(fds[0]);
                    write(fds[1], "E", 1);
                    abort();
                } else {
                    int waitc = wait(NULL);
                    // Read from pipe check for error
                    close(fds[1]);
                    read(fds[0], buff, 1);
                    if(buff[0] == 'E'){
                        continue;
                    }
                }
            }
            // Time stuff for log function
            time_t rawtime;
            struct tm *info;
            time( &rawtime );
            info = localtime( &rawtime );
            char* tempTime = asctime(info);
            size_t timeln = strlen(tempTime) - 1;
            if(*tempTime && tempTime[timeln] == '\n'){
                tempTime[timeln] = '\0';
            }
            // Track Command
            commandHistory[i].commandName = malloc(maxCmdLen);
            strcpy(commandHistory[i].commandName, command);
            commandHistory[i].time = malloc(150);
            strcpy(commandHistory[i].time, tempTime);
            commandHistory[i].value = 0;
            
            i++;
        }
    }
    return 0;
}
