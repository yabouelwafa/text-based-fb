#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "a2_nodes.h"
#include "a2_functions.h"
#include <assert.h>

int main()
{
    FILE *csv_file = fopen("user_details.csv", "r");
    if (csv_file == NULL)
    {
        perror("Error opening the CSV file");
        return 1;
    }
    // Parse CSV data and create users
    user_t *users = read_CSV_and_create_users(csv_file, 49);

    fclose(csv_file);
 
    _Bool running = true;
    char username[30];
    char password[15];
    int p_choice;
    int f_choice;
    user_t * finding = malloc(sizeof(user_t));

    
    // Main running loop
    while (running)
    {
        //Getting user choice 
        int m_choice = menu_choice(6);

        if (m_choice == 1)
        {

            printf("Enter a username: ");
            scanf("%s", &username);


            printf("Enter an up to 15 characters password: ");
            scanf("%s", &password);

            users = add_user(users, username, password);
            printf("******** USER ADDED! ************\n\n\n");
        }
        else if (m_choice == 2)
        {
            //Allocating memory for the user thats going to be searched for
            finding = malloc(sizeof(user_t));
            assert(finding != NULL);

            printf("Enter a username to update their password: ");
            scanf("%s", &username);

            finding = find_user(users, username);

            //if user not found
            if (finding == NULL)
            {
                printf("*************************\n");
                printf("      USER NOT FOUND     \n");
                printf("*************************\n\n\n");
            }
            //If user is found
            else
            {
                printf("Enter the password: ");
                scanf("%s", &password);
                if (strcmp(password, finding->password) == 0)
                {
                    printf("Enter a new password. up to 15 characters: ");
                    scanf("%s", &password);
                    strcpy(finding->password, password);
                    printf("\n\n\n*********** PASSWORD CHANGED! ************\n\n\n");
                }
                else
                {
                    printf("*************************\n");
                    printf("    INCORRECT PASWORD    \n");
                    printf("*************************\n\n\n");
                }
            }
        }
        else if (m_choice == 3)
        {
            printf("Enter a username to manage their posts: ");
            scanf("%s" ,username);
            finding = find_user(users, username);
            
            //If user is not found
            if (finding == NULL)
            {
                printf("*************************\n");
                printf("      USER NOT FOUND     \n");
                printf("*************************\n\n\n");
            }
            //If user is found
            else 
            {
                printf("*************************\n");
                printf("    MANAGING %s'S POSTS  \n", username);
                printf("*************************\n");

                _Bool p_running = true;
                while (p_running)
                {

                    //Getting users choice
                    p_choice = post_menu_choice(finding, 3);
                    char post[250];
                    if (p_choice == 1)
                    {
                        printf("Enter your post content: ");
                        scanf(" %[^\n]s", post);
                        add_post(finding, post);
                        display_all_user_posts(finding);
                    }
                    else if (p_choice == 2)
                    {
                        if(delete_post(finding)){
                            display_all_user_posts(finding);
                        }
                        else
                        {
                            printf("\n********USER HAS NO POSTS*********\n");
                        }
                    }
                    else if (p_choice == 3)
                    {
                        p_running = false;
                    }
                }
            }
        }
        else if (m_choice == 4)
        {
            finding = malloc(sizeof(user_t));
            assert(finding != NULL);

            printf("Enter a username to update their friend: ");
            scanf("%s" ,username);
            finding = find_user(users, username);

            if (finding == NULL)
            {
                printf("*************************\n");
                printf("      USER NOT FOUND     \n");
                printf("*************************\n\n\n");
            }
            else 
            {
                printf("*************************\n");
                printf("  MANAGING %s'S FRIENDS  \n", username);
                printf("*************************\n");

                _Bool f_running = true;
                while (f_running)
                {
                    f_choice = friends_menu_choice(3);
                    if (f_choice == 1)
                    {
                        printf("Enter a new friends' name: ");
                        scanf("%s", &username);
                        add_friend(finding, username);
                        printf("List of %s's friends: \n", finding->username);
                        display_user_friends(finding);                        
                    }
                    else if (f_choice == 2)
                    {
                        if (finding->friends == NULL)
                        {
                            printf("\n****Friends list is empty****\n\n");
                        }
                        else
                        {
                            printf("Enter a friend's name to delete: ");
                            scanf("%s", &username);
                            _Bool found = delete_friend(finding, username);
                            printf("\n");
                            if (found)
                            {
                                printf("Updated list of %s's friends: \n\n", finding->username);
                                display_user_friends(finding);
                            }
                            else
                            {
                                printf("Invalid friend's name\n");
                            }
                        }
                    }
                    else
                    {
                        f_running = false;
                    }
                }
            }
        }
        else if(m_choice == 5)
        {
            finding = malloc(sizeof(user_t));
            printf("Enter a username to display their posts: ");
            scanf("%s" ,username);
            finding = find_user(users, username);

            if (finding == NULL)
            {
                printf("*************************\n");
                printf("      USER NOT FOUND     \n");
                printf("*************************\n\n\n");
            }
            else
            {
                display_posts_by_n(finding, 3);
            }
        }
        else if (m_choice == 6)
        {
            printf("Good Bye!\n");
            teardown(users);
            running = false;
        }
    }
}