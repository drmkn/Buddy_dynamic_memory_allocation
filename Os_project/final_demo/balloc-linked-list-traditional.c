#include "common.h"
#include <assert.h>
#include <time.h>
#include "balloc.h"
#define MAX_VALUE 1000

typedef int DATA;
typedef struct node {
    DATA data;
    struct node *next, *prev;
} NODE;
typedef struct {
    unsigned int length;
    NODE *head, *tail;
} LIST;

NODE *create_node(DATA d);
void init_list(LIST *l);
int insert(LIST *l, unsigned int index, DATA d);
int delete(LIST *l, unsigned int index, DATA *d);
int find_index_of(LIST *l, DATA d);
int find_value_at(LIST *l, unsigned int index, DATA *d);
void print_list(LIST *l);
void free_list(LIST *l);


NODE *create_node(int d) {
    NODE *nptr;
    if (NULL == (nptr = (NODE *)bmalloc(sizeof(NODE))))
        ERR_MESG("out of memory");
    nptr->data = d;
    nptr->next =nptr->prev =  NULL;
    return nptr;
}

void init_list(LIST *l) {
    l->length = 0;
    l->head = l->tail = NULL;
    return;
}

int insert(LIST *l, unsigned int index, DATA d) {
    int i;
    //freelist_Status();
    NODE *newnode, *nptr1, *nptr2 = NULL;
    if (index < 0 || index > l->length)
        return -1;
    for (i = 0, nptr1 = l->head; i != index; i++, nptr1 = nptr1->next)
        nptr2 = nptr1;
    newnode = create_node(d);
    newnode->prev = nptr2;
    newnode->next = nptr1;
    if (nptr2 == NULL) /* inserted at head */
        l->head = newnode;
    else
        nptr2->next = newnode;
    if (nptr1 == NULL) /* inserted at tail */
        l->tail = newnode;
    else
        nptr1->prev = newnode;
    l->length++;
    //freelist_Status();
    return 1;
}

int delete(LIST *l, unsigned int index, DATA *d) {
    int i;
    NODE *deletednode, *nptr1, *nptr2 = NULL;
    if (index < 0 || index >= l->length)
        return -1;
    for (i = 0, nptr1 = l->head; i != index; i++, nptr1 = nptr1->next)
        nptr2 = nptr1;
    assert(nptr1 != NULL);

    if (nptr2 == NULL) /* head was deleted */
        l->head = nptr1->next;
    else
        nptr2->next = nptr1->next;
    if (nptr1->next == NULL) /* tail was deleted */
        l->tail = nptr2;
    else
        nptr1->next->prev = nptr2;
    l->length--;

    deletednode = nptr1;
    deletednode->next = deletednode->prev = NULL;
    *d = deletednode->data;
    bfree(deletednode);
    //freelist_Status();
    return 1;
}

int find_index_of(LIST *l, DATA d) {
    int i;
    NODE *nptr;
    for (i = 0, nptr = l->head; nptr != NULL && nptr->data != d; i++, nptr = nptr->next);
    if (nptr == NULL)
        return -1;
    else /* nptr->data == d */
        return i;
}

int find_index_in_sorted(LIST *l, DATA d) {
    int i;
    NODE *nptr;
    for (i = 0, nptr = l->head; nptr != NULL && nptr->data <= d; i++, nptr = nptr->next);
    if (nptr == NULL)
        assert(i == l->length);
    return i;
}

int find_value_at(LIST *l, unsigned int index, DATA *d) {
    int i;
    NODE *nptr;
    if (index < 0 || index >= l->length)
        return -1;
    for (i = 0, nptr = l->head; i != index; i++, nptr = nptr->next);
    *d = nptr->data;
    return 1;
}

void print_list(LIST *l) {
    int i;
    NODE *nptr;
    for (i = 1, nptr = l->head; i <= l->length; i++, nptr = nptr->next) {
        printf("%6d %6d\t", i-1, nptr->data);
        if (i%5 == 0) putchar('\n');
    }
    if ((i-1)%5) putchar('\n');
    return;
}

void free_list(LIST *l) {
    NODE *nptr1, *nptr2;
    if (l->head == NULL) return;
    for (nptr1 = l->head, nptr2 = nptr1->next;
         nptr1 != NULL;
         nptr1 = nptr2, nptr2 = nptr2->next)
        bfree(nptr1);
    return;
}


int main(int ac, char *av[])
{
    int n, index, i;
    DATA d, d1;
    LIST l;

    clock_t starting_time,stoping_time;
    double cpu_time_used;

    starting_time = clock();

    if (ac != 2)
        ERR_MESG("Usage: linked-list <number>");
    n = atoi(av[1]);
    //srand((int) time(NULL));

    init_list(&l);
    for (i = 0; i < n; i++) {
        d = rand() % MAX_VALUE;
        index = find_index_in_sorted(&l, d);
        if (-1 == insert(&l, index, d))
            fprintf(stderr, "Inserting %d at index %d failed\n", d, index);
        else
            fprintf(stderr, "Inserted %d at index %d successfully\n", d, index);
        print_list(&l);
    }

    while (l.length > 0) {
        index = rand() % l.length;
        if (-1 == find_value_at(&l, index, &d))
            fprintf(stderr, "Failed to find value at index %d\n", index);
        else {
            fprintf(stderr, "Found %d at index %d, deleting\n", d, index);
            if (-1 == delete(&l, index, &d1))
                fprintf(stderr, "Delete failed\n");
            else
                assert(d == d1);
            print_list(&l);
        }
    }

    free_list(&l);
    freelist_Status();

    stoping_time = clock();
    cpu_time_used = ((double)(stoping_time - starting_time))/CLOCKS_PER_SEC;
    printf("\nTotal execution time of balloc-linked-list-traditional.c %f seconds for n=%d\n",cpu_time_used,n);
    return 0;
}