#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>

#define BOOKPATH "/home/arim/books/"
#define LIBRARY "/home/arim/books/library.txt"
#define MAXLINE 64
#define MAXBOOKS 100
#define A_MAXLEN 64
#define T_MAXLEN 128

typedef struct {
    char author[A_MAXLEN];
    char title[T_MAXLEN];
    int pubyear;                 /* YYYY */
} Book;

Book library[MAXBOOKS];
int n_books = 0;

int
add_book(Book *b)
{
    FILE *f;
    f = fopen(LIBRARY, "a");
    if (!f) {
        perror("fopen error");
        return -1;
    }
    fprintf(f, "\n%s/%s/%d\n", b->author, b->title, b->pubyear);
    fclose(f);
    return 0;
}

void
print_book(FILE *dst, Book *b)
{
    fprintf(dst, "<tr><td class=\"author\">%s</td><td\
class=\"title\">%s</td><td>%d</td></tr>\n", b->author, b->title,
    b->pubyear);
}

static int
comp_books(const void *b, const void *c)
{
    const char *t1 = ((Book *)b)->author;
    const char *t2 = ((Book *)c)->author;

    return strcmp(t1, t2);

    // TODO: check why this is not correct

    /* for (int i = 0; t1[i] == t2[i]; i++) { */
    /*     if (t1[i] == '\0') */
    /*         return 0; */
    /*     if (t1[i] == ',') */
    /*         return 0; */
    /* } */
    /* if ((unsigned char)t1[0] < (unsigned char)t2[0]){ */
    /*     return -1; */
    /* } */
    /* return 1; */
}

void
sort_books(Book *barr, int n)   /* Lexicographic order; title */
{
    qsort(barr, n, sizeof(Book), comp_books);
}

void
parse_lib(FILE *src)
{
    char *line;
    
    size_t len = 0;
    ssize_t c;
    int i;
    
    for (i = 0; (c = getline(&line, &len, src)) != (ssize_t) -1; ++i) {
        if (i == 0) {
            continue;
        }
        
        char *a;
        char *t;
        int y;
        
        a = strtok(line, "/");
        t = strtok(NULL, "/");
        y = atoi(strtok(NULL, "/"));

        strcpy(library[i-1].author, a);
        strcpy(library[i-1].title, t);
        library[i-1].pubyear = y;
    }

    n_books = i-1;
    
    fclose(src);
}


int
main(void) // TODO: argc and argv will be used for sorting
{
    FILE *f;
    f = fopen(LIBRARY, "r");

    if (!f)
        return -1;

    /* "Parse" means: read library.txt and print to stdout
       [1] Parse library.txt
       [2] Add book (and then auto parse)
       [3] Delete book (and then auto parse)
       [4] Sort books by:
           [a] author DONE
           [t] title
           [y] year
     */

    parse_lib(f);

    /* sorting */
    sort_books(library, n_books);
    
    /* debug  */
    // printf("n_books = %d\n", n_books);
    // printf("PRINTING (sorted) LIBRARY:\n");

    fputs("<!-- script-generated -->\n", stdout);
    fputs("<table class=\"library\">\n", stdout);
    fputs("<thead><tr><td>author</td><td>title</td><td>year of\
publication</td></tr></thead>\n", stdout);
    fputs("<tbody>\n", stdout);
    for (int i = 0; i < n_books; i++) {
        print_book(stdout, &library[i]);
        // printf("%s %s %d\n", library[i].author, library[i].title, library[i].pubyear);
    }
    fputs("</tbody>\n", stdout);
    fputs("</table>", stdout);
    
    return 0;
}
