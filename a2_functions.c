#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include "a2_nodes.h"
#include "a2_functions.h"

unsigned short int get_user_input(int max){

    int input;
    printf("Enter your choice: ");
	scanf("%d", &input);

    while (input <= 0 || input > max)
    {
        printf("Invalid choice, Please try again: ");
        scanf("%d", &input);
    }

    return input;
}

int menu_choice(int max){

    //Print Menu choices
    printf("*************************\n");
    printf("       MAIN MENU:        \n");
    printf("*************************\n");
    printf("1. Register a new user\n");
    printf("2. Manage a user's profile (change password)\n");
    printf("3. Manage a user's posts (add/remove)\n");
    printf("4. Manage a user's friends (add/remove)\n");
    printf("5. Display a user's posts\n");
    printf("6. Exit\n\n\n\n");

    return get_user_input(max);
}

int post_menu_choice(user_t * user, int max){

    if (user->posts == NULL)
    {
        printf("Note: No posts available for %s\n", user->username);
    }

    //Print post menu choices
    printf("1. Add a new post\n");
    printf("2. Remove a post\n");
    printf("3. Return to main menu\n\n");

    return get_user_input(max);

}

int friends_menu_choice(int max){

    //Print friends menu choices
    printf("1. Add a new friend\n");
    printf("2. Remove a friend\n");
    printf("3. Return to main menu\n");

    return get_user_input(max);


}


user_t *add_user(user_t *users, const char *username, const char *password) {

    //Allocate memory for user
    user_t * user = malloc(sizeof(user_t));
    assert(user != NULL);

    //intialize user values
    strcpy(user->username, username);
    strcpy(user->password, password);
    user->next = NULL;
    user->friends = NULL;
    user->posts = NULL;


    //Case if there are no users
    if (users == NULL)
    {
        return user;
    }

    //Sorting users if there is only one user
    if (strcmp(users->username, user->username) > 0) {
        user->next = users;
        return user;
    }

    //Sorting if there is more than one user
    user_t * current = users;
    while (current->next != NULL && strcmp(current->next->username, user->username) < 0)
    {
        current = current->next;        
    }
    
    
    user->next = current->next;
    current->next = user;

    return users;
}

user_t *find_user(user_t *users, const char *username){

    if (users == NULL)
    {
        return NULL;
    }

    for (user_t * current = users; current != NULL; current = current->next)
    {
        if (strcmp(current->username, username) == 0)
        {
            return current;
        }
    }

    return NULL;
    
}

post_t *create_post(const char *text){

    //Allocating heap memory for the post
    post_t * post = malloc(sizeof(post_t));
    assert(post != NULL);

    //Initializing post content
    strcpy(post->content, text);
    post->next = NULL;

    return post;
}



void add_post(user_t *user, const char *text){

    assert(user != NULL);

    //Initializing "post" to the post created by create post
    post_t * post = create_post(text);
    post->next = user->posts;

    //Adding post to the user's posts
    user->posts = post;
}

_Bool delete_post(user_t *user){

    post_t * placeholder = malloc(sizeof(user_t));
    assert(placeholder != NULL);

    if (user->posts == NULL)
    {
        return false;
    }
    else
    {
        placeholder = user->posts->next;
        free(user->posts);
        user->posts = placeholder;
        return true;
    }
    
}

_Bool delete_friend(user_t *user, char *friend_name){

    friend_t * placeholder = malloc(sizeof(friend_t));
    assert(placeholder != NULL);

    //Checking if the first friend's username matches
    if (strcmp(user->friends->username, friend_name) == 0)
    {
        placeholder = user->friends->next;
        free(user->friends);
        user->friends = placeholder;
        return true;
    }
    else
    {
        //Checking for the rest of the names
        for (friend_t * current_f = user->friends; current_f->next != NULL; current_f = current_f->next)
        {
            if (strcmp(current_f->next->username, friend_name) == 0)
            {
                if (current_f->next->next != NULL)
                {
                    placeholder = current_f->next;
                    current_f->next = current_f->next->next;
                    free(placeholder);
                    return true;
                }
                else
                {
                    placeholder = current_f->next;
                    current_f->next = NULL;
                    free(placeholder);
                    return true;
                }
            }
        }
    }
    return false;
}


