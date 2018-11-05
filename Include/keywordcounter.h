#include <math.h>
#include <climits>
#include <iostream>
#include <string.h>
#include <unordered_map>

#include <thread>
#include <chrono>

#define FALSE 0
#define TRUE 1

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
        /* Set childcut to FALSE */
        childcut = FALSE ;
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

        #ifdef DBUG_PRINT
        cout << "DEBUG::merge(): Degree at beginning of merge() = " ;
        cout << degree << endl ;
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
        cout << "DEBUG::merge(): Degree at end of merge() = " ;
        cout << degree << endl ;
        #endif

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
    /* -------------------------- meld() ---------------------------- */
    /* -------------------------------------------------------------- */
    /* Meld single node at root level.                                */
    void meld(Node *nd) {
        /* ---- Insert nd into linked list with max node ---- */
#if 0
    /* This operation is done in remove() */
        nd->parent = NULL ;   /* Set parent pointer of nd to NULL */
#endif
        /* -- Insert nd to right of max node -- */
        max->rsibling->lsibling = nd ; /* Insert nd to right of max */
        nd->rsibling = max->rsibling ; /* Insert nd to right of max */
        nd->lsibling = max ; /* Insert nd to right of max */
        max->rsibling = nd ; /* Insert nd to right of max */
        /* -- Set childcut to FALSE for given node as it is now in root -- */
        nd->childcut = FALSE ; 
        /* -- Check if max pointer needs to be updated -- */
        if(max->data < nd->data) {
            /* Set max pointer to pointer of new node */
            max = nd ;
        }
    }

    /* -------------------------------------------------------------- */
    /* ------------------------ meld_list() ------------------------- */
    /* -------------------------------------------------------------- */
    /* Meld list of nodes at root level.                              */
    void meld_list(Node *nd) {
        /* ---- Insert nd into linked list with max node ---- */
#if 0
    /* This operation is done in remove() */
        nd->parent = NULL ;   /* Set parent pointer of nd to NULL */
#endif
        /* -- Insert nd to right of max node -- */
        max->rsibling->lsibling = nd->lsibling ; /* Set nd lsib right of max */
        nd->lsibling->rsibling = max->rsibling ; /* Set nd lsib right of max */
        nd->lsibling = max ; /* Insert nd to right of max */
        max->rsibling = nd ; /* Insert nd to right of max */
        /* -- Set childcut to FALSE for given node as it is now in root -- */
        nd->childcut = FALSE ; 
            /* TODO: NEED TO SET ALL childcut VALUES TO FALSE FROM MELDED HEAP */
        /* -- Check if max pointer needs to be updated -- */
        if(max->data < nd->data) {
            /* Set max pointer to pointer of new node */
            max = nd ;
        }
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
        cout << "Printing root keys, queries, degrees, childcut, and parent " ;
        cout << "thru right siblings: " << endl ;
        cout << "Max: " << max->keyword << ", " << max->data ;
        cout << ", " << max->degree << ", " << max->childcut ;
        if (max->parent == NULL) {
            cout << ", No parent" << endl ;
        }
        else {
            cout << ", " << max->parent->keyword << endl ;
        }
    
        /* Print all top level values */
        node = max->rsibling ;
        while(node != max) {
            cout << "     " << node->keyword << ", " << node->data ;
            cout << ", " << node->degree << ", " << node->childcut ;
            if (node->parent == NULL) {
                cout << ", No parent" << endl ;
            }
            else {
                cout << ", " << node->parent->keyword << endl ;
            }
#if 0
            std::chrono::seconds dura( 1);
            std::this_thread::sleep_for( dura );
#endif
            node = node->rsibling ;
        }
    }

  /* -------------------- Public members of heap -------------------- */
  public:
    unordered_map<string, Node*> hashmap ; /* Hash table for keywords
                                              mapped to pointers to 
                                              nodes                   */

    /* -------------------------------------------------------------- */
    /* -------------------- Default Constructor --------------------- */
    /* -------------------------------------------------------------- */
    FibonacciHeap() {
        /* Set max pointer to NULL */
        max = NULL ;
        /* Set number of nodes in heap to 0 */
        n = 0 ;
    }

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
        #ifdef DBUG_PRINT
        cout << "DEBUG::insert(): In insert()." << endl ;
        #endif

        /* ---- Check if keyword is already in heap ---- */
        if (hashmap[keyword] != NULL) {/* keyword in heap */
            #ifdef DBUG_PRINT
            cout << "DEBUG::insert(): Given keyword already in heap." << endl ;
            #endif

            /* Call increase_key instead */
            increase_key(keyword, frequency) ;

            /* Exit program */
            return ;
        }

        #ifdef DBUG_PRINT
        cout << "DEBUG::insert(): Given keyword not in heap." << endl ;
        #endif

        /* ---- If keyword is not in heap add it to heap ---- */
        /* Create new node with data equal to number of queries */
        hashmap[keyword] = new Node(keyword, frequency) ;
        /* Check if heap already has nodes in it */
        if(n > 0) {/* Heap has at least one node */
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
        }
        else {
            #ifdef DBUG_PRINT
            cout << "DEBUG::insert(): No nodes in heap." ;
            cout << "Setting max to new pointer." << endl ;
            #endif
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

        /* Check if number of nodes in heap is zero */
        if (n == 0) {/* No nodes in heap */
            /* Print error message */
            cout << "remove_max(): Given heap has zero elements." << endl ;
            /* Exit program */
            return ;
        }

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
                /* Set t to child of max */
                t = max->child ;
                /* Set parent of t to NULL */
                t->parent = NULL ;
                /* Set s to t's right sibling */
                s = t->rsibling ;

                /* ---- Change parent pointers of all children to NULL ---- */
                while (s != t) {
                    /* Set parent of s to NULL */
                    s->parent = NULL ;
                    /* Set s to s's right sibling */
                    s = s->rsibling ;
                }

                /* Erase max element from hashmap */
                hashmap.erase(max->keyword) ;

                /* Set max pointer to any child of current max */
                max = t ;
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

                /* Erase max element from hashmap */
                hashmap.erase(max->keyword) ;

                /* Set max pointer to any sibling of current max */
                max = t ;
            }
            else {/* max has at least one child */
                #ifdef DBUG_PRINT
                cout << "DEBUG::remove_max(): max has at least one child" << endl ;
                #endif

                /* Set t to child of max */
                t = max->child ;
                /* Set parent of t to NULL */
                t->parent = NULL ;
                /* Set s to t's right sibling */
                s = t->rsibling ;

                /* ---- Change parent pointers of all children to NULL ---- */
                while (s != t) {
                    /* Set parent of s to NULL */
                    s->parent = NULL ;
                    /* Set s to s's right sibling */
                    s = s->rsibling ;
                }

                /* ---- Add children to root linked list ---- */
#if 0
                /* Set t to child of max */
                t = max->child ;
#endif
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
                s->lsibling = max->lsibling ;

                /* Erase max element from hashmap */
                hashmap.erase(max->keyword) ;

                /* Set max pointer to any child of current max */
                max = t ;
            }
        }
        /* Decrease total number of nodes by 1 since max has been removed */
        n -= 1 ;

        /* ------ Step 2: Determine new max ------ */
        #ifdef DBUG_PRINT
        cout << "DEBUG::remove_max(): Determine new max" << endl ;
        #endif

        /* Check if number of nodes in heap is now zero */
        if (n == 0) {/* No nodes left in heap */
            /* Set max equal to NULL pointer */
            max = NULL ;
            /* Exit program */
            return ;
        }

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

        #ifdef DBUG_PRINT
        cout << endl << "DEBUG::remove_max(): Root after removing max" ;
        PrintRoot() ;
        #endif

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

    /* -------------------------------------------------------------- */
    /* ------------------------- remove() --------------------------- */
    /* -------------------------------------------------------------- */
    /* Remove node from heap given pointer to node. Updates childcut 
       of input parent node.
       Returns FALSE if childcut was not changed from TRUE
               TRUE  if childcut was changed from TRUE                */
    /* This function is required for the increase_key() function.     */
    bool remove(Node *nd) {
        /* Initialize variables */
        bool output ;
        Node *par ;

        /* Check if node contains maximum key */
        if (nd == max) {/* Node to remove contains max key */
            /* Perform remove max */
            remove_max() ;
            /* Exit function */
            return FALSE ;
        }
        else {/* Node to remove does not contain max key */
            /* ---- Insert nd into linked list with 'this' node ---- */
            par = nd->parent ;    /* Store parent pointer of nd */
            nd->parent = NULL ;   /* Set parent pointer of nd to NULL */
#if 0
    /* This operation is performed in meld() */
            /* -- Set childcut to FALSE for given node as it is now in root -- */
            nd->childcut = FALSE ; 
#endif
            /* -- Decrease value of degree for parent node by 1 -- */
            par->degree -= 1 ;

            /* -- Update child pointer for par node -- */
            /* Check if nd is child of nd parent */
            if (par->child == nd) {/* nd == nd->parent->child */
                /* Check if nd has siblings */
                if (nd->rsibling == nd) {/* nd has no siblings */
                    /* Set child pointer to NULL */
                    par->child = NULL ;
                }
                else {/* nd has at least one sibling */
                    /* Set child pointer to right sibling of nd */
                    par->child = nd->rsibling ;
                }
            }
            else {/* nd != nd->parent->child */
                /* Nothing to be done */
            }

            /* -- Update value of childcut for 'this' node -- */
            /* Store output value */
            output = par->childcut ;
            /* Check if 'this' node is not at root level */
            if (par->parent != NULL) {/* Parent node is not at root level */
                /* Flip boolean value of childcut */
                par->childcut = !output ; 
            }
            /* Return output */
            return output ;
        }
    }

    /* -------------------------------------------------------------- */
    /* ---------------------- increase_key() ------------------------ */
    /* -------------------------------------------------------------- */
    /* Given a node, increase its key value by a given amount. */
    void increase_key(string keyword, long int amount) {
        /* Initialize variables */
        Node *t, *s, *nd ;
        bool childcut ;

        #ifdef DBUG_PRINT
        cout << "DEBUG::increase_key(): In increase_key()" << endl ;
        #endif

        /* Initialize nd */
        nd = hashmap[keyword] ;

        /* Check if given node is orphan and store output in t */
        t = orphan(nd) ;

        /* Increase nd key value by given amount */
        nd->data += amount ;

        #ifdef DBUG_PRINT
        cout << "DEBUG::increase_key(): " << nd->keyword ;
        cout << "->data = " << nd->data << endl ;
        #endif

        if (t == NULL) {/* Given node is at root level */
            #ifdef DBUG_PRINT
            cout << "DEBUG::increase_key(): " << nd->keyword ;
            cout << " has no parent" << endl ;
            #endif

            /* Check if max pointer needs to be updated */
            if(max->data < nd->data) {
                /* Set max pointer to pointer of new node */
                max = nd ;
            }
            /* Exit program */
            return ;
        }
        else {/* Given node is not at root level */
            #ifdef DBUG_PRINT
            cout << "DEBUG::increase_key(): " << nd->keyword ;
            cout << " has parent " << nd->parent->keyword << endl ;
            #endif

            /* Check if nd data is less than parent data */
            if(nd->data < nd->parent->data) {
                /* Nothing else to be done. Exit program. */
                return ;
            }

            /* Set s to nd (necessary for beginning of while loop) */
            s = nd ;
            /* Initialize childcut to TRUE (necessary to enter while loop) */
            childcut = TRUE ;
            /* Move up subtree from nd until childcut is FALSE */
            while (childcut) {
                /* Set t to s */
                t = s ;
                /* Set s to t->parent */
                s = t->parent ;
                /* Remove node nd from fibonacci heap */
                childcut = remove(t) ;
                /* Insert subtree with nd at root level */
                meld(t) ;
            }
        }
    }

    /* -------------------------------------------------------------- */
    /* ------------------------- combine() -------------------------- */
    /* -------------------------------------------------------------- */
    /* Given a fib heap, combine its top level with this fib heap. */
    void combine(Node *fheap, long int size) {
        /* Meld root level of given heap to current heap */
        this->meld_list(fheap) ;
        /* Increase number of elements in current heap */
        n += size ;
    }
} ;
