#include <math.h>
#include <climits>
#include <iostream>
#include <string.h>
#include <unordered_map>
using namespace std;

/* Node struct used for nodes in Fibonacci heap implementation */
class Node {
  /* ------------------- Private members of node -------------------- */
  private:
    /* -------------------------------------------------------------- */
    /* ------------------------- merge() ---------------------------- */
    /* -------------------------------------------------------------- */
    /* Void function which merges two trees of same degree, the 
       result is rooted at current node and the tree rooted at the 
       given node becomes a subtree.                                  */
    void merge(Node *nd) {
        /* Check if degrees of two nodes are different */
        if (degree != nd->degree) {
            /* Print error message */
            cout << "NODE::merge(): degrees of trees are not the same. " ;
            cout << "Cannot perform merge(). Exiting function. " << endl ;

            /* Exit function */
            return ;
        }
        /* Initialize variables */
        long int i ;

        /* Move down tree and combine chilren to circular linked list */ 
    }
  /* -------------------- Public members of node -------------------- */
  public:
    /* -------- Integer values -------- */
    long int   data ; /* Data in node (keyword frequencies) */
    long int degree ; /* Degree of node (number of children) */

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
    long int      n ; /* Number of nodes in fibonacci heap */

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
        return (nd->degree == 0) ;
    }

    /* -------------------------------------------------------------- */
    /* ------------------------- orphan() --------------------------- */
    /* -------------------------------------------------------------- */
    /* Function which checks if node has a parent.       
       If the given node has no parent, it returns NULL. If given 
       node has a parent, it returns the pointer nd->parent->child.   */
    Node *orphan(Node *nd) {
        /* Check if parent pointer is NULL */
        if (nd->parent == NULL) {/* nd has no parent */
            /* Return NULL pointer */
            return NULL ;
        }
        else {/* nd has a parent */
            /* Return nd's parent's child pointer */
            return nd->parent->child ;
        }
    }

    /* -------------------------------------------------------------- */
    /* ----------------------- rank_bound() ------------------------- */
    /* -------------------------------------------------------------- */
    /* Compute bound on rank based on current number of nodes.        */       
    long int rank_bound() {
        /* Rank of heap bounded by log_{phi}(n), where phi ~ 1.618. */
        return (long int)(log(n) / log(1.62)) ;
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
        /* Set number of nodes in heap to 1 */
        n = 1 ;
    }

    /* -------------------------------------------------------------- */
    /* --------------------- number_of_nodes() ---------------------- */
    /* -------------------------------------------------------------- */
    /* Return number of nodes in heap. */
    long int number_of_nodes() {
        return n ;
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
        /* Set right sibling's left sibling of max node to new node */
        max->rsibling->lsibling = hashmap[keyword] ;
        /* Set left sibling of new node to max node */
        hashmap[keyword]->lsibling = max ;
        /* Set right sibling of max node to new node */
        max->rsibling = hashmap[keyword] ;
        /* Check if max pointer needs to be updated */
        if(max->data < hashmap[keyword]->data) {
            /* Set max pointer to pointer of new node */
            max = hashmap[keyword] ;
        }
        /* Increase number of nodes in heap */
        n += 1 ;
    }

    /* -------------------------------------------------------------- */
    /* ----------------------- remove_max() ------------------------- */
    /* -------------------------------------------------------------- */
    /* Remove max node from heap. */
    /* This function is required when we want to determine most 
       requested queries.                                             */
    void remove_max() {
        /* Initialize variables */
        long int i, k, degree ;
        Node *t, *s, *ndmin ;

        #ifdef DBUG_PRINT
        cout << "DEBUG::remove_max(): In remove_max()" << endl ;
        #endif

        /* ------ Step 1: Delete max from heap ------ */
        /* Check if max has no siblings */
        if (only_child(max)) {/* max has no siblings */
            #ifdef DBUG_PRINT
            cout << "DEBUG: max node has no siblings" << endl ;
            #endif

            if (barren(max)) {/* max has no children */
                #ifdef DBUG_PRINT
                cout << "DEBUG::remove_max(): max node has no children" << endl ;
                #endif

                /* In this case, heap was single max node */
                /* FIX: NEED TO DEAL WITH THIS CASE */
                return ;
            }
            else {/* max has at least one child */
                #ifdef DBUG_PRINT
                cout << "DEBUG::remove_max(): max has at least one child" << endl ;
                #endif
                /* Set max pointer to any child of current max */
                max = max->child ;
            }
        }
        else {/* max has at least one sibling */
            #ifdef DBUG_PRINT
            cout << "DEBUG::remove_max(): max has at least one sibling" << endl ;
            #endif

            /* Check if max has children */
            if (barren(max)) {/* max has no children */
                #ifdef DBUG_PRINT
                cout << "DEBUG::remove_max(): max has no children" << endl ;
                #endif

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
                #ifdef DBUG_PRINT
                cout << "DEBUG::remove_max(): max has at least one child" << endl ;
                #endif

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
        /* Decrease total number of nodes by 1 since max has been removed */
        n -= 1 ;

        /* ------ Step 2: Determine new max ------ */
        #ifdef DBUG_PRINT
        cout << "DEBUG::remove_max(): Determine new max" << endl ;
        #endif
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

        /* ---- Step 3: Consolidate trees so no two roots have same rank ---- */
        #ifdef DBUG_PRINT
        cout << "DEBUG::remove_max(): Consolidate trees" << endl ;
        #endif
        /* -- Initialize array to hold node pointers by rank -- */
        k = rank_bound() + 1 ; /* bound on rank */
        Node **rank_tree[k] ;
        /* Set all pointers in rank_tree array to null */
        for (i = 0; i < k; i++) {
            rank_tree[i] = NULL ;
        }
        /* -- Move through all roots and merge trees of same rank -- */
        /* Add degree of max node to rank tree */
        rank_tree[max->degree] = max ;
        /* Use s to keep track of current node */
        s = max->rsibling ;
        /* Move through roots in node and merge trees of same rank */
        while (s != max) {
            /* Set current degree */
            degree = s->degree ;

            /* Check if degree of current node is not in rank_tree */
            if (rank_tree[degree] == NULL) {/* degree of s not in rank_tree */
                /* Add degree of s to rank tree */
                rank_tree[degree] = s ;
            }
            else {/* degree of s already in rank_tree */
                /* -- consolidate s with rank_tree[degree] -- */
                /* Compare root values to determine largest */
                if (rank_tree[degree]->data < s->data) {/* larger root at s */
                    /* Set t = s */
                    t = s ;
                    /* Set ndmin = rank_tree[degree] */
                    ndmin = rank_tree[degree] ;
                }
                else {/* larger root at rank_tree[degree] */
                    /* Set t = rank_tree[degree] */
                    t = rank_tree[degree] ;
                    /* Set ndmin = s */
                    ndmin = s ;
                }

                /* Consolidate t with ndmin using t as root */
                /* WRITING node.merge() FUNCTION */

                /* Set rank_tree[degree] = NULL */
                rank_tree[degree] = NULL ;
                /* Set rank_tree[degree+1] = t */
                rank_tree[degree+1] = t ;
            }

            /* Update node s to next right sibling in root list */
            s = s->rsibling ;
        }


    }

#if 0
    /* The arbitrary remove function is not required for this exercise */

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

    /* -------------------------------------------------------------- */
    /* -------------------------- meld() ---------------------------- */
    /* -------------------------------------------------------------- */
    /* Meld two fibonacci heaps at top level. */
    void meld(FibonacciHeap fheap) {
    }
#endif
} ;
