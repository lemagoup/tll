#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "tll.h"

llList* tll_create(void) {
    llList* list = calloc(1, sizeof(llList));
    
    return list;
}

int tll_size(llList* list) {
    return list->count;
}

void tll_prepend(llList* list, void * data) {
    llElem* new_str_element = malloc(sizeof(llElem));

    if (!list->count) {
        list->head = new_str_element;
        list->tail = new_str_element;
    }
    else {
        new_str_element->next = list->head;
        new_str_element->prev = 0;
        list->head = new_str_element;
    }
    new_str_element->data = data;
    list->count++;
}

void tll_append(llList* list, void * data) {
    llElem* new_str_element = malloc(sizeof(llElem));

    if (!list->count) {
        list->head = new_str_element;
        list->tail = new_str_element;
    }
    else {
        new_str_element->prev = list->tail;
        new_str_element->next = 0;
        list->tail->next = new_str_element;
        list->tail = new_str_element;
    }
    new_str_element->data = data;
    list->count++;
}

void tll_list_print(llList* ll) {
    printf("count: %d\n", ll->count);
    printf("head: %p\n", ll->head);
    printf("tail: %p\n", ll->tail);
}

void tll_elem_print(llElem* elt) {
    printf("elt: %p\n", elt);
    printf("\tdata: %p | %s\n", elt->data, elt->data ? (char*)elt->data : "(NULL)");
    printf("\tnext: %p\n", elt->next);
    printf("\tprev: %p\n", elt->prev);
}

void tll_remove(llList* list, llElem* elt) {
    if(elt->data)
        free(elt->data);

    if(elt->prev)
        elt->prev->next = elt->next;
    if(elt->next)
        elt->next->prev = elt->prev;

    free(elt);
    list->count--;
}

void tll_destroy(llList* list) {
    llElem* elt = list->head;
    llElem* tmp;

    while(elt){
        tmp = elt->next;
        tll_remove(list, elt);
        elt = tmp;
    };

    free(list);
}

void tll_swap(llElem* elem1, llElem* elem2) {
  llElem* tmp = elem2->next;
  if(tmp)
    tmp->prev = elem1;
  elem2->next = elem1;
  elem1->next = tmp;

  tmp = elem1->prev;
  if(tmp)
    tmp->next = elem2;
  elem1->prev = elem2;
  elem2->prev = tmp;
}

llList* tll_cat(llList* list1, llList* list2) {
    list1->tail->next = list2->head;
    list2->head->prev = list1->tail;
    list1->tail = list2->tail;
    list1->count += list2->count;

    return list1;
}

llList* tll_split(llList* list1) {
    llList* list2 = tll_create();
    list2->tail = list1->tail;
    llElem* cur = list1->head;
    list2->count = list1->count/2;
    list1->count -= list2->count;

    for (int i=1 ; i < list1->count ; i++){
        cur = cur->next; 
    }

    list2->head = cur->next;
    list2->head->prev = 0;
    cur->next = 0;
    list1->tail = cur;

    return list2;  
}

void tll_bubble_sort(llList* list) {
    int sorted;
    do {
        sorted = 1;
        llElem* cur = list->head;
        while(cur->next) {
            int* i = cur->data;
            int* j = cur->next->data;
            if (*i > *j) {
                sorted = 0;
                tll_swap(cur, cur->next);

                if(cur == list->head) {
                    list->head = cur->prev;
                }
                if(cur->next == list->tail) {
                    list->tail = cur;
                }
            } else {
                cur = cur->next;
            }
        }
    } while (!sorted);
}

static void tll_add(llList* list, llElem* elt) {
    if (!list->head) {
        list->head = elt;
        list->tail = elt;
    }
    else {
        elt->prev = list->tail;
        list->tail->next = elt;
        list->tail = elt;
    }
    list->count++;
}

static llList* tll_merge_sort(llList* list1, llList* list2) {
    llList* sorted = tll_create();
    llElem* smallest;

    if(list1->count == 0) {
        return list2;
    }
    else if(list2->count == 0) {
        return list1;
    } else {
        int* i = list1->head->data;
        int* j = list2->head->data;

        if (*i < *j) {
            smallest = list1->head;
            list1->head = smallest->next;
            list1->count--;
        }
        else
        {
            smallest = list2->head;
            list2->head = smallest->next;
            list2->count--;
        }
        smallest->prev = 0;
        smallest->next = 0;
        tll_add(sorted, smallest);

        return tll_cat(sorted, tll_merge_sort(list1, list2));
    }
}

llList* tll_msort(llList* list) {
    if(list->count == 1) {
        return list;
    }
    else {
        llList* list2 = tll_split(list);
        list2 = tll_merge_sort(tll_msort(list),tll_msort(list2));
        return list2;
    }
}

