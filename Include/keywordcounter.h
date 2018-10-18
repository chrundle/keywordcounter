#include <climits>
#include <iostream>
#include <string.h>
#include <unordered_map>
using namespace std;

/* Node struct used for nodes in Fibonacci heap implementation */
class Node {
  /* -------------------- Public members of node -------------------- */
  public:
    /* -------- Integer values -------- */
    long int   data ; /* Data in node (keyword frequencies) */
    long int degree ; /* Degree of node */

    /* -------- String values -------- */
    string  keyword ; /* Keyword at node (search keyword) */

    /* -------- Boolean values -------- */
    bool   childcut ; /* TRUE  => Node has lost child since it became
                                  child of its current parent 
                         FALSE => Set F by remove min, which is the  
                                  only operation that makes one node 
                                  child of another
                         Note: Undefined for a root node              */

    /* -------- Node pointers -------- */
    Node     *child ; /* Child pointer */
    Node  *lsibling ; /* Left sibling pointer */
    Node  *rsibling ; /* Right sibling pointer */
    Node    *parent ; /* Parent pointer */

    /* -------------------------------------------------------------- */
    /* -------------------- Default Constructor --------------------- */
    /* -------------------------------------------------------------- */
    Node() {
        /* Set parent pointer to NULL */
        parent = NULL ;
        /* Set left and right sibling pointers to each other */
        lsibling = this ;
        rsibling = this ;
        /* Set child pointer to NULL */
        child = NULL ;
        /* Set degree to zero */
        degree = 0 ;
    }
    /* -------------------------------------------------------------- */
    /* ------------------ Parametrized Constructor ------------------ */
    /* -------------------------------------------------------------- */
    /* Initialize node with data value */
    Node(string key, long int d) {
        /* Set parent pointer to NULL */
        parent = NULL ;
        /* Set L and R sibling pointers to pointer for this node */
        lsibling = this ;
        rsibling = this ;
        /* Set child pointer to NULL */
        child = NULL ;
        /* Set degree to zero */
        degree = 0 ;

        /* Set keyword to input string key */
        keyword = key ;
        /* Set data to input d */
        data = d ;
    }
} ;

/* FibonacciHeap class used for Fibonacci max heap implementation */
class FibonacciHeap {
  /* -------------------- Private members of heap ------------------- */
  private:
    Node       *max ; /* Pointer to node containing the max key */

    /* -------------------------------------------------------------- */
    /* ----------------------- only_child() ------------------------- */
    /* -------------------------------------------------------------- */
    /* Boolean function which checks if node has any siblings.       
       Returns TRUE if given node is only child
               FALSE if given node has at least one sibling           */
    bool only_child(Node *nd) {
        return (nd->rsibling == nd) ;
    }

    /* -------------------------------------------------------------- */
    /* ------------------------- barren() --------------------------- */
    /* -------------------------------------------------------------- */
    /* Boolean function which checks if node has any children.       
       Returns TRUE if given node has no children 
               FALSE if given node has at least one child             */
    bool barren(Node *nd) {
        return (nd->child == NULL) ;
    }

    /* -------------------------------------------------------------- */
    /* ------------------------- orphan() --------------------------- */
    /* -------------------------------------------------------------- */
    /* Function which checks if node has a parent.       
       If given node has a parent, it returns the child pointer from
       the parent. If the given node has no parent, it returns NULL.  */
    Node *orphan(Node *nd) {
        /* Initialize varibles */
        Node *t ;

        /* Check if given node has no siblings */
        if (only_child(nd)) {/* Given node has no siblings */
            /* Check if parent pointer is NULL */
            if (nd->parent != NULL) {/* Parent pointer is not NULL */
                /* Return pointer to given node */
                return nd ;
            }
            else {/* Parent pointer is NULL */
                /* Return NULL pointer */
                return NULL ;
            }
        }

        /* Set t to right sibling of given node */
        t = nd->rsibling ;

        /* Loop through sibling list and check if each one has parent */
        while (t != nd) {
            /* Check if parent pointer is not NULL */
            if (t->parent != NULL) {
                /* Return pointer of current child */
                return t ;
            }
            else {
                /* Set t to next sibling in linked list */
                t = t->rsibling ;
            }
        }
        /* Every other parent pointer was NULL. Check if parent pointer 
           is NULL */
        if (nd->parent != NULL) {/* Parent pointer is not NULL */
            /* Return pointer to given node */
            return nd ;
        }
        else {/* Parent pointer is NULL */
            /* Return NULL pointer */
            return NULL ;
        }
    }

  /* -------------------- Public members of heap -------------------- */
  public:
    unordered_map<string, Node*> hashmap ; /* Hash table for keywords
                                              mapped to pointers to 
                                              nodes                   */

    /* -------------------------------------------------------------- */
    /* ------------------ Parametrized Constructor ------------------ */
    /* -------------------------------------------------------------- */
    FibonacciHeap(string keyword, long int frequency) {
        /* Set max pointer to initialized node */
        hashmap[keyword] = new Node(keyword, frequency) ;
        /* Use hashmap to associate keyword with pointer to new node */
        max = hashmap[keyword] ;
    }

