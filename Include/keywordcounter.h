/* Node struct used for nodes in Fibonacci heap implementation */
struct Node {
    /* -------- Integer values -------- */
    int     degree ; /* Degree of node */
    int       data ; /* Data in node */

    /* -------- Boolean values -------- */
    bool  childcut ; /* TRUE  => Node has lost child since it became
                                 child of its current parent 
                        FALSE => Set F by remove min, which is the only 
                                 operation that makes one node child of
                                 another
                        Note: Undefined for a root node              */
    /* -------- Node pointers -------- */
    Node    *child ; /* Child pointer */
    Node *lsibling ; /* Left sibling pointer */
    Node *rsibling ; /* Right sibling pointer */
    Node   *parent ; /* Paretn pointer */
} ;
