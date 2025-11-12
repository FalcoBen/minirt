#include "my_malloc.h"


static void free_all(t_alloc **head)
{
    t_alloc *tmp;
    t_alloc *cur;

    if (!head || !*head)
        return ;
    tmp = *head;
    while (tmp)
    {
        cur = tmp->next;
        free(tmp->holder);
        tmp->holder = NULL;
        free(tmp);
        tmp = NULL;
        tmp = cur;
    }
    return ;
}

// static void free_all1(t_alloc1 **head)
// {
//     t_alloc1 *tmp;
//     t_alloc1 *cur;

//     if (!head || !*head)
//         return ;
//     tmp = *head;
//     while (tmp)
//     {
//         cur = tmp->next;
//         free(tmp->holder);
//         tmp->holder = NULL;
//         // free(tmp);
//         tmp = NULL;
//         tmp = cur;
//     }
//     return ;
// }

static t_alloc *new_node(size_t size, t_alloc **head)
{
    t_alloc *node;

    node = malloc(sizeof(t_alloc));
    if (!node)
    {
        free_all(head);
        printf("out of memory\n");
        exit(1);
    }
    node->holder = malloc(size);
    if (!node->holder)
    {
        printf("out of memory\n");
        free(node);
        free_all(head);
        exit (1);
    }
    node->next = NULL;
    return (node);
}

// static t_alloc1 *new_node1(size_t size, t_alloc1 **head)
// {
//     t_alloc1 *node;

//     node = malloc(sizeof(t_alloc1));
//     if (!node)
//     {
//         free_all1(head);
//         printf("out of memory\n");
//         exit(1);
//     }
//     node->holder = malloc(size);
//     if (!node->holder)
//     {
//         printf("out of memory\n");
//         free(node);
//         free_all1(head);
//         exit (1);
//     }
//     node->next = NULL;
//     return (node);
// }

static void add_list(t_alloc **head, t_alloc *node)
{
    t_alloc *tmp;
    if (!head || !node)
        return ;
    if (!*head)
    {
        *head = node;
        return ;
    }
    tmp = *head;
    *head = node;
    node->next = tmp;
    return ;
}

// static void add_list1(t_alloc1 **head, t_alloc1 *node)
// {
//     t_alloc1 *tmp;
//     if (!head || !node)
//         return ;
//     if (!*head)
//     {
//         *head = node;
//         return ;
//     }
//     tmp = *head;
//     *head = node;
//     node->next = tmp;
//     return ;
// }

void *alloc(size_t size, bool _free)
{
    static t_alloc *garbage;
    t_alloc *node = NULL;
    // void *ptr;
    if (_free)
    {
        free_all(&garbage);
        return (NULL);
    }
    node = new_node(size, &garbage);   
    add_list(&garbage, node);
    return node->holder;
}