    /* -------------------------------------------------------------- */
    /* ------------------------ find_max() -------------------------- */
    /* -------------------------------------------------------------- */
    /* Find and return maximum value in heap. */
    Node *find_max() {
        return max ;
    }

    /* -------------------------------------------------------------- */
    /* ------------------------- insert() --------------------------- */
    /* -------------------------------------------------------------- */
    /* Insert node into heap. */
    void insert(string keyword, long int frequency) {
        /* Create new node with data equal to number of queries */
        hashmap[keyword] = new Node(keyword, frequency) ;
        /* Set right sibling of new node to right sibling to max node */
        hashmap[keyword]->rsibling = max->rsibling ;
        /* Set left sibling of new node to max node */
        hashmap[keyword]->lsibling = max ;
        /* Set right sibling of max node to new node */
        max->rsibling = hashmap[keyword] ;
        /* Check if max pointer needs to be updated */
        if(max->data < hashmap[keyword]->data) {
            /* Set max pointer to pointer of new node */
            max = hashmap[keyword] ;
        }
    }

    /* -------------------------------------------------------------- */
    /* ----------------------- remove_max() ------------------------- */
    /* -------------------------------------------------------------- */
    /* Remove max node from heap. */
    /* This function is required when we want to determine most 
       requested queries.                                             */
    void remove_max() {
        /* Initialize variables */
        Node *t, *s ;

        /* ------ Step 1: Delete max from heap ------ */
        /* Check if max has no siblings */
        if (only_child(max)) {/* max has no siblings */
            if (barren(max)) {/* max has no children */
                /* In this case, heap was single max node */
                /* FIX: NEED TO DEAL WITH THIS CASE */
                return ;
            }
            else {/* max has at least one child */
                /* Set max pointer to any child of current max */
                max = max->child ;
            }
        }
        else {/* max has at least one sibling */
            /* Check if max has children */
            if (barren(max)) {/* max has no children */
                /* Set t to right sibling of max */
                t = max->rsibling ;
                /* Set s to left sibling of max */
                s = max->lsibling ;

                /* -- Remove max by changing sibling pointers -- */
                /* Set right sibling of s to t */
                s->rsibling = t ;
                /* Set left sibling of t to s */
                t->lsibling = s ;

                /* Set max pointer to any sibling of current max */
                max = t ;
            }
            else {/* max has at least one child */
                /* ---- Add children to root linked list ---- */
                /* Set t to child of max */
                t = max->child ;
                /* Set s to t's left sibling */
                s = t->lsibling ;
    
                /* -- Meld of right endpoint -- */
                /* Set right sibling of s to right sibling of max node */
                s->rsibling = max->rsibling ;
                /* Close right end of meld by linking back to s */
                s->rsibling->lsibling = s ;
    
                /* -- Meld of left endpoint -- */
                /* Set left sibling of t to left sibling of max node */
                t->lsibling = max->lsibling ;
                /* Close left end of meld by linking back to t */
                t->lsibling->rsibling = t ;
            }
        }

        /* ------ Step 2: Determine new max ------ */
        /* Set s to current max */
        s = max ;
        /* Set t to right sibling of max */
        t = s->rsibling ;

        /* Loop through sibling list and check if each one is max */
        while (t != s) {
            /* Check if data at t is larger than max data */
            if (max->data < t->data) {
                /* Set max to t */
                max = t ;
            }
            /* Set t to next sibling in linked list */
            t = t->rsibling ;
        }
    }

#if 0
    /* The remove function is not required for this exercise */

    /* -------------------------------------------------------------- */
    /* ------------------------- remove() --------------------------- */
    /* -------------------------------------------------------------- */
    /* Remove node from heap given pointer to node. */
    /* This function is required for the increase_key() function.     */
    void remove(Node *nd) {
        /* Check if node contains maximum key */
        if (nd == max) {/* Node to remove contains max key */
            /* Perform remove max */
            remove_max() ;
        }
        else {/* Node to remove does not contain max key */

        }
    }

    /* -------------------------------------------------------------- */
    /* ---------------------- increase_key() ------------------------ */
    /* -------------------------------------------------------------- */
    /* Given a node, increase its key value by a given amount. */
    void increase_key(Node *nd, long int amount) {
        /* Initialize variables */
        Node *t ;

        /* Check if given node is orphan and store output in t */
        t = orphan(nd) ;

        if (t == nd) {/* Given node points to parent */
            /* Remove subtree rooted at nd and add to top level */
        }
        else if (t != NULL) {/* Given node does not point to parent */
            /* ---- Remove subtree rooted at given node ---- */
            /* Set parent child pointer to NULL */
        }
        else {/* Given node does not have parent */
            /* Nothing to do */
        }

        /* Increase nd key value by given amount */
        nd->data += amount ;
        /* Check if max pointer needs to be updated */
        if(max->data < nd->data) {
            /* Set max pointer to pointer of new node */
            max = nd ;
        }
    }
#endif
} ;
