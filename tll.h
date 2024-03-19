/* tll is the implementation of two linked list in C */
struct llElem;
typedef struct llElem {
    void* data;
    struct llElem* prev;
    struct llElem* next;
} llElem;

struct llList;
typedef struct llList {
    int count;
    llElem *head;
    llElem *tail;
} llList;


llList* tll_create(void);
void tll_destroy(llList* list);
int tll_size(llList* list);
void tll_prepend(llList* list, void * data);
void tll_append(llList* list, void * data);
llList* tll_cat(llList* list1, llList* list2);
llList* tll_split(llList* list1);

void tll_remove(llList* list, llElem* elt);
void tll_swap(llElem* elem1, llElem* elem2);

/* sort functions */
void tll_bubble_sort(llList* list);
llList* tll_msort(llList* list);

/* Debug functions */
void tll_list_print(llList* ll);
void tll_elem_print(llElem* elt);
