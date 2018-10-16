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
        lsibling = rsibling ;
        rsibling = lsibling ;
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
    /* Find and return maximum value in heap */
    Node *find_max() {
        return max ;
    }

    /* -------------------------------------------------------------- */
    /* ------------------------- insert() --------------------------- */
    /* -------------------------------------------------------------- */
    /* Insert node into heap */
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
} ;
