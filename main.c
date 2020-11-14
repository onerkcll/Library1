#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//case option
#define OPTION_CREATE_BOOK 0
#define OPTION_READ_BOOK 1
#define OPTION_LIST_BOOKS 2
#define BOOK_FLAG_SOLD 0b00000001 //binary 1  bit masking
typedef unsigned char BOOK_FLAGS;

struct book
{
    char title[20];
    char description[200];
    //allias
    BOOK_FLAGS flags;
};
//Pointer file
FILE *f = NULL;
struct book *ptr = NULL;
int handle_create()
{
    struct book b;
    memset(&b, 0, sizeof(b));
    printf("Enter the books name :\n");
    //Reading from keyboard
    fgets(b.title, sizeof(b.title), stdin);

    printf("Enter the description:\n\a");
    fgets(b.description, sizeof(b.description), stdin);

    printf("Book name : %s\n", b.title);
    printf("Book Description:%s\a\n", b.description);
    printf("Has the book sold?: Y/N?\n");
    char c = 0;
    scanf("%c", &c);
    if (c == 'Y' || c == 'y')
    {
        b.flags |= BOOK_FLAG_SOLD;
    }

    //fixing overwrite , im holding start point of my newfile or end of last one  you know
    fseek(f, 0, SEEK_END);
    //this will write entire structure to the  file
    fwrite(&b, sizeof(b), 1, f);
}
int how_many_books()
{

    fseek(f, 0, SEEK_END);
    unsigned long total_size = ftell(f);
    fseek(f, 0, SEEK_SET);
    return total_size / sizeof(struct book);
}
void view_book(struct book *book)
{
    printf("Title:%s\n", book->title);
    printf("Description: %s\n", book->description);
    if (book->flags & BOOK_FLAG_SOLD)
    {
        printf("This book was sold , sorry\n");
    }
    else
    {
        printf("U CAN PURCHASE THIS BOOK");
    }
}
int handle_list()
{
    struct book b;
    fseek(f, 0, SEEK_SET);
    int index = 0;
    int total_books = how_many_books();
    //                                       struct kadar          kere
    ptr = (struct book *)malloc(sizeof(struct book) * total_books);
    while (fread(&b, sizeof(b), 1, f) == 1)
    {
        memcpy(&ptr[index], &b, sizeof(b));
        printf("%i- %s\n", index, b.title);

        index++;
    }
    int option = 0;
    printf("Choose a book  ");
    scanf("%i", &option);
    if (option < 0 || option > total_books)
    {
        printf("invalid book");
        return -1;
    }
    view_book(&ptr[option]);
    free(ptr);
    return 0;
}
int setup_file()
{
    //that will open binary file for reading and writing
    f = fopen("./data.bin", "r+");
    if (!f)
    {
        printf("Failed to open file");
        return -1;
    }
    return 0;
}
//switch func
int choose_option()
{
    int option = 0;
    printf("Enter an option:\n");
    scanf("%i", &option);
    //fixing1 reading new line
    getchar();
    int res = 0;
    switch (option)
    {
    case OPTION_CREATE_BOOK:
        res = handle_create();
        if (res < 0)
        {
            return res;
        }

        break;
    case OPTION_READ_BOOK:
        printf("You selected to read a book\n");
        break;
    case OPTION_LIST_BOOKS:
        handle_list();
        break;
    default:
        printf("No invalid option selection\n ");
        return -1;
    }

    return 0;
}

int main(int argc, char **argv)
{
    if (setup_file() < 0)
        return -1;
    if (choose_option() < 0)
        return -1;

    fclose(f);
    return 0;
}