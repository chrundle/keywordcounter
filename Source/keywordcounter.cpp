#include "keywordcounter.h"
#include <fstream>

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
int main(int argc, char *argv[]) {
    /* Initialize variables */
    int exit_status, space_num ;
    long int frequency, query ;
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

    /* Initialize FibonacciHeap */
#if 0
    FibonacciHeap fheap("facebook", 5) ;
#endif

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
            #endif
            cout << "main::DEBUG: Current keyword, frequency: " ;
            cout << keyword << ", " << frequency << endl ;

#if 0
            /* Update keyword and frequency in heap */
            fheap.insert(keyword, frequency) ;
#endif
        }
        else {/* Received query request */
            /* Extract query request from line and convert to int */
            query = stoi(line) ;

            #ifdef DEBUG_MAIN
            /* Print current query number */
            #endif
            cout << "main::DEBUG: Current query: " << query << endl ;

#if 0
            /* -- Remove requested number of elements from fheap and print -- */
            while (query > 1) {
                /* Find maximum value in heap */
                max = fheap.find_max() ;
                /* Print 'keyword,' to output file */
                outfile << max->keyword << "," ;
                /* Meld node to separate fibonacci heap */
             
                /* Decrease query by one */
                query-- ;
            }

            /* Repeat operations for final query */
            /* Find maximum value in heap */
            max = fheap.find_max() ;
            /* Print 'keyword' to output file and endline */
            outfile << max->keyword << endl ;
            /* Meld node to separate fibonacci heap */
             

            /* -- Insert removed elements back into fheap -- */
            /* Merge separate fibonacci heap with fheap */
#endif

        }

        /* Get next line and break if no next line */
        if (!getline (qfile, line)) {/* Error occured in getline */
            cout << "main::DEBUG: Error occured using getline." << endl ;
            break ;
        }
    }


    #ifdef DEBUG_MAIN
    /* Print current line in input file */
    cout << "main::DEBUG: Received 'stop' command." << endl ;
    cout << "main::DEBUG: Current line in file: " << line << endl ;
    #endif

#if 0
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

    /* Increase number of queries for facebook */
    fheap.increase_key("facebook", 12) ;

    /* Print top level nodes */
    PrintKeysAndQueries(fheap) ;

    /* Increase number of queries for facebook */
    fheap.increase_key("dog-pictures", 10) ;

    /* Print top level nodes */
    PrintKeysAndQueries(fheap) ;

    /* Close file and free file pointer */
    fclose(fp) ;
#endif

    /* Close files */
    qfile.close() ;
    outfile.close() ;

    /* Exit program and return exit status */
    return exit_status ;
}

