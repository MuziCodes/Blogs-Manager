#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include <time.h>
#include <conio.h>

struct blogformat{

    char author[1000];
    char title[1000];
    char body[1000];
    char time[1000];
    char viewcount[1000];

};

int display(char *usernamefile);
char *Sign();
void Home();
void modify(char *usernamefile, int index, char mode);
void find(char *text, char *search);
void crypto(char mode);
void insert(char *username, char *title, char *body, char *time);
void printbody(char *body);
void compose(char *usernamefile);
void view_others(char *usernamefile);
void view_own(char *usernamefile);

int main(){

	system("COLOR 09");

    bool restart = false;
    printf("Use the corresponding numbers to navigate the menu.\n");

    //crypto('D');							//decrypts all files

    int option, option2, mode;
    char *usernamefile;

    do{

    	do{
            printf("\nWould you like to:\n0.Exit\n1.Sign up\n2.Sign in\n\nSelect: ");
            scanf("%d",&option);
            fflush(stdin);

            if(option != 1 && option != 2 && option != 0){
            	printf("Invalid Input\n");
			}

        }while(option != 1 && option != 2 && option != 0);


    	if(option != 0){
			usernamefile = Sign(option);

   			system("cls");

    		do{

	    		option2 = display(usernamefile);
	        	if(option2 != 0){
		            printf("Press any key to go back to the Home Page.....");
		            getch();
		            system("cls");
		        }

			}while(option2 != 0);

			free(usernamefile);
	        system("cls");

		}


    }while(option != 0);

	//crypto('E');						//encrypts all files

}

int display(char *usernamefile){

    int option;

	Home();

	printf("\n0. Logout\n1. Compose New Blog\n2. View other people's posts\n3. View your own posts\nSelect: ");
    scanf("%d", &option);
    fflush(stdin);
    if(option != 0 && option != 1 && option != 2 && option !=3){
        printf("Invalid choice. Please try again.\n");
    }

    switch(option){

        case 1:
            compose(usernamefile);
            break;

        case 2:
            view_others(usernamefile);
            break;

        case 3:
            view_own(usernamefile);
            break;
    }

    return option;

}

void compose(char *usernamefile){

    char *newpost, *newtitle, temp[500], temp2[1000];        //temp variable is to temporarily store the value for the input
    char ch1, ch2;
    int count, verify;

    do{
        printf("Enter the title: ");
        gets(temp);
        fflush(stdin);
        if(strlen(temp) == 0){
            printf("Your title must have atleast one character.\n");
        }
    }while(strlen(temp) == 0);

	newtitle = (char*)calloc(strlen(temp)+1, sizeof(char));
	strcpy(newtitle, temp);

    do{
        printf("Enter the body (press Enter twice to complete the input): ");
        count = 0;
        verify = 1;
        while(1){
            ch1 = getchar();
            if(ch1 != '\n'){
                temp2[count++] = ch1;
            }
            else{
                if(count == 0){                //Validation for body paragraph to be atleast one character
                    verify = 0;
                    break;
                }
                temp2[count++] = '~';
                ch2 = getchar();
                if(ch2 == '\n'){
                    break;
                }
                else{
                    temp2[count++] = ch2;
                }
            }
        }
        temp2[count] = '\0';
        temp2[count-1] = '\0';
        fflush(stdin);

        if(verify == 0){
            printf("Your body paragraph must have atleast one character.\n");
        }
    }while(verify == 0);

    newpost = (char*)calloc(strlen(temp2)+1, sizeof(char));
	strcpy(newpost, temp2);

    time_t currenttime = time(NULL);

    insert(usernamefile, newtitle, newpost, ctime(&currenttime));        //ctime takes address as parameter

}

