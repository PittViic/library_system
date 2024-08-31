#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct registration{ 
    char isbn[14]; 
    char name[30];
    int year;
    char author[30];
    int quantity;
    registration *next;
    registration *prev;
} books;

typedef struct reference{ //Reference to the head and tail of list
    books *head;
    books *foot;
} ref;

void start(ref *element){ //Function to start
    element->head = NULL;
    element->foot = NULL;
}

void to_lower(char *str){ //Function to convert to lower case
    int i;
    for(i = 0; i < str[i]; i++){
        str[i] = tolower(str[i]);
    }
}

void to_Sort(ref *element, books *New){ //Function to sort the list of books
    if(element->head == NULL){ 
        element->head = New;
        element->foot = New;
        New->next = NULL;
        New->prev = NULL;
    } else {
        books *present = element->head;
        books *previous = NULL;

        while(present != NULL && strcmp(present->name, New->name) < 0){
            previous = present;
            present = present->next;
        }

        if(previous == NULL){ //Head of the list
            New->next = present;
            present->prev = New;
            New->prev = NULL;
            element->head = New;
        } else if(present == NULL){ //End of the list
            previous->next = New;
            New->prev = previous;
            element->foot = New;
            New->next = NULL;
        } else { //Middle of the list
            previous->next = New;
            New->prev = previous;
            New->next = present;
            present->prev = New;
        }
    }
}

books *repeated(ref *element, books *New){ //Verify the same elements in list and return the adress of element
    books *present = element->head;

    while(present != NULL && strcmp(present->isbn, New->isbn) != 0){
        present = present->next;
    }

    if(present != NULL && strcmp(present->isbn, New->isbn) == 0){ //if is the same ISBN, return the adress
        return present;
    } else { //if not, return NULL
        return NULL;
    }
}

void insert_isbn(char input[14]){ //Function to insert and verify a ISBN code
    int verification = 1; 

    do{
        printf("ISBN of the book: ");
        scanf("%13s", input);

        int length = strlen(input);

        if(length == 13){ //the length need to be equal to 13
            int i;

            for(i = 0; i < length; i++){
                if(!isdigit(input[i])){
                    verification = 0; //If the element its not a digit, change the verification to 0
                    break;
                }
            }
        }

        if(verification == 0){ //If equal to 0, print an error message
            printf("Invalid ISBN!\n");
        }
    } while(strlen(input) != 13);
}

void insert(ref *element){ //Function to insert an element and use the other functions to sort an verify the existence of same elements
    books *New = (books *)malloc(sizeof(books));
    int qtd;

    if(New == NULL){ //If is not possible to allocate memory
        printf("Couldn't allocate memory for a new book\n");
        return;
    } else { //If not, execute and insert the elements
        insert_isbn(New->isbn);
        getchar();
        books *aux = repeated(element, New);

        if(aux != NULL){ //If the repeate function finds a duplicate element, just increment quantity of elements
            printf("Quantity of books that you want to insert: ");
            scanf("%d", &qtd);

            aux->quantity += qtd;
            return;
        } else { //If not, insert a new element
            printf("Title: ");
            fgets(New->name, sizeof(New->name), stdin);
            New->name[strcspn(New->name, "\n")] = '\0'; //remove the new line caracter
            to_lower(New->name); 

            do{
                printf("Year of publication: ");
                scanf("%d", &New->year);
                getchar();
            } while(New->year < 0 || New->year > 2023);

            printf("Author: ");
            fgets(New->author, sizeof(New->author), stdin);
            New->author[strcspn(New->author, "\n")] = '\0';
            to_lower(New->name);

            printf("Amount of books: ");
            scanf("%d", &New->quantity);
            getchar();
            
            to_Sort(element, New);
        }
    }
}

void print(ref *element){ //Function to print the list of elements
    books *present = element->head;

    printf("List of books: \n\n");
    if(present == NULL){
        printf("There's no book on this list!\n");
        return;
    } else {
        while(present != NULL){
            printf("+------------------------------------------------+\n");
            printf("| ISBN: %s\n", present->isbn);
            printf("| Name: %s\n", present->name);
            printf("| Author: %s\n", present->author);
            printf("| Year of publication: %d\n", present->year);
            printf("| Amount of books: %d\n", present->quantity);
            printf("+------------------------------------------------+\n");
            printf("\n");

            present = present->next;
        }
    }
}

void print_reverse(ref *element){ //Function to print the list of elements in reverse order
    books *present = element->foot;

    printf("List of books (descending order): \n\n");

    if (present == NULL) {
        printf("There's no book on this list!\n");
    } else {
        while (present != NULL) {
            printf("+------------------------------------------------+\n");
            printf("| ISBN: %s\n", present->isbn);
            printf("| Name: %s\n", present->name);
            printf("| Author: %s\n", present->author);
            printf("| Year of publication: %d\n", present->year);
            printf("| Amount of books: %d\n", present->quantity);
            printf("+------------------------------------------------+\n");
            printf("\n");

            present = present->prev;
        }
    }
}

