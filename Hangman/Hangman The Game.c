#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<conio.h>

#define types 3                     // increase it to add more files
#define maxFileName 20              // increase it if filename gets bigger


void readWord( FILE *file);             // get a random word from FILE
void fillGuess(char s[], int i);        // fill 'guess' with underscores and a space for clear view

char a[20];

int main()
{
    srand(time(NULL));                                  // seed  random number

    const char category [types][maxFileName] = {"country.txt", "sport.txt", "city.txt"}; // add filename of different category here

    char word[20], guess[20], entered[20];
    int len, choice,i, life=5, c, guessCount=0, enteredIndex=0;                   // c => store input from fgetc();
    char *position;                                                               // search for CHAR in STRING and store result here

    FILE *file;

    printf("Categories:\n\n");
    printf("1 - Country\t2 - Sport\n3 - City\n\n");           // splash scrren showing all categories

    scanf("%d", &choice);                           // get choice of users

    file = fopen(category[choice-1], "r");          // open the file

    readWord( file );                               // read a random word from file

    if( a[strlen(a)-1] =='\n' ) a[strlen(a)-1] = '\0';

    strcpy(word, a);                                // store the random word in word[]

    len = strlen(word);

    fillGuess(guess, len);

    printf("%s\n\n", guess);

    while( life > 0 && guessCount != len ) {

        printf("Enter your guess\n\n");

        c = getchar();

        while(c == '\n') c = getchar();             // getchar gives '\n' need to take it out

        position = strchr(word, c);

        if(position == NULL) {                  // character not found

            life--;

            if(life>0) {
                printf("Wrong Guess. %d tries left\n", life);
            }
        }

        else if(strchr(entered, c) == NULL){                                  // found. lets see if there's any more

            entered[enteredIndex++] = c;            // store right guess in an array

            entered[enteredIndex++] = ' ';

            entered[enteredIndex] = '\0';         // add an null incase the previous character is the last one

            while(position != NULL) {
                guess[(position-word)*2] = c;       // keep it in a new array. || position - word == same position of the char in the word ||

                guessCount++;

                position = strchr(position+1, c);
            }

            printf("%s\n\n", guess);
        }
        else {
            printf("Uh-Oh. Youve already entered : %s\n\n%s\n", entered, guess);
        }

    }

    if( life == 0) {
        printf("You are HANGED!!");
        printf("\n\nIt is :  \%s", word);
    }
    else{
        printf("Well Done!");
    }

    getchar();

}

/*  Functions  */

void readWord ( FILE *fp ){         // read a random word from file

    fseek(fp, 0 , SEEK_END);

    long pos = ftell(fp);

    long ofset = rand()%pos;

    fseek(fp, ofset, SEEK_SET);

    if( (fgets(a, 20, fp)) == NULL ) {          // reached EOF last line
        fseek(fp, 0, SEEK_SET);                 //go to beginning
        fgets(a, 20, fp);                       // read first line
    }

    else{
        if( (fgets(a, 20, fp)) != NULL ){
            // got my line
        }
        else {
            fseek(fp, 0, SEEK_SET);
            fgets(a, 20, fp);
        }
    }
}

/* fill the guess array with underscore and space */

void fillGuess(char str[], int len) {
    int i;

    for(i=0; i<2*len ; i++) {
        str[i++] = '_';
        str[i] = ' ';
    }
    str[i] = '\0';
}