void view_others(char *usernamefile){

    char username[100], content[1000], usernamenewline[100], temp[1000];
    FILE *fpointer;
    int count = 1, count2, count3, index = 0, usernum, currentview;

    strcpy(username, usernamefile);

    for(count = 1; count <= 4; count++){
        username[strlen(username)-1] = '\0';        //gives us the name of the user by removing .txt
    }

    fpointer = fopen("users.txt", "r");
    strcpy(usernamenewline, username);
    strcat(usernamenewline, "\n");

	count = 1;

    while(fgets(content, 1000, fpointer) != NULL){              //to check how many lines are in the file
        count++;
    }

    if(count==2 && !strcmpi(usernamenewline, content)){          //strcmpi ignores difference between capitals and non-capitals
        printf("No other users are currently registered.\n");
    }
    else{
        count = 1;
        rewind(fpointer);

        while(fgets(content, 1000, fpointer) != NULL){                 //feof is only true AFTER the EOF has been reached, not WHEN it has been reached.
            if(content[0] != '\n' && strcmpi(usernamenewline, content)){        //strcmpi ignores difference between capitals and non-capitals
                printf("%d. ", count);
                printf("%s", content);
                count++;
            }
        }

        rewind(fpointer);        //now the pointer points to the beginning of the txt file

        printf("\n");
        do{
            printf("Select: ");
            scanf("%d", &usernum);
            fflush(stdin);
            if(usernum >= count || usernum <=0){
                printf("Invalid choice. Please try again.\n");
            }
        }while(usernum >= count || usernum <= 0);

        count = 1;
        while(count <= usernum){
            fgets(content, 1000, fpointer);
            if(content[0] != '\n' && strcmpi(usernamenewline, content)){
                count++;
            }
        }

        if(content[strlen(content)-1] == '\n'){
            content[strlen(content)-1] = '\0';
        }

        strcpy(temp, content);
        strcat(content, ".txt");
        fclose(fpointer);

        fpointer= fopen(content, "r");

        count = 1;
        while(fgets(content, 1000, fpointer) != NULL){
            count++;
        }

        system("cls");

        if(count == 4){
            printf("This user has not made any posts.\n");
            fclose(fpointer);
        }
        else{
            rewind(fpointer);
            printf("---------------------------\n");
            printf("\n%s's posts: \n", temp);

            count = 1;
            count2 = 1;
            count3 = 1;

            while(fgets(content, 1000, fpointer) != NULL){
                if(count > 3){                                  // if we put both fgets and count condition in if, it is important to put count>3 AFTER the && otherwise fgets wont run if count>3 is false
                    if(content[0] == '\n'){
                        printf("%s", content);
                    }
                    else if(count2 == 1){
                        printf("%d. ", count3);
                        printf("Title: %s", content);
                        count2++;
                    }
                    else if(count2 == 2){
                        count2++;
                    }
                    else if(count2 == 3){
                        printf("Posted on: %s", content);
                        count2++;
                    }
                    else if(count2 == 4){
                        printf("%s", content);
                        count2 = 1;
                        count3++;
                    }
                }
                count++;
            }
            printf("-------------------------------------------------------------------------\n");

            rewind(fpointer);

            do{
                printf("\nSelect: ");
                scanf("%d", &usernum);
                fflush(stdin);
                if(usernum >= count3 || usernum <=0){
                    printf("Invalid choice. Please try again.\n");
                }
            }while(usernum >= count3 || usernum <= 0);

            strcat(temp, ".txt");

            count = 1;
            count2 = 1;
            count3 = 1;
            while(fgets(content, 1000, fpointer) != NULL){
                if(count2>3 && count3 == usernum){
                    if(count == 1){
                        printf("\n\n---------------------------\n");
                        printf("Title: %s", content);
                        count++;
                    }
                    else if(count == 2){
                        printbody(content);
                        count++;
                    }
                    else if(count == 3){
                        printf("Posted at: %s", content);
                                                                    //fscanf will move the file pointer ahead so we can't make another if statement
                        fscanf(fpointer, "%*s %d", &currentview);	//%*s means to take the input but do not store it anywhere.

                        fclose(fpointer);
                        modify(temp, usernum, 'u');					//updates the view count
                        printf("Views: %d", currentview+1);
                        printf("\n-------------------------------------------------------------------------\n");
                        break;
                    }
                }
                if(count2>1 && content[0] == '\n'){
                    count3++;										//so that it doesn't count the first newline in the very start.
                }
                count2++;
            }
        }
    }

}

void view_own(char *usernamefile){

        int count = 1, count2 = 1, count3 = 1, index = 0, usernum, mode;
        char content[1000], search[100];
        FILE *fpointer;

		fpointer = fopen(usernamefile, "r");

        while(fgets(content, 1000, fpointer) != NULL){
            count++;
        }

        if(count == 4){
            printf("You have currently not made any posts.\n\n");
            fclose(fpointer);
		}
		else{

			count = 1;
			printf("\n----------------------------------------------------------------------------------------------------\n\n");

	        rewind(fpointer);
	        while(fgets(content, 1000, fpointer) != NULL){
	            if(count > 3){                                  // if we put both fgets and count condition in if, it is important to put count>3 AFTER the && otherwise fgets wont run if count>3 is false
	                if(content[0] == '\n'){
	                    printf("%s", content);
	                }
	                else if(count2 == 1){
	                	printf("%d. ", count3);
	                    printf("Title: %s", content);
	                    count2++;
	                }
	                else if(count2 == 2){
	                    printbody(content);
	                    count2++;
	                }
	                else if(count2 == 3){
	                    printf("Posted on: %s", content);
	                    count2++;
	                }
	                else if(count2 == 4){
	                    printf("%s", content);
	                    count2 = 1;
	                    count3++;
	                }

            	}
            	count++;
        	}

			printf("----------------------------------------------------------------------------------------------------\n\n");


			do{
                printf("Select the post to modify: ");
                scanf("%d", &usernum);
                fflush(stdin);
                if(usernum >= count3 || usernum <=0){
                    printf("Invalid choice. Please try again.\n");
                }
            }while(usernum >= count3 || usernum <= 0);

			do{
				printf("\n0. Go to Home page\n1. Edit\n2. Delete\n3. Find a word");
                printf("\nSelect: ");
                scanf("%d", &mode);
                fflush(stdin);
                if(mode >= 4 || mode < 0){
                    printf("Invalid choice. Please try again.\n");
                }
            }while(mode >= 4 || mode < 0);

            if(mode == 0){
                fclose(fpointer);                     //to make sure all file pointers are closed
            }
			else if(mode == 1){
				fclose(fpointer);
				modify(usernamefile, usernum, 'm');
			}
			else if(mode == 2){
				fclose(fpointer);
				modify(usernamefile, usernum, 'd');
			}
			else if(mode == 3){
				printf("Enter the word you want to search: ");
				gets(search);

				rewind(fpointer);
				count = 1;

				while(count <= 3){
					fgets(content, 1000, fpointer);
					count++;
				}

				count=1;

				while(fgets(content, 1000, fpointer) != NULL){
					if(count == usernum){
						fgets(content, 1000, fpointer);
						break;
					}

					if(content[0] == '\n'){
						count++;
					}

				}
				fclose(fpointer);

				find(content, search);
				printf("\n");

			}

		}

}

char *Sign(int mode){

    char *usernameptr;
    char username[100], password[200], content[1000];
    bool testuc, testlc, testdc, testlength, testspace, verify, verify2;
    int count, length;
    FILE *fpointer;

    if(mode == 1){
        do{
            verify = true;
            do{
                verify = true;
                printf("Enter your username: ");
                gets(username);

                if((strlen(username)<=2) || (strchr(username, ' ')) || (strchr(username, '/')) || (strchr(username, '\\')) || (strchr(username, '*')) || (strchr(username, ':')) || (strchr(username, '\"') || (strchr(username, '<'))) || (strchr(username, '>')) || (strchr(username, '|'))){
                    printf("Invalid username. Username should have atleast 3 characters.\n");
                    printf("Note: / \\ * : \" < > | cannot be used for a username.\n\n");

                    verify = false;
                }
            }while(!verify);

            fflush(stdin);

            strcat(username, ".txt");

            fpointer = fopen(username, "r");

            if(fpointer != NULL){
                verify = false;
                printf("This username already exists.\n\n");
            }

            fclose(fpointer);


        }while(!verify);

		usernameptr = (char *)calloc( (strlen(username)+1 ), sizeof(char) );
        strcpy(usernameptr, username);

        do{
            testuc = false, testlc = false, testdc = false, testlength = false, testspace = false;
            count = 0;
            printf("Enter your password (should have at least 6 characters, 1 uppercase, 1 lower case letter and 1 digit): ");
            gets(password);
            fflush(stdin);

            length = strlen(password);

            while(count < length){
                if(length>=6){
                    testlength = true;
                }
                if(islower(password[count])){
                    testlc = true;
                }
                if(isupper(password[count])){
                    testuc = true;
                }
                if(isdigit(password[count])){
                    testdc = true;
                }
                if(isspace(password[count])){
                    testspace = true;
                }
                count++;
            }

            if(testuc == true && testlc == true && testdc == true && testlength == true && testspace != true){
                printf("\nPassword Accepted.\n");
            }
            else{
                printf("Password did not match the requirements. Input another password.\n");
            }

        }while(!(testuc && testlc && testdc && testlength && !testspace));
        printf("Account successfully created.\n\n");

        fpointer = fopen(usernameptr, "w");
        fprintf(fpointer, "\n%s\n%s\n", username, password);
        fclose(fpointer);

        fpointer = fopen("users.txt", "a");
        fprintf(fpointer, "%s\n", username);
        fclose(fpointer);
        return usernameptr;

    }
    else if(mode == 2){
        count = 1;

        do{
            verify = true;
            printf("\nEnter your username: ");
            gets(username);
            fflush(stdin);

			strcat(username, ".txt");
            fpointer = fopen(username, "r");
            if(fpointer == NULL){
                printf("This username does not exist.\n");
                verify = false;
            }

        }while(!verify);

        usernameptr = (char *)calloc( (strlen(username) + 1), sizeof(char) );
        strcpy(usernameptr, username);

        do{
            printf("Enter your password: ");
            gets(password);
            fflush(stdin);

            while(!feof(fpointer) && count<=3){					//count statment is added so that it won't move the file cursor again when the loop triggers
                fgets(content, 1000, fpointer);
                if(count == 3 && (content[strlen(content)-1] == '\n')){
                    content[strlen(content)-1] = '\0';			//removes newline char
                    count++;
                    break;
                }
                count++;
            }
            printf("\n");

            if(!strcmp(password, content)){
                printf("Successfully logged into your account.\n");
                fclose(fpointer);
                return usernameptr;
            }
            else{
                printf("Password is incorrect. Please try again.\n\n");
            }
        }while(strcmp(password, content));						//runs a loop untill password and content are same

    }
    system("cls");

}

void insert(char *username, char *title, char *body, char *time){

	FILE *fp, *fp2;
	fp = fopen(username, "r");
	fp2 = fopen("temp.txt", "w");

	int i=0, j=0, index=0;
	char content[1000];
	char copy[100][1000];

	while(fgets(content, 1000, fp) != NULL){
		strcpy(copy[i], content);
		i++;
	}

	while(j<i){
		fprintf(fp2, "%s", copy[j]);
		j++;

		if(j==3){
			fprintf(fp2, "%s\n", title);
			fprintf(fp2, "%s\n", body);
			fprintf(fp2, "%s", time);
			fprintf(fp2, "Views: 0\n\n");
		}
	}
	fclose(fp);
	fclose(fp2);

    free(title);
    free(body);

	remove(username);											//fclose needs to be done before this function is called
	rename("temp.txt", username);
}

void crypto(char mode){
    int shift;
    char line[1000], c;
    FILE *fp, *fp2, *temp;
	fp = fopen("users.txt", "r");

	if (mode == 'E'){
	    shift = -2;
	}
	else if(mode == 'D'){
	    shift = 2;
	}

	if(fp != NULL){

		while( fgets(line, 1000, fp) ){

			line[strlen(line)-1] = '\0';
			strcat(line, ".txt");

			fp2 = fopen(line, "r");
			temp = fopen("temp.txt", "w");

	        char c;
	        while ( (c = fgetc(fp2)) != EOF ){
	            fputc(c + shift, temp);
	        }

		    fclose(fp2);
		    fclose(temp);
		    remove(line);
		    rename("temp.txt", line);

		}

	}

	fclose(fp);

}