int main() {
    // Bubble sort
    srand(time(NULL));
    llList* listb = tll_create();

    for(int i = 0 ; i < 10000 ; i++) {
        int* val = malloc(sizeof(int));
        *val = rand() % 1000;
        tll_append(listb, val);
    }

    // llElem* elt = lista->head;
    // while(elt) {
    //     int* tmp = elt->data;
    //     printf("%d->", *tmp);
    //     elt = elt->next;
    // }

    printf("\n");
    clock_t begin = clock();
    //tll_bubble_sort(lista);
    clock_t end = clock();
    double time_spent = (double)(end - begin);
    printf("bubble time : %f\n", time_spent);

    // elt = lista->head;
    // while(elt) {
    //     int* tmp = elt->data;
    //     printf("%d->", *tmp);
    //     elt = elt->next;
    // }

    // printf("\n");

    begin = clock();
    tll_msort(listb);
    end = clock();
    time_spent = (double)(end - begin);
    printf("merge time : %f\n", time_spent);
    // elt = sorted->head;
    // while(elt) {
    //     int* tmp = elt->data;
    //     printf("%d->", *tmp);
    //     elt = elt->next;
    // }

    // printf("\n");

    tll_destroy(listb);

    return 0;
}

int test_split() {
    srand(time(NULL));
    llList* list1 = tll_create();

    for(int i = 0 ; i < 10 ; i++) {
        int* val = malloc(sizeof(int));
        *val = rand() % 100;
        tll_append(list1, val);
    }

    printf("list : ");
    tll_list_print(list1);
    printf("\n");
    llElem* elt = list1->head;
    while(elt) {
        int* tmp = elt->data;
        printf("%d->", *tmp);
        elt = elt->next;
    }


    llList* list2 = tll_split(list1);
    printf("list1 : ");
    tll_list_print(list1);
    printf("\n");
    elt = list1->head;
    while(elt) {
        int* tmp = elt->data;
        printf("%d->", *tmp);
        elt = elt->next;
    }
    printf("\n");


    printf("list2 : ");
    tll_list_print(list2);
    printf("\n");
    elt = list2->head;
    while(elt) {
        int* tmp = elt->data;
        printf("%d->", *tmp);
        elt = elt->next;
    }
    printf("\n");


    return 0;
}


int test_cat() {
    srand(time(NULL));
    llList* list1 = tll_create();
    llList* list2 = tll_create();

    for(int i = 0 ; i < 5 ; i++) {
        int* val = malloc(sizeof(int));
        *val = rand() % 100;
        tll_append(list1, val);
    }

    for(int i = 0 ; i < 5 ; i++) {
        int* val = malloc(sizeof(int));
        *val = rand() % 100;
        tll_append(list2, val);
    }

    printf("list1 : ");
    tll_list_print(list1);
    printf("\n");
    llElem* elt = list1->head;
    while(elt) {
        int* tmp = elt->data;
        printf("%d->", *tmp);
        elt = elt->next;
    }


    printf("\nlist2 : ");
    tll_list_print(list2);
    printf("\n");
    elt = list2->head;
    while(elt) {
        int* tmp = elt->data;
        printf("%d->", *tmp);
        elt = elt->next;
    }
    printf("\nlist cat : ");

    llList* cat = tll_cat(list1,list2);
    tll_list_print(cat);
    printf("\n");
    elt = cat->head;
    while(elt) {
        int* tmp = elt->data;
        printf("%d->", *tmp);
        elt = elt->next;
    }
    return 0;
}

int test2() {
    llList* list = tll_create();

    tll_append(list, strdup("test"));
    tll_append(list, strdup("toto"));
    tll_append(list, strdup("#######"));
    tll_append(list, strdup("tata"));

    printf("Initiate List\n");

    llElem* cur = list->head;
    do {
        printf("%s->", (char*)cur->data);
        cur = cur->next;
    } while(cur);

    tll_destroy(list);

    return 0;
}

int test1() {

    llList* list = tll_create();
    char* test = malloc(sizeof(char)*5);
    strcpy(test, "test");
    char* toto = malloc(sizeof(char)*5);
    strcpy(toto, "toto");

    printf("List created\n");
    tll_list_print(list);
    tll_append(list, test);

    printf("print after add new str elem\n");
    tll_list_print(list);
    tll_elem_print(list->head);
    tll_append(list, toto);

    printf("print after adding second elt\n");
    tll_list_print(list);
    tll_elem_print(list->head->next);
    tll_elem_print(list->tail);
    tll_elem_print(list->tail->prev);

    return 0;
}
