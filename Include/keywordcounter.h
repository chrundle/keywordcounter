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
    /* -------------------------------------------------------------- */
    /* ------------------------- merge() ---------------------------- */
    /* -------------------------------------------------------------- */
    /* Void function which merges two trees of same degree, the 
       result is rooted at current node and the tree rooted at the 
       given node becomes a subtree.                                  */
    void merge(Node *nd) {
        #ifdef DBUG_PRINT
        cout << "DEBUG::merge(): In merge()" << endl ;
        #endif

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
        Node *t ;

        /* Check if degree of root is 0 */ 
        if (degree == 0) {
            #ifdef DBUG_PRINT
            cout << "DEBUG::merge(): Degree of this node = 0" << endl ;
            #endif

            /* -- Remove nd from root linked list -- */
            nd->lsibling->rsibling = nd->rsibling ;
            nd->rsibling->lsibling = nd->lsibling ;
            /* ---- Insert nd into linked list below 'this' node ---- */
            nd->parent = this ;   /* Set parent pointer of nd to this */
            this->child = nd  ;   /* Set child pointer of this to nd */
            /* -- Set sibling pointers of nd to itself -- */
            nd->rsibling = nd ; 
            nd->lsibling = nd ; 
            /* -- Update degree of root node -- */
            degree += 1 ;
        }
        else if (degree >= 1) {/* Merge one level down if degree >= 1 */
            /* -- Remove nd from root linked list -- */
            nd->lsibling->rsibling = nd->rsibling ;
            nd->rsibling->lsibling = nd->lsibling ;
            /* ---- Insert nd into linked list below 'this' node ---- */
            nd->parent = this ;   /* Set parent pointer of nd to this */
            /* -- Insert nd to right of child of 'this' -- */
            t = this->child ;   /* Set t to child of this node */
            t->rsibling->lsibling = nd ; /* Insert nd to right of t */
            nd->rsibling = t->rsibling ; /* Insert nd to right of t */
            nd->lsibling = t ; /* Insert nd to right of t */
            t->rsibling = nd ; /* Insert nd to right of t */
            /* -- Update degree of root node -- */
            degree += 1 ;
        }
        else {/* Degree was somehow set to negative which is illegal */
            /* Print error message */
            cout << "NODE::merge(): degree of root negative, must be >=0. " ;
            cout << "Cannot perform merge(). Exiting function. " << endl ;
        }

        #ifdef DBUG_PRINT
        cout << "DEBUG::merge(): Program completed successfully." << endl ;
        #endif
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

    /* -------------------------------------------------------------- */
    /* ----------------------- PrintRoot() -------------------------- */
    /* -------------------------------------------------------------- */
    /* Compute bound on rank based on current number of nodes.        */       
    void PrintRoot() {
        /* Node for checking find_max */
        Node *node = new Node() ;
    
        /* Print max node keyword and query */
        cout << endl ;
        cout << "Total number of nodes: " << n << endl ;
        cout << "Printing all root nodes passing through right siblings: " << endl ;
        cout << "Max: " << max->keyword << ", " << max->data << endl ;
    
        /* Print all top level values */
        node = max->rsibling ;
        while(node != max) {
            cout << "     " << node->keyword << ", " << node->data << std::endl ; 
            node = node->rsibling ;
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
                /* Set s to t's right sibling */
                s = t->rsibling ;
    
                /* -- Meld of right endpoint -- */
                /* Set left sibling of max's right sibling to t */
                max->rsibling->lsibling = t ;
                /* Set right sibling of t to right sibling of max node */
                t->rsibling = max->rsibling ;
    
                /* -- Meld of left endpoint -- */
                /* Set right sibling of max's left sibling to s */
                max->lsibling->rsibling = s ;
                /* Set left sibling of s to left sibling of max node */
                s->rsibling = max->rsibling ;

                /* Set max pointer to any sibling of current max */
                max = t ;
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
        Node *rank_tree[k] ;

        #ifdef DBUG_PRINT
        cout << "DEBUG::remove_max(): rank_bound = " << k << endl ;
        #endif

        /* Set all pointers in rank_tree array to null */
        for (i = 0; i < k; i++) {
            rank_tree[i] = NULL ;
        }
        /* -- Move through all roots and merge trees of same rank -- */
        /* Add degree of max node to rank tree */
        rank_tree[max->degree] = max ;
        /* Use s to keep track of current node */
        s = max->rsibling ;

        #ifdef DBUG_PRINT
        cout << "DEBUG::remove_max(): Move through rank_tree" << endl ;
        #endif

        /* Move through roots in node and merge trees of same rank */
        while (s != max) {
            /* Set current degree */
            degree = s->degree ;

            #ifdef DBUG_PRINT
            cout << "DEBUG::remove_max(): Current degree = " << degree << endl ;
            #endif

            /* Check if degree of current node is not in rank_tree */
            if (rank_tree[degree] == NULL) {/* degree of s not in rank_tree */
                /* Add degree of s to rank tree */
                rank_tree[degree] = s ;

                #ifdef DBUG_PRINT
                cout << "DEBUG::remove_max(): Added pointer to rank_tree[" << degree << "]" << endl ;
                #endif
            }
            else {/* degree of s already in rank_tree */
                while (rank_tree[degree] != NULL) {
                    /* -- consolidate s with rank_tree[degree] -- */
                    /* Compare root values to determine largest */
                    if (rank_tree[degree]->data < s->data) {/* larger root at s */
                        /* Set ndmin = rank_tree[degree] */
                        ndmin = rank_tree[degree] ;
                    }
                    else {/* larger root at rank_tree[degree] */
                        /* Set ndmin = s */
                        ndmin = s ;
                        /* Set s = rank_tree[degree] */
                        s = rank_tree[degree] ;
                    }
    
                    #ifdef DBUG_PRINT
                    cout << "DEBUG::remove_max(): Keyword at top merge node: " ;
                    cout << s->keyword << endl ;
                    cout << "DEBUG::remove_max(): Keyword at bottom merge node: " ;
                    cout << ndmin->keyword << endl ;
                    #endif
    
                    /* Consolidate t with ndmin using t as root */
                    s->merge(ndmin) ;
    
                    #ifdef DBUG_PRINT
                    cout << "DEBUG::remove_max(): Root after merge" << endl ;
                    PrintRoot() ;
                    #endif
    
                    /* Update pointers in rank_tree as ranks have changed */
                    /* Set rank_tree[degree] = NULL */
                    rank_tree[degree] = NULL ;

                    #ifdef DBUG_PRINT
                    cout << "DEBUG::remove_max(): Updated rank_tree[" ;
                    cout << degree << "] to NULL" << endl ;
                    #endif
                   
                    /* Increase degree by one  */
                    degree += 1 ;
                }

                /* Set rank_tree[degree] = s */
                rank_tree[degree] = s ;
                #ifdef DBUG_PRINT
                cout << "DEBUG::remove_max(): Updated rank_tree[" ;
                cout << degree << "] to " << s->keyword << endl ;
                #endif
            }

            /* Update node s to next right sibling in root list */
            s = s->rsibling ;
        }

        /* ---- Finished merging trees of same rank. Exit program ---- */
        #ifdef DBUG_PRINT
        cout << "DEBUG::remove_max(): Exiting program" << endl ;
        #endif
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