void modify(char *usernamefile, int index, char mode){

	FILE *fp, *fp2;
	fp = fopen(usernamefile, "r");
	fp2 = fopen("temp.txt", "w");

	char copy[100][1000];
	char content[1000];

	int count = 1, i = 0, j = 0;

	while(fgets(content, 1000, fp) != NULL){
		strcpy(copy[i], content);
		i++;
	}

	while(j<3){
		fprintf(fp2, "%s", copy[j]);
		j++;
	}

	if(mode == 'd'){

		while(j<i){
			if(count==index){
				j+=5;
				count++;
			}
			else{
				fprintf(fp2, "%s", copy[j]);
				if(copy[j][0] == '\n'){
					count++;
				}
				j++;
			}
		}

	}
	else if(mode == 'm'){
		char title[100], body[1000], ch1, ch2;
		int verify, count2;
		do{
            printf("Enter the title: ");
            gets(title);
            fflush(stdin);
            if(strlen(title) == 0){
                printf("Your title must have atleast one character.\n");
            }
        }while(strlen(title) == 0);

        do{
            printf("Enter the body (press Enter twice to complete the input): ");
            count2 = 0;
            verify = 1;
            while(1){
                ch1 = getchar();
                if(ch1 != '\n'){
                    body[count2++] = ch1;
                }
                else{
                    if(count2 == 0){                //Validation for body paragraph to be atleast one character
                        verify = 0;
                        break;
                    }
                	body[count2++] = '~';
                    ch2 = getchar();
                    if(ch2 == '\n'){
                        break;
                    }
                    else{
                        body[count2++] = ch2;
                    }
                }
            }
            body[count2] = '\0';
            body[count2-1] = '\0';
            fflush(stdin);

            if(verify == 0){
                printf("Your body paragraph must have atleast one character.\n");
            }
        }while(verify == 0);

		while(j<i){
			if(count==index){
				fprintf(fp2, "%s\n", title);
				fprintf(fp2, "%s\n", body);
				j+=2;
				count++;							//so the if count==index statement is never true again
			}
			else{
				fprintf(fp2, "%s", copy[j]);
				if(copy[j][0] == '\n'){
					count++;
				}
				j++;
			}
		}
	}
	else if(mode == 'u'){
		int currentviews;
		char temp[1000];
		index = ((index-1)*5) + 3 + 4 -1;							//relies on specified pattern
		while(j<i){
			if(j==index-1){
				fprintf(fp2, "%s", copy[j]);
				j++;
				sscanf(copy[j], "Views: %d", &currentviews);			//extracts the views from the string
				fprintf(fp2, "Views: %d\n", ++currentviews);
			}
			else{
				fprintf(fp2, "%s", copy[j]);
			}
			j++;
		}

	}

	fclose(fp);
	fclose(fp2);

	remove(usernamefile);											//fclose needs to be done before this function is called
	rename("temp.txt", usernamefile);

}

void find(char *text, char *search){

	static int count=0;
    // Initialise console information
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;

    // Save current attributes
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes;

    char *pos = strstr(text, search); // get pointer to first char where search is found

	// Allocate memory for temp text
	char *temp = calloc(strlen(text) + 1, 1);

	if (pos==NULL){
		printbody(text); // print text where its not found
		printf("\n\n'%s' found %d times", search, count);
	}

    else
    {
        // Copy the text before the text to replace. memcpy(destination, source, size)
        memcpy(temp, text, pos - text);
        printbody(temp);					//for body paragraphs with multiple lines

        // Print found text with green background, then change color properties to normal
        SetConsoleTextAttribute(hConsole, BACKGROUND_GREEN);
        printf("%s", search);
        SetConsoleTextAttribute(hConsole, saved_attributes);

        // Copy the remaining text from after the replace text
        memcpy(temp, pos + strlen(search), strlen(text));
		count++;

		find(temp, search); // as strstr gives only first found, run again for remaining text
		text=temp; // otherwise temp will get reset
    }
    free(temp);
    count = 0;					//reset for next time function is called

}

void Home(){

	int i;
	char temp[1000] = {'\0'};
	struct blogformat highestblogs[3];				//top 3 most viewed posts

	for(i=0; i<3; i++){								//initialize so they are not undefined when we compare them
		strcpy(highestblogs[i].author, temp);
		strcpy(highestblogs[i].body, temp);
		strcpy(highestblogs[i].time, temp);
		strcpy(highestblogs[i].title, temp);
		strcpy(highestblogs[i].viewcount, temp);
	}

	char content[1000], line[1000];
	char title[1000], body[1000], time[1000], author[1000], stringviews[1000];
	int count, count2, views, maxview1 = -1, maxview2 = -1, maxview3 = -1, length;
	FILE *fp, *fp2;


	fp = fopen("users.txt", "r");

	printf("-----------------------------------------------------------------------------------------------------------------------");
	printf("\n\t\t\t\t\t\t\tHOME PAGE\n");
	printf("\n\t\t\t\t\t\t    Most Viewed Posts:");
	while(fgets(line, 1000, fp) != NULL){

		count = 0;
		count2 = 1;

		strcpy(author, line);
		line[strlen(line)-1] = '\0';
		strcat(line, ".txt");

		fp2 = fopen(line, "r");
		while(count<3){
			fgets(content, 1000, fp2);
			count++;
		}

		while(fgets(content, 1000, fp2) != NULL){

			if(content[0] == '\n'){
				;
			}
			else if(count2 == 1){
				strcpy(title, content);
				count2++;
			}
			else if(count2 == 2){
				strcpy(body, content);
				count2++;
			}
			else if(count2 == 3){
				strcpy(time, content);

				fscanf(fp2, "%s %d", temp, &views);
				if(views > maxview1){
					if(maxview1 != -1){							//so that this doesn't happen on the first run
						if(maxview2 != -1){						//so that this doesn't happen on the first run

                            strcpy(highestblogs[2].author, highestblogs[1].author);
                            strcpy(highestblogs[2].title, highestblogs[1].title);
                            strcpy(highestblogs[2].body, highestblogs[1].body);
                            strcpy(highestblogs[2].time, highestblogs[1].time);
                            strcpy(highestblogs[2].viewcount, highestblogs[1].viewcount);

							maxview3 = maxview2;
						}											//Put both copy blocks so that all view positions can be demoted

                        strcpy(highestblogs[1].author, highestblogs[0].author);
                        strcpy(highestblogs[1].title, highestblogs[0].title);
                        strcpy(highestblogs[1].body, highestblogs[0].body);
                        strcpy(highestblogs[1].time, highestblogs[0].time);
                        strcpy(highestblogs[1].viewcount, highestblogs[0].viewcount);

						maxview2 = maxview1;
					}

					maxview1 = views;
					strcpy(highestblogs[0].title, title);
					strcpy(highestblogs[0].body, body);
					strcpy(highestblogs[0].time, time);

					snprintf(stringviews, 100, "%d", views);			//stores views as a string in stringviews
					strcat(temp, " ");
					strcat(temp, stringviews);
					strcat(temp, "\n");

					strcpy(highestblogs[0].viewcount, temp);
					strcpy(highestblogs[0].author, author);

				}
				else if(views > maxview2){

					if(maxview2 != -1){						//so that this doesn't happen on the first run

						strcpy(highestblogs[2].author, highestblogs[1].author);
                        strcpy(highestblogs[2].title, highestblogs[1].title);
                        strcpy(highestblogs[2].body, highestblogs[1].body);
                        strcpy(highestblogs[2].time, highestblogs[1].time);
                        strcpy(highestblogs[2].viewcount, highestblogs[1].viewcount);

						maxview3 = maxview2;
					}

					maxview2 = views;
					strcpy(highestblogs[1].title, title);
					strcpy(highestblogs[1].body, body);
					strcpy(highestblogs[1].time, time);

					snprintf(stringviews, 100, "%d", views);		//stores views as a string in stringviews
					strcat(temp, " ");
					strcat(temp, stringviews);
					strcat(temp, "\n");

                    strcpy(highestblogs[1].viewcount, temp);
					strcpy(highestblogs[1].author, author);
				}
				else if(views > maxview3){

					maxview3 = views;
                    strcpy(highestblogs[2].title, title);
					strcpy(highestblogs[2].body, body);
					strcpy(highestblogs[2].time, time);

					snprintf(stringviews, 100, "%d", views);
					strcat(temp, " ");
					strcat(temp, stringviews);
					strcat(temp, "\n");

					strcpy(highestblogs[2].viewcount, temp);
					strcpy(highestblogs[2].author, author);
				}

				count2 = 1;
			}

		}

        fclose(fp2);
	}

	fclose(fp);

    count = 0;						//so that we can print "no posts" if count==0

    for(i = 0; i<3; i++){

        if(highestblogs[i].title[0] != '\0'){
            printf("\nAuthor: %s", highestblogs[i].author);
            printf("Title: %s", highestblogs[i].title);
            printbody(highestblogs[i].body);
            printf("Posted at: %s", highestblogs[i].time);
            printf("%s\n", highestblogs[i].viewcount);
            count++;
        }

    }


    if(count == 0){
        printf("\nNo posts have been uploaded yet\n\n");
    }

	printf("-----------------------------------------------------------------------------------------------------------------------\n\n");

}

void printbody(char *body){

    int i=0, length;
	length = strlen(body);

    while(i<length){
        if(body[i] == '~'){
            printf("\n");
        }
        else{
            printf("%c", body[i]);
        }
        i++;
    }

}
