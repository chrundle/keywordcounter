#include "keywordcounter.h"
#include <thread>
#include <chrono>

/* Test function to print all top level keywords and queries for unit tests */
void PrintKeysAndQueries(FibonacciHeap fheap) {
    /* Node for checking find_max */
    Node *node = new Node() ;
    Node *max = new Node() ;

    /* Find maximum value in heap */
    max = fheap.find_max() ;

    /* Print max node keyword and query */
    cout << endl ;
    cout << "Total number of nodes: " << fheap.number_of_nodes() << endl ;
    cout << "Printing all root nodes passing through right siblings: " << endl ;
    cout << "Max: " << max->keyword << ", " << max->data << endl ;

    /* Print all top level values */
    node = max->rsibling ;
    while(node != max) {
        cout << "     " << node->keyword << ", " << node->data << std::endl ; 
        node = node->rsibling ;
    }
}

/* Program for determining the n most popular keywords used in search engine */
int main() {
    /* Node for checking find_max */
    Node *node = new Node() ;

    /* Initialize FibonacciHeap */
    FibonacciHeap fheap("facebook", 5) ;

    /* Print top level nodes */
    PrintKeysAndQueries(fheap) ;

    /* Insert into FibonacciHeap */
    fheap.insert("google", 20) ;

    /* Print top level nodes */
    PrintKeysAndQueries(fheap) ;

    /* Insert into FibonacciHeap */
    fheap.insert("duckduckgo", 1) ;

    /* Print top level nodes */
    PrintKeysAndQueries(fheap) ;

    /* Insert into FibonacciHeap */
    fheap.insert("edge", 3) ;

    /* Print top level nodes */
    PrintKeysAndQueries(fheap) ;

    /* Insert into FibonacciHeap */
    fheap.insert("firefox", 100) ;

    /* Print top level nodes */
    PrintKeysAndQueries(fheap) ;

    /* Delete max */
    fheap.remove_max() ;

    /* Print top level nodes */
    PrintKeysAndQueries(fheap) ;

    /* Delete max */
    fheap.remove_max() ;

    /* Print top level nodes */
    PrintKeysAndQueries(fheap) ;

    /* Delete max */
    fheap.remove_max() ;

    /* Print top level nodes */
    PrintKeysAndQueries(fheap) ;

    /* Insert into FibonacciHeap */
    fheap.insert("firefox", 2) ;

    /* Print top level nodes */
    PrintKeysAndQueries(fheap) ;

    /* Delete max */
    fheap.remove_max() ;

    /* Print top level nodes */
    PrintKeysAndQueries(fheap) ;

    /* Delete max */
    fheap.remove_max() ;

    /* Print top level nodes */
    PrintKeysAndQueries(fheap) ;
}