void remove(ref *element){ //Function to remove a book from the list
    books *present = element->head;
    books *previous = NULL;
    char search[14];

    print(element);

    insert_isbn(search);

    while(present != NULL && strcmp(search, present->isbn) != 0){ //Search in the list the ISBN code
        previous = present;
        present = present->next;
    }

    if(present != NULL){ //if found, i look for each case
        int qtd;
        printf("\nQuantity of books you want to remove: ");
        scanf("%d", &qtd);

        if(qtd < 0){ //Invalid number of books
            printf("Invalid number of books!\n");
            return;
        } else if(qtd >= present->quantity ){ //Quantity bigger than the present in list, remove the book from the list
            if(previous == NULL){ //Top of list
                element->head = present->next;
                books *toRemove = present;
                free(toRemove);
                printf("Book removed successfully!\n");
            } else if(present == element->foot){ //Foot of list
                element->foot = present->prev;
                present->prev->next = NULL;
                books *toRemove = present;
                free(toRemove);
                printf("Book removed successfully!\n");
            } else { //Middle of list
                books *toRemove = present;
                present->next->prev = present->prev;
                present->prev->next = present->next;
                free(toRemove);
                printf("Book removed successfully!\n");
            }
        } else { //Quantity of books smaller than existing
            present->quantity = present->quantity - qtd;
            printf("Book removed successfully!\n");
        } 
    } else { //Present == NULL
        printf("Book not found in list!\n");
        return;
    }
}

void search_by_isbn(ref *element){ //Function to search by isbn 
    books *present = element->head;
    char search[14];
    int count = 0;

    if(present == NULL){
        printf("There is no book present in list!\n");
        return;
    } else { 
        insert_isbn(search);

        while(present != NULL){
            if(strcmp(present->isbn, search) == 0){
                printf("+------------------------------------------------+\n");
                printf("| Name: %s\n", present->name);
                printf("| Name: %d\n", present->year);
                printf("| Name: %s\n", present->author);
                printf("| Name: %d\n", present->quantity);
                printf("+------------------------------------------------+\n");
                printf("\n");

                count++; //If found any element, increment count
            }
            present = present->next;
        }

        if(count == 0){ //If there is no ISBN present, print this message
            printf("ISBN not registered!\n");
        }
    }
} 

void search_by_author(ref *element){ //Function to search by author name
    char search[50];
    books *present = element->head;
    int count = 0;

    if(present == NULL){
        printf("No book present in list!\n");
    } else {
        printf("Name of author: ");
        getchar();
        fgets(search, sizeof(search), stdin);
        search[strcspn(search, "\n")] = '\0';
        to_lower(present->author);

        while(present != NULL){
            if(strcmp(search, present->author) == 0){
                printf("+------------------------------------------------+\n");
                printf("| Name: %s\n", present->name);
                printf("+------------------------------------------------+\n");
                printf("\n");

                count++; //If found any element, increment count
            }
            present = present->next;
        }

        if(count == 0){ //If there is no author present, print this message
            printf("Author not found!\n");
        }
    }
}

void menu(){ //Function to print the menu
    printf("+----------------------------------+\n");
    printf("|               MENU               |\n");
    printf("+----------------------------------+\n");
    printf("| 1 - Insert book                  |\n");
    printf("| 2 - Print list of books          |\n");
    printf("| 3 - Search book                  |\n");
    printf("| 4 - Remove book                  |\n");
    printf("| 0 - Exit                         |\n");
    printf("+----------------------------------+\n");
    printf("Select an option: ");
}

void free_memory(ref *element){ //Function to free the memory in the end of the code
    books *present = element->head;
    books *aux;

    while(present != NULL){
        aux = present;
        present = present->next;
        free(aux);
    }
}

int main(){
    ref element;
    int op;
    int case2;
    int case3;

    start(&element);

    do{
        menu();
        scanf("%d", &op);
        system("cls || clear");

        switch(op){
            case 1:
                insert(&element);
                printf("Press any key to continue...");
                getchar();
                system("cls || clear");
                break;
            case 2:
                printf("+------------------------+\n");
                printf("| Printing list of books:|\n");
                printf("| 1 - Ascending order    |\n");
                printf("| 2 - Descending order   |\n");
                printf("+------------------------+\n");
                scanf("%d", &case2);    
                system("cls || clear");

                switch(case2){
                    case 1:
                        print(&element);
                        break;
                    case 2:
                        print_reverse(&element);
                        break;
                    case 0:
                        printf("Leaving...\n"); 
                        break;
                    default:
                        printf("Invalid option!\n");
                        break;
                }
                break;
            case 3:
                printf("+------------+\n");
                printf("| Search by: |\n");
                printf("| 1 - ISBN   |\n");
                printf("| 2 - Author |\n");
                printf("+------------+\n");
                scanf("%d", &case3);
                system("cls || clear");

                switch(case3){
                    case 1:
                        search_by_isbn(&element);
                        break;
                    case 2:
                        search_by_author(&element);
                        break;
                    case 0:
                        printf("Exiting...\n");
                        break;
                    default:
                        printf("Invalid option!\n");
                        break;
                }
                break;
            case 4:
                remove(&element);
                printf("\n");
                break;
            case 0:
                printf("Exiting the program...\n");
                break;
            default:
                printf("Invalid option...\n");
                break;
        } 
    } while(op != 0);

    free_memory(&element);

    return 0;
}