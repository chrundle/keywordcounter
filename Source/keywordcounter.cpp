#include "keywordcounter.h"
#include <fstream>
#include <queue>
#include <utility>
#include <map>


/* Test function to print all top level keywords and queries for unit tests */
void PrintKeysAndQueriesLtoR(FibonacciHeap *fheap) {
    /* Node for checking find_max */
    Node *node = new Node() ;
    Node *max = new Node() ;

    /* Find maximum value in heap */
    max = fheap->find_max() ;

    /* Print max node keyword and query */
    cout << endl ;
    cout << "Total number of nodes: " << fheap->number_of_nodes() << endl ;
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
void PrintKeysAndQueriesRtoL(FibonacciHeap *fheap) {
    /* Node for checking find_max */
    Node *node = new Node() ;
    Node *max = new Node() ;

    /* Find maximum value in heap */
    max = fheap->find_max() ;

    /* Print max node keyword and query */
    cout << endl ;
    cout << "Total number of nodes: " << fheap->number_of_nodes() << endl ;
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
    long int frequency, query, data, tmp_data ;
    queue < pair<string, long int> > removed_pairs ;
    pair <string, long int> key_query_pair ;
    ifstream qfile ;
    ofstream outfile ;
    string line, keyword ;
    Node *max ;

    #ifdef GENERATE_SOLUTION
    /* Initialize map to store keyword, query pairs in order */
    map<long int, string> GS_sol_map ;
    map<long int, string>::reverse_iterator GS_rit ;
    size_t GS_position, GS_str_len ;
    Node *GS_node ;
    long int GS_data ;
    int GS_query_num ;

    /* Initialize number of queries to zero */
    GS_query_num = 0 ;
    #endif

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
    FibonacciHeap fheap ;

    /* Get first line from qfile */
    getline (qfile, line) ;

    /* Pass through input file extracting current line at each pass */
    while (line.compare("STOP")) {/* Compare current line to stop command */
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

            #ifdef GENERATE_SOLUTION
                #ifdef DEBUG_GENERATE_SOLUTION
                /* Print current keyword and frequency */
                cout << "main::GS: Current keyword, frequency: " ;
                cout << keyword << ", " << frequency << endl ;
                #endif

            /* Set node pointer */
            GS_node = fheap.hashmap[keyword] ;
            /* Check if keyword already in map */
            if (GS_node != NULL) {/* keyword in map */
                    #ifdef DEBUG_GENERATE_SOLUTION
                    /* Print current keyword already in map */
                    cout << "  main::GS: keyword = " ;
                    cout << keyword << " already in map" << endl ;
                    #endif

                /* Set current frequency of keyword in fheap */
                GS_data = GS_node->data ;
                    #ifdef DEBUG_GENERATE_SOLUTION
                    /* Print current keyword already in map */
                    cout << "    main::GS: keyword " ;
                    cout << keyword << " has frequency " << GS_data  << endl ;
                    #endif

                /* Determine position and length of string */
                GS_position = GS_sol_map[GS_data].find(keyword) ;
                GS_str_len = keyword.length() + 1 ;

                    #ifdef DEBUG_GENERATE_SOLUTION
                    /* Print position and length of keyword */
                    cout << "    main::GS: GS_position = " ;
                    cout << GS_position << endl ;
                    cout << "    main::GS: GS_str_len = " ;
                    cout << GS_str_len << endl ;
                    #endif

                /* Remove string from map with current frequency */
                if (GS_position != string::npos){
                    GS_sol_map[GS_data].erase(GS_position,GS_str_len) ;
                }
#if 0
                /* Check if GS_position indicates string was found */
                if (GS_position != std::string::npos) {/* String was found */
                    /* Remove string from map with current frequency */
                    GS_sol_map[GS_data].erase(GS_position,GS_str_len) ;
                }
#endif

                /* If string is empty remove from map */
                if (GS_sol_map[GS_data].empty()) {/* string is empty */
                        #ifdef DEBUG_GENERATE_SOLUTION
                        /* Print keyword to be removed from string in map */
                        cout << "    main::GS: Removing " << keyword ;
                        cout << " from string" << endl ;
                        #endif

                    /* Remove element from map that has empty string */
                    GS_sol_map.erase(GS_data) ;
                }
                /* Update value in map */
                GS_sol_map[frequency + GS_data] = keyword + " " + GS_sol_map[frequency + GS_data] ;
                    #ifdef DEBUG_GENERATE_SOLUTION
                    /* Print updated position of keyword in map */
                    cout << "    main::GS: GS_sol_map[" ;
                    cout << frequency + GS_data << "] = " ;
                    cout << GS_sol_map[frequency + GS_data] << endl ;
                    #endif
            }
            else {
                /* Update keyword in map */
                GS_sol_map[frequency] = keyword + " " + GS_sol_map[frequency] ;

                    #ifdef DEBUG_GENERATE_SOLUTION
                    /* Print updated position of keyword in map */
                    cout << "    main::GS: GS_sol_map[" << frequency << "] = " ;
                    cout << GS_sol_map[frequency] << endl ;
                    #endif
            }
            #endif

            /* Update keyword and frequency in heap */
            fheap.insert(keyword, frequency) ;
        }
        else {/* Received query request */
            /* Extract query request from line and convert to int */
            query = stoi(line) ;

            #ifdef DEBUG_MAIN
            /* Print notice that query request was received */
            cout << "main::DEBUG: Query request received: " << query << endl ;
            #endif

            /* -- Remove requested number of elements from fheap and print -- */
            while (query > 1) {
                /* Find maximum value in heap */
                max = fheap.find_max() ;
                /* Set keyword to current keyword */
                keyword = max->keyword ;
                /* Set keyword to current keyword */
                tmp_data = max->data ;
                /* Remove maximum value from heap */
                fheap.remove_max() ;
                /* Print 'keyword,' to output file */
                outfile << keyword << "," ;
                /* Insert keyword and query into FIFO queue */
                removed_pairs.push(make_pair(keyword, tmp_data)) ;
                /* Decrease query by one */
                query-- ;
            }

            /* Repeat operations for final query */
            /* Find maximum value in heap */
            max = fheap.find_max() ;
            /* Set keyword to current keyword */
            keyword = max->keyword ;
            /* Set keyword to current keyword */
            tmp_data = max->data ;
            /* Remove maximum value from heap */
            fheap.remove_max() ;
            /* Print 'keyword,' to output file */
            outfile << keyword << endl ;
            /* Insert keyword and query into FIFO queue */
            removed_pairs.push(make_pair(keyword, tmp_data)) ;

            #ifdef DEBUG_MAIN
            /* Print Keys and Queries */
            PrintKeysAndQueriesLtoR(&fheap) ;
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

            #ifdef GENERATE_SOLUTION
            /* Print current ordered list of keywords and frequencies and
               increase query counter by one */
            cout << endl ;
            cout << "Ordered list of keywords and frequencies at query " ;
            cout << ++GS_query_num << ": " << endl ;
            for (GS_rit = GS_sol_map.rbegin(); GS_rit != GS_sol_map.rend(); ++GS_rit) {
                cout << "  " << GS_rit->second << ", " << GS_rit->first << endl ;
            }
            #endif
        }

        #ifdef DEBUG_MAIN
        /* Print Keys and Queries */
        PrintKeysAndQueriesLtoR(&fheap) ;
        #endif

        /* If parent of max is not NULL an error has occurred */
        if (fheap.hashmap["woollier"] != NULL) {
            if (fheap.hashmap["woollier"]->parent != NULL) {
                cout << "main::DEBUG: woollier has keyword: " << fheap.hashmap["woollier"]->keyword << endl ; 
                cout << "main::DEBUG: woollier has parent: " << fheap.hashmap["woollier"]->parent->keyword << endl ; 
            }
        }

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

