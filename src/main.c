#include <stdio.h>
#include "utlist.h"

struct node_t {
    char *name;
    struct node_t *next;
};

int main() {
    struct node_t *head =NULL,*el = NULL;
    struct node_t el1 = {.name="11111"};
    LL_APPEND(head,&el1);
    struct node_t el2 = {.name="2222"};
    LL_APPEND(head,&el2);


    LL_FOREACH(head,el) {
        printf("%s\n",el->name);
    };




    printf("Hello, World!\n");
    return 0;
}
