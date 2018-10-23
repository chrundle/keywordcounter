#include "keywordcounter.h"

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
    cout << "Printing root keys, queries, degrees, and childcut " ;
    cout << "thru right siblings: " << endl ;
    cout << "Max: " << max->keyword << ", " << max->data ;
    cout << ", " << max->degree << ", " << max->childcut << endl ;

    /* Print all top level values */
    node = max->rsibling ;
    while(node != max) {
        cout << "     " << node->keyword << ", " << node->data ; 
        cout << ", " << node->degree << ", " << node->childcut << endl ;
#if 0
        std::chrono::seconds dura( 1);
        std::this_thread::sleep_for( dura );
#endif
        node = node->rsibling ;
    }
}

/* Program for determining the n most popular keywords used in search engine */
int main() {
    /* Node for checking find_max */
    Node *node = new Node() ;

    /* Initialize FibonacciHeap */
    FibonacciHeap fheap("facebook", 5) ;

    /* Insert into FibonacciHeap */
    fheap.insert("google", 20) ;

    /* Increase number of queries for facebook */
    fheap.increase_key("facebook", 2) ;

    /* Insert into FibonacciHeap */
    fheap.insert("duckduckgo", 1) ;

    /* Insert into FibonacciHeap */
    fheap.insert("pinterest", 32) ;

    /* Insert into FibonacciHeap */
    fheap.insert("dog-pictures", 12) ;

    /* Insert into FibonacciHeap */
    fheap.insert("weather", 17) ;

    /* Insert into FibonacciHeap */
    fheap.insert("edge", 3) ;

    /* Insert into FibonacciHeap */
    fheap.insert("reddit", 6) ;

    /* Insert into FibonacciHeap */
    fheap.insert("hogwarts", 10) ;

    /* Insert into FibonacciHeap */
    fheap.insert("firefox", 100) ;

    /* Print top level nodes */
    PrintKeysAndQueries(fheap) ;

    cout << endl ;
    cout << "Calling remove_max()." << endl ;

    /* Delete max */
    fheap.remove_max() ;

    /* Print top level nodes */
    PrintKeysAndQueries(fheap) ;

#if 0
    /* Increase number of queries for facebook */
    fheap.increase_key("facebook", 22) ;

    /* Print top level nodes */
    PrintKeysAndQueries(fheap) ;
#endif

    /* Increase number of queries for facebook */
    fheap.increase_key("facebook", 12) ;

    /* Print top level nodes */
    PrintKeysAndQueries(fheap) ;

    /* Increase number of queries for facebook */
    fheap.increase_key("dog-pictures", 10) ;

    /* Print top level nodes */
    PrintKeysAndQueries(fheap) ;

#if 0
    cout << endl ;
    cout << "Calling remove_max()." << endl ;

    /* Delete max */
    fheap.remove_max() ;

    /* Print top level nodes */
    PrintKeysAndQueries(fheap) ;

    cout << endl ;
    cout << "Calling remove_max()." << endl ;

    /* Delete max */
    fheap.remove_max() ;

    /* Print top level nodes */
    PrintKeysAndQueries(fheap) ;

    /* Insert into FibonacciHeap */
    fheap.insert("firefox", 2) ;

    /* Print top level nodes */
    PrintKeysAndQueries(fheap) ;

    cout << endl ;
    cout << "Calling remove_max()." << endl ;

    /* Delete max */
    fheap.remove_max() ;

    /* Print top level nodes */
    PrintKeysAndQueries(fheap) ;

    cout << endl ;
    cout << "Calling remove_max()." << endl ;

    /* Delete max */
    fheap.remove_max() ;

    /* Print top level nodes */
    PrintKeysAndQueries(fheap) ;
#endif
}

