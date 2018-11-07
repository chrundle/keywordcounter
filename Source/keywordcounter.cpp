#include "keywordcounter.h"
#include <fstream>
#include <queue>
#include <utility>


/* Test function to print all top level keywords and queries for unit tests */
void PrintKeysAndQueriesLtoR(FibonacciHeap fheap) {
    /* Node for checking find_max */
    Node *node = new Node() ;
    Node *max = new Node() ;

    /* Find maximum value in heap */
    max = fheap.find_max() ;

    /* Print max node keyword and query */
    cout << endl ;
    cout << "Total number of nodes: " << fheap.number_of_nodes() << endl ;
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

/* Test function to print all top level keywords and queries for unit tests */
void PrintKeysAndQueriesRtoL(FibonacciHeap fheap) {
    /* Node for checking find_max */
    Node *node = new Node() ;
    Node *max = new Node() ;

    /* Find maximum value in heap */
    max = fheap.find_max() ;

    /* Print max node keyword and query */
    cout << endl ;
    cout << "Total number of nodes: " << fheap.number_of_nodes() << endl ;
    cout << "Printing root keys, queries, degrees, childcut, and parent " ;
    cout << "thru left siblings: " << endl ;
    cout << "Max: " << max->keyword << ", " << max->data ;
    cout << ", " << max->degree << ", " << max->childcut ;
    if (max->parent == NULL) {
        cout << ", No parent" << endl ;
    }
    else {
        cout << ", " << max->parent->keyword << endl ;
    }

    /* Print all top level values */
    node = max->lsibling ;
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
        node = node->lsibling ;
    }
}

/* Program for determining the n most popular keywords used in search engine */
int main(int argc, char *argv[]) {
    /* Initialize variables */
    int exit_status, space_num ;
    long int frequency, query, data ;
    queue < pair<string, long int> > removed_pairs ;
    pair <string, long int> key_query_pair ;
    ifstream qfile ;
    ofstream outfile ;
    string line, keyword ;
    Node *max = new Node() ;

    /* Check if number of input arguments is correct */
    if (argc != 2) {
        /* Program requires file of queries to run */
        cout << "keywordcounter::Error: Problem file containing queries " ;
        cout << "not provided. Terminating program." << endl ;
        /* Set exit status to 1 */
        exit_status = 1 ;
        /* Exit program */
        return exit_status ;
    }

    /* Initialize exit_status to 0 (indicating no errors) */
    exit_status = 0 ;

    #ifdef DEBUG_MAIN
    /* Print name of input file */
    cout << "main::DEBUG: Input file provided: " << argv[1] << endl ;
    #endif

    /* Open input file */
    qfile.open (argv[1]) ;

    /* Check if file was unable to be opened */
    if (!qfile.is_open()) {/* File unable to be opened succefully */
        /* Set exit status to 2 */
        exit_status = 2 ;
        /* Exit program */
        return exit_status ;
    }
    #ifdef DEBUG_MAIN
    cout << "main::DEBUG: Input file successfully opened." << endl ;
    #endif

    /* Open output file */
    outfile.open ("output_file.txt") ;

    /* Check if file was unable to be opened */
    if (!outfile.is_open()) {/* File unable to be opened succefully */
        /* Set exit status to 2 */
        exit_status = 2 ;
        /* Exit program */
        return exit_status ;
    }
    #ifdef DEBUG_MAIN
    cout << "main::DEBUG: Output file successfully opened." << endl ;
    #endif

    /* Initialize main FibonacciHeap and backup */
    FibonacciHeap fheap, temp_heap ;

    /* Get first line from qfile */
    getline (qfile, line) ;

    /* Pass through input file extracting current line at each pass */
    while (line.compare("stop")) {/* Compore current line to stop command */
        #ifdef DEBUG_MAIN
        /* Print current line in input file */
        cout << "main::DEBUG: Current line in file: " << line << endl ;
        #endif

        /* Check if current line contains update or request query */
        if (line.substr(0,1) == "$") {/* Received keyword and frequency */
            /* Identify position of " " in line */
            space_num = line.find(' ') ;

            /* Extract keyword from line */
            keyword = line.substr(1, space_num - 1) ;
            /* Extract frequency from line and convert to int */
            frequency = stoi(line.substr(space_num + 1)) ;

            #ifdef DEBUG_MAIN
            /* Print current keyword and frequency */
            cout << "main::DEBUG: Current keyword, frequency: " ;
            cout << keyword << ", " << frequency << endl ;
            #endif

            /* Update keyword and frequency in heap */
            fheap.insert(keyword, frequency) ;
        }
        else {/* Received query request */
            /* Extract query request from line and convert to int */
            query = stoi(line) ;

            #ifdef DEBUG_MAIN
            /* Print current query number */
            cout << "main::DEBUG: Current query: " << query << endl ;
            #endif

            /* -- Remove requested number of elements from fheap and print -- */
            while (query > 1) {
                /* Find maximum value in heap */
                max = fheap.find_max() ;
                /* Remove maximum value from heap */
                fheap.remove_max() ;
                /* Set keyword to current keyword */
                keyword = max->keyword ;
                /* Print 'keyword,' to output file */
                outfile << keyword << "," ;
                /* Insert keyword and query into FIFO queue */
                removed_pairs.push(make_pair(keyword, max->data)) ;
                /* Decrease query by one */
                query-- ;
            }

            /* Repeat operations for final query */
            /* Find maximum value in heap */
            max = fheap.find_max() ;
            /* Remove maximum value from heap */
            fheap.remove_max() ;
            /* Set keyword to current keyword */
            keyword = max->keyword ;
            /* Print 'keyword\n' to output file */
            outfile << keyword << endl ;
            /* Insert keyword and query into FIFO queue */
            removed_pairs.push(make_pair(keyword, max->data)) ;

            #ifdef DEBUG_MAIN
            /* Print Keys and Queries */
            PrintKeysAndQueriesLtoR(fheap) ;
            #endif
             
            /* -- Insert removed elements back into fheap -- */
            while (!removed_pairs.empty()) {
                /* Extract keyword,query pair from FIFO queue */
                key_query_pair = removed_pairs.front() ;

                #ifdef DEBUG_MAIN
                /* Print pair to be reinserted */
                cout << "main::DEBUG: Reinserting keyword, query pair: " ;
		cout << key_query_pair.first << ", " ;
                cout << key_query_pair.second << endl ;
                #endif

                /* Insert element into fheap */
                fheap.insert(key_query_pair.first, key_query_pair.second) ;
                /* Pop keyword,query pair from FIFO queue */
                removed_pairs.pop() ;
            }
        }

        #ifdef DEBUG_MAIN
        /* Print Keys and Queries */
        PrintKeysAndQueriesLtoR(fheap) ;
        #endif

        /* Get next line and break if no next line */
        if (!getline (qfile, line)) {/* Error occured in getline */
            cout << "main::DEBUG: Error occured using getline." << endl ;
            break ;
        }
    }

    #ifdef DEBUG_MAIN
    /* Print to console that stop command was received */
    cout << "main::DEBUG: Received 'stop' command." << endl ;
    #endif

    /* Close files */
    qfile.close() ;
    outfile.close() ;

    /* Exit program and return exit status */
    return exit_status ;
}