friend_t *create_friend(const char *username){
    
    //Allocating memory for the friend
    friend_t * friend = malloc(sizeof(friend_t));
    assert(friend != NULL);

    //Intializing friend values
    strcpy(friend->username, username);
    friend->next = NULL;

    return friend;
}

void add_friend(user_t *user, const char *friend){



    friend_t * friend_a = create_friend(friend);

    //Checking if user has no friends
    if (user->friends == NULL)
    {
        user->friends = friend_a;
    } 
    //Checking if the first friend's username matches
    else if (strcmp(user->friends->username, friend_a->username) > 0)
    {
        friend_a->next = user->friends;

        user->friends = friend_a;
        
    }
    else
    {

        friend_t * current = user->friends;
        while (current->next != NULL && strcmp(current->next->username, friend_a->username) < 0)
        {
            current = current->next;        
        }
        
        
        friend_a->next = current->next;
        current->next = friend_a;
    }
    
}


void display_all_user_posts(user_t *user){

    if(user->posts != NULL)
    {
        printf("------------------------------------------------\n");
        printf("               %s's posts               \n\n", user->username);

        int counter = 1;

        for (post_t * current = user->posts; current != NULL; current = current->next){
            printf("%i - %s\n\n",counter, current->content);
            counter += 1;
        }

        printf("------------------------------------------------\n\n");
    }

}

void display_user_friends(user_t *user){

    if(user->friends != NULL)
    {

        int counter = 1;

        for (friend_t * current = user->friends; current != NULL; current = current->next){
            printf("%i - %s\n\n",counter, current->username);
            counter += 1;
        }

    }
}

void display_posts_by_n(user_t *users, int number){
    
    int count = 1;
    char * choice = malloc(sizeof(char));
    assert(choice != NULL);
    _Bool posts = true;

    post_t * current = users->posts;

    while (posts)
    {
        while (current != NULL && count <= number)
        {
            printf("%i - %s\n\n", count, current->content);
            count++;
            current = current->next;
        }

        if (current != NULL)
        {
            printf("Would you like to display more posts(y-yes n-no): ");
            scanf("%s", choice);
            if (strcmp(choice, "Y") == 0 || strcmp(choice, "y") == 0)
            {
                number += 3;
            }
            else
            {
                posts = false;
            }
        }
        else
        {
            printf("All posts have been displayed\n");
            posts = false;
        }
    }


    
}

void teardown(user_t *users){

    assert(users != NULL);

    // user_t * placeholder;
    user_t * current = users;
    
    while (current != NULL)
    {
        users = users->next;

        //clearing user's friends
        friend_t * current_f = current->friends;
        while (current_f != NULL)
        {
            current->friends = current->friends->next;
            free(current_f);
            current_f = current->friends;
        }
        
        //clearing user's posts
        post_t * current_p = current->posts;
        while (current_p != NULL)
        {
            current->posts = current->posts->next;
            free(current_p);
            current_p = current->posts;
        }

        free(current);
        current = users;
    }
    users = NULL;
}


/*
   ******** DONT MODIFY THIS FUNCTION ********
   Function that reads users from the text file.
   IMPORTANT: This function shouldn't be modified and used as is
   ******** DONT MODIFY THIS FUNCTION ********
*/
user_t *read_CSV_and_create_users(FILE *file, int num_users)
{
    srand(time(NULL));
    user_t *users = NULL;
    char buffer[500];
    fgets(buffer, sizeof(buffer), file); // Read and discard the header line
    int count = 0;
    for (int i = 0; i < num_users; i++)
    {
        fgets(buffer, sizeof(buffer), file);
        buffer[strcspn(buffer, "\r\n")] = 0; // Remove newline characters

        char *token = strtok(buffer, ",");
        char *token2 = strtok(NULL, ",");
        users = add_user(users, token, token2);
        char *username = token;

        token = strtok(NULL, ",");

        user_t *current_user = users;
        for (; current_user != NULL && strcmp(current_user->username, username) != 0; current_user = current_user->next)
            ;

        while (token != NULL && strcmp(token, ",") != 0 && count < 3)
        {
            if (strcmp(token, " ") != 0)
            {
                add_friend(current_user, token);
            }
            token = strtok(NULL, ",");
            count++;
        }
        count = 0;

        // token = strtok(NULL, ",");
        while (token != NULL && strcmp(token, ",") != 0)
        {
            add_post(current_user, token);
            token = strtok(NULL, ",");
        }
    }
    return users;
}
