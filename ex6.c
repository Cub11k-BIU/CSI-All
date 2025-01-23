#include "ex6.h"

// ================================================
// Basic struct definitions from ex6.h assumed:
//   PokemonData { int id; char *name; PokemonType TYPE; int hp; int attack; EvolutionStatus CAN_EVOLVE; }
//   PokemonNode { PokemonData* data; PokemonNode* left, *right; }
//   OwnerNode   { char* ownerName; PokemonNode* pokedexRoot; OwnerNode *next, *prev; }
//   OwnerNode* ownerHead;
//   const PokemonData pokedex[];
// ================================================

// --------------------------------------------------------------
// 1) Safe integer reading
// --------------------------------------------------------------

void trimWhitespace(char *str)
{
    // Remove leading spaces/tabs/\r
    int start = 0;
    while (str[start] == ' ' || str[start] == '\t' || str[start] == '\r')
        start++;

    if (start > 0)
    {
        int idx = 0;
        while (str[start])
            str[idx++] = str[start++];
        str[idx] = '\0';
    }

    // Remove trailing spaces/tabs/\r
    int len = (int)strlen(str);
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t' || str[len - 1] == '\r'))
    {
        str[--len] = '\0';
    }
}

char *myStrdup(const char *src)
{
    if (!src)
        return NULL;
    size_t len = strlen(src);
    char *dest = (char *)malloc(len + 1);
    if (!dest)
    {
        printf("Memory allocation failed in myStrdup.\n");
        return NULL;
    }
    strcpy(dest, src);
    return dest;
}

int readIntSafe(const char *prompt)
{
    char buffer[INT_BUFFER];
    int value;
    int success = 0;

    while (!success)
    {
        printf("%s", prompt);

        // If we fail to read, treat it as invalid
        if (!fgets(buffer, sizeof(buffer), stdin))
        {
            printf("Invalid input.\n");
            clearerr(stdin);
            continue;
        }

        // 1) Strip any trailing \r or \n
        //    so "123\r\n" becomes "123"
        size_t len = strlen(buffer);
        if (len > 0 && (buffer[len - 1] == '\n' || buffer[len - 1] == '\r'))
            buffer[--len] = '\0';
        if (len > 0 && (buffer[len - 1] == '\r' || buffer[len - 1] == '\n'))
            buffer[--len] = '\0';

        // 2) Check if empty after stripping
        if (len == 0)
        {
            printf("Invalid input.\n");
            continue;
        }

        // 3) Attempt to parse integer with strtol
        char *endptr;
        value = (int)strtol(buffer, &endptr, 10);

        // If endptr didn't point to the end => leftover chars => invalid
        // or if buffer was something non-numeric
        if (*endptr != '\0')
        {
            printf("Invalid input.\n");
        }
        else
        {
            // We got a valid integer
            success = 1;
        }
    }
    return value;
}

// --------------------------------------------------------------
// 2) Utility: Get type name from enum
// --------------------------------------------------------------
const char *getTypeName(PokemonType type)
{
    switch (type)
    {
    case GRASS:
        return "GRASS";
    case FIRE:
        return "FIRE";
    case WATER:
        return "WATER";
    case BUG:
        return "BUG";
    case NORMAL:
        return "NORMAL";
    case POISON:
        return "POISON";
    case ELECTRIC:
        return "ELECTRIC";
    case GROUND:
        return "GROUND";
    case FAIRY:
        return "FAIRY";
    case FIGHTING:
        return "FIGHTING";
    case PSYCHIC:
        return "PSYCHIC";
    case ROCK:
        return "ROCK";
    case GHOST:
        return "GHOST";
    case DRAGON:
        return "DRAGON";
    case ICE:
        return "ICE";
    default:
        return "UNKNOWN";
    }
}

// --------------------------------------------------------------
// Utility: getDynamicInput (for reading a line into malloc'd memory)
// --------------------------------------------------------------
char *getDynamicInput()
{
    char *input = NULL;
    size_t size = 0, capacity = 1;
    input = (char *)malloc(capacity);
    if (!input)
    {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        if (size + 1 >= capacity)
        {
            capacity *= 2;
            char *temp = (char *)realloc(input, capacity);
            if (!temp)
            {
                printf("Memory reallocation failed.\n");
                free(input);
                return NULL;
            }
            input = temp;
        }
        input[size++] = (char)c;
    }
    input[size] = '\0';

    // Trim any leading/trailing whitespace or carriage returns
    trimWhitespace(input);

    return input;
}

// Function to print a single Pokemon node
void printPokemonNode(PokemonNode *node)
{
    if (!node)
        return;
    printf("ID: %d, Name: %s, Type: %s, HP: %d, Attack: %d, Can Evolve: %s\n",
           node->data->id,
           node->data->name,
           getTypeName(node->data->TYPE),
           node->data->hp,
           node->data->attack,
           (node->data->CAN_EVOLVE == CAN_EVOLVE) ? "Yes" : "No");
}

// --------------------------------------------------------------
// Display Menu
// --------------------------------------------------------------
void displayMenu(OwnerNode *owner)
{
    if (!owner->pokedexRoot)
    {
        printf("Pokedex is empty.\n");
        return;
    }

    printf("Display:\n");
    printf("1. BFS (Level-Order)\n");
    printf("2. Pre-Order\n");
    printf("3. In-Order\n");
    printf("4. Post-Order\n");
    printf("5. Alphabetical (by name)\n");

    int choice = readIntSafe("Your choice: ");

    switch (choice)
    {
    case 1:
        displayBFS(owner->pokedexRoot);
        break;
    case 2:
        preOrderTraversal(owner->pokedexRoot);
        break;
    case 3:
        inOrderTraversal(owner->pokedexRoot);
        break;
    case 4:
        postOrderTraversal(owner->pokedexRoot);
        break;
    case 5:
        displayAlphabetical(owner->pokedexRoot);
        break;
    default:
        printf("Invalid choice.\n");
    }
}

// --------------------------------------------------------------
// Sub-menu for existing Pokedex
// --------------------------------------------------------------
void enterExistingPokedexMenu()
{
    if (!ownerHead) {
        printf("No existing Pokedexes.\n");
        return;
    }
    // list owners
    printf("\nExisting Pokedexes:\n");

    printAllOwnersIndexed();

    // choose owner
    int ownerIndex = readIntSafe("Choose a Pokedex by number: ");
    // find to the chosen one
    OwnerNode *cur = findOwnerByIndex(ownerIndex);
    if (!cur) {
        printf("Invalid choice.\n");
        return;
    }

    printf("\nEntering %s's Pokedex...\n", cur->ownerName);

    int subChoice;
    do
    {
        printf("\n-- %s's Pokedex Menu --\n", cur->ownerName);
        printf("1. Add Pokemon\n");
        printf("2. Display Pokedex\n");
        printf("3. Release Pokemon (by ID)\n");
        printf("4. Pokemon Fight!\n");
        printf("5. Evolve Pokemon\n");
        printf("6. Back to Main\n");

        subChoice = readIntSafe("Your choice: ");

        switch (subChoice)
        {
        case 1:
            addPokemon(cur);
            break;
        case 2:
            displayMenu(cur);
            break;
        case 3:
            freePokemon(cur);
            break;
        case 4:
            pokemonFight(cur);
            break;
        case 5:
            evolvePokemon(cur);
            break;
        case 6:
            printf("Back to Main Menu.\n");
            break;
        default:
            printf("Invalid choice.\n");
        }
    } while (subChoice != 6);
}

// --------------------------------------------------------------
// Main Menu
// --------------------------------------------------------------
void mainMenu()
{
    int choice;
    do
    {
        printf("\n=== Main Menu ===\n");
        printf("1. New Pokedex\n");
        printf("2. Existing Pokedex\n");
        printf("3. Delete a Pokedex\n");
        printf("4. Merge Pokedexes\n");
        printf("5. Sort Owners by Name\n");
        printf("6. Print Owners in a direction X times\n");
        printf("7. Exit\n");
        choice = readIntSafe("Your choice: ");

        switch (choice)
        {
        case 1:
            openPokedexMenu();
            break;
        case 2:
            enterExistingPokedexMenu();
            break;
        case 3:
            deletePokedex();
            break;
        case 4:
            mergePokedexMenu();
            break;
        case 5:
            sortOwners();
            break;
        case 6:
            printOwnersCircular();
            break;
        case 7:
            printf("Goodbye!\n");
            break;
        default:
            printf("Invalid.\n");
        }
    } while (choice != 7);
}

int main()
{
    mainMenu();
    freeAllOwners();
    return 0;
}

/* ------------------------------------------------------------
   Creating & Freeing Nodes
   ------------------------------------------------------------ */

PokemonNode  *createPokemonNode(const PokemonData *data) {
    PokemonNode *newNode = (PokemonNode *) malloc(sizeof(PokemonNode));
    if (!newNode) {
        printf("Memory allocation failed in createPokemonNode.\n");
        return NULL;
    }
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

OwnerNode *createOwner(char *ownerName, PokemonNode *starter) {
    OwnerNode *newNode = (OwnerNode *) malloc(sizeof(OwnerNode));
    if (!newNode) {
        printf("Memory allocation failed in createOwner.\n");
        return NULL;
    }
    newNode->ownerName = ownerName;
    newNode->pokedexRoot = starter;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}

void freePokemonNode(PokemonNode *node) {
    node->data = NULL;
    node->right = NULL;
    node->left = NULL;
    free(node);
}

void freePokemonTree(PokemonNode *root) {
    if (!root) {
        return;
    }
    freePokemonTree(root->left);
    freePokemonTree(root->right);
    freePokemonNode(root);
}

void freeOwnerNode(OwnerNode *owner) {
    if (!owner) {
        return;
    }
    free(owner->ownerName);
    freePokemonTree(owner->pokedexRoot);
    owner->next = NULL;
    owner->prev = NULL;
    free(owner);
}

/* ------------------------------------------------------------
   BST Insert, Search, Remove
   ------------------------------------------------------------ */

PokemonNode *insertPokemonNode(PokemonNode *root, PokemonNode *newNode) {
    if (!root || !newNode) {
        return newNode;
    }
    if (root->data->id == newNode->data->id) {
        freePokemonNode(newNode);
        return root;
    } else if (root->data->id < newNode->data->id) {
        root->right = insertPokemonNode(root->right, newNode);
    } else {
        root->left = insertPokemonNode(root->left, newNode);
    }
    return root;
}

PokemonNode *searchPokemonBFS(PokemonNode *root, int id) {
    while (root) {
        if (root->data->id == id) {
            return root;
        } else if (root->data->id < id) {
            root = root->right;
        } else {
            root = root->left;
        }
    }
    return root;
}

PokemonNode *removeNodeBST(PokemonNode *root, int id) {
    if (!root) {
        return root;
    }
    if (root->data->id > id) {
        root->left = removeNodeBST(root->left, id);
    } else if (root->data->id < id) {
        root->right = removeNodeBST(root->right, id);
    } else {
        // one right child or zero children
        if (!root->left) {
            PokemonNode *temp = root->right;
            freePokemonNode(root);
            return temp;
        }
        // one left child or zero children
        if (!root->right) {
            PokemonNode *temp = root->left;
            freePokemonNode(root);
            return temp;
        }
        // two children
        PokemonNode *successor = root->right;
        while (successor->left) {
            successor = successor->left;
        }
        root->data = successor->data;
        // delete successor
        root->right = removeNodeBST(root->right, successor->data->id);
    }
    return root;
}

PokemonNode *removePokemonByID(PokemonNode *root, int id) {
    if (!searchPokemonBFS(root, id)) {
        return root;
    }
    printf("Removing Pokemon %s (ID %d).\n", pokedex[id].name, id);
    return removeNodeBST(root, id);
}

/* ------------------------------------------------------------
   Generic BST Traversals (Function Pointers)
   ------------------------------------------------------------ */

void BFSGeneric(PokemonNode *root, VisitNodeFunc visit) {
    if (!root) {
        return;
    }
    NodeQueue *queue = initNodeQueue(INIT_QUEUE_CAPACITY);
    insertNodeQueue(queue, root);
    while (!isEmptyNodeQueue(queue)) {
        PokemonNode *node = popNodeQueue(queue);
        insertNodeQueue(queue, node->left);
        insertNodeQueue(queue, node->right);
        visit(node);
    }
    freeNodeQueue(queue);
}

void preOrderGeneric(PokemonNode *root, VisitNodeFunc visit) {
    if (!root) {
        return;
    }
    visit(root);
    preOrderGeneric(root->left, visit);
    preOrderGeneric(root->right, visit);
}

void inOrderGeneric(PokemonNode *root, VisitNodeFunc visit) {
    if (!root) {
        return;
    }
    inOrderGeneric(root->left, visit);
    visit(root);
    inOrderGeneric(root->right, visit);
}

void postOrderGeneric(PokemonNode *root, VisitNodeFunc visit) {
    if (!root) {
        return;
    }
    postOrderGeneric(root->left, visit);
    postOrderGeneric(root->right, visit);
    visit(root);
}

/* ------------------------------------------------------------
   Display Methods (BFS, Pre, In, Post, Alphabetical)
   ------------------------------------------------------------ */

NodeQueue *initNodeQueue(int cap) {
    NodeQueue *queue = (NodeQueue *) malloc(sizeof(NodeQueue));
    queue->size = 0;
    queue->index = 0;
    queue->capacity = cap;
    queue->nodes = (PokemonNode **) malloc(queue->capacity * sizeof(PokemonNode *));
    return queue;
}

void insertNodeQueue(NodeQueue *nq, PokemonNode *node) {
    if (!nq || !node) {
        return;
    }
    if (nq->size == nq->capacity) {
        // max capacity reached, realloc
        nq->capacity *= QUEUE_CAPACITY_REALLOC_MULTIPLIER;
        PokemonNode **temp = (PokemonNode **) realloc(nq->nodes, nq->capacity * sizeof(PokemonNode *));
        if (!temp) {
            printf("Memory reallocation failed in insertNodeQueue.\n");
            return;
        }
        nq->nodes = temp;
    }
    nq->nodes[nq->size] = node;
    nq->size++;
}

PokemonNode *popNodeQueue(NodeQueue *nq) {
    if (!nq) {
        return NULL;
    }
    if (nq->index >= nq->size) {
        return NULL;
    }
    nq->index++;
    return nq->nodes[nq->index - 1];
}

int isEmptyNodeQueue(NodeQueue *nq) {
    return !(nq && (nq->index < nq->size));
}

void freeNodeQueue(NodeQueue *nq) {
    if (!nq) {
        return;
    }
    free(nq->nodes);
    free(nq);
}

void collectAllNodeQueue(PokemonNode *root, NodeQueue *nq) {
    // Classic inOrder traversal, but not using generics cuz of the signature
    if (!root) {
        return;
    }
    collectAllNodeQueue(root->left, nq);
    insertNodeQueue(nq, root);
    collectAllNodeQueue(root->right, nq);
}

int comparePokemonNodeByName(const void *a, const void *b) {
    const PokemonNode **first = (const PokemonNode **) a;
    const PokemonNode **second = (const PokemonNode **) b;
    int result = strcmp((*first)->data->name, (*second)->data->name);
    // strcmp returns a negative value, zero or a positive value
    // return -1, 0, +1 accordingly
    return (result < 0) ? -1 : ((result > 0) ? 1 : 0);
}

void displayAlphabetical(PokemonNode *root) {
    if (!root) {
        return;
    }
    NodeQueue *queue = initNodeQueue(INIT_QUEUE_CAPACITY);
    collectAllNodeQueue(root, queue);
    qsort(queue->nodes, queue->size, sizeof(PokemonNode *), comparePokemonNodeByName);
    for (int i = 0; i < queue->size; ++i) {
        printPokemonNode(queue->nodes[i]);
    }
    freeNodeQueue(queue);
}

void displayBFS(PokemonNode *root) {
    BFSGeneric(root, printPokemonNode);
}

void preOrderTraversal(PokemonNode *root) {
    preOrderGeneric(root, printPokemonNode);
}

void inOrderTraversal(PokemonNode *root) {
    inOrderGeneric(root, printPokemonNode);
}

void postOrderTraversal(PokemonNode *root) {
    postOrderGeneric(root, printPokemonNode);
}

/* ------------------------------------------------------------
   Pokemon-Specific
   ------------------------------------------------------------ */

void pokemonFight(OwnerNode *owner) {
    if (!owner->pokedexRoot) {
        printf("Pokedex is empty.\n");
        return;
    }
    int firstID = readIntSafe("Enter ID of the first Pokemon: ");
    int secondID = readIntSafe("Enter ID of the second Pokemon: ");
    PokemonNode *first = searchPokemonBFS(owner->pokedexRoot, firstID);
    PokemonNode *second = searchPokemonBFS(owner->pokedexRoot, secondID);
    if (!(first && second)) {
        printf("One or both Pokemon IDs not found.\n");
        return;
    }
    double firstScore = 1.5 * first->data->attack + 1.2 * first->data->hp;
    double secondScore = 1.5 * second->data->attack + 1.2 * second->data->hp;
    printf("Pokemon 1: %s (Score = %.2lf)\n", first->data->name, firstScore);
    printf("Pokemon 2: %s (Score = %.2lf)\n", second->data->name, secondScore);
    if (firstID == secondID || firstScore == secondScore) {
        // floating point comparison might be inaccurate
        printf("It's a tie!\n");
    } else if (firstScore > secondScore) {
        printf("%s wins!\n", first->data->name);
    } else if (secondScore > firstScore) {
        printf("%s wins!\n", second->data->name);
    }
}

void evolvePokemon(OwnerNode *owner) {
    if (!owner->pokedexRoot) {
        printf("Cannot evolve. Pokedex empty.\n");
        return;
    }
    int idToEvolve = readIntSafe("Enter ID of Pokemon to evolve: ");
    int idEvolved = idToEvolve + 1;
    PokemonNode *pokemonToEvolve = searchPokemonBFS(owner->pokedexRoot, idToEvolve);
    if (!pokemonToEvolve) {
        printf("No Pokemon with ID %d found.\n", idToEvolve);
    } else if (pokemonToEvolve->data->CAN_EVOLVE == CANNOT_EVOLVE) {
        printf("%s (ID %d) cannot evolve.\n", pokedex[idToEvolve].name, idToEvolve);
    } else {
        // can evolve
        // search for evolved version first to not allocate and free memory for nothing
        if (!searchPokemonBFS(owner->pokedexRoot, idToEvolve + 1)) {
            // remove the un-evolved version
            owner->pokedexRoot = removePokemonByID(owner->pokedexRoot, idToEvolve);
            // if evolved version is not found - insert
            owner->pokedexRoot = insertPokemonNode(owner->pokedexRoot, createPokemonNode(&pokedex[idEvolved]));
            printf("Pokemon evolved from %s (ID %d) to %s (ID %d).\n",
                   pokedex[idToEvolve].name, idToEvolve, pokedex[idEvolved].name, idEvolved);
        } else {
            printf("Evolution ID %d (%s) already in the Pokedex. Releasing %s (ID %d).\n",
                   idEvolved, pokedex[idEvolved].name, pokedex[idToEvolve].name, idToEvolve);
            // remove the un-evolved version
            owner->pokedexRoot = removePokemonByID(owner->pokedexRoot, idToEvolve);
        }
    }
}

void addPokemon(OwnerNode *owner) {
    int idToAdd = readIntSafe("Enter ID to add: ");
    if (!searchPokemonBFS(owner->pokedexRoot, idToAdd)) {
        owner->pokedexRoot = insertPokemonNode(owner->pokedexRoot, createPokemonNode(&pokedex[idToAdd]));
        printf("Pokemon %s (ID %d) added.\n", pokedex[idToAdd].name, idToAdd);
    } else {
        printf("Pokemon with ID %d is already in the Pokedex. No changes made.\n", idToAdd);
    }
}

void freePokemon(OwnerNode *owner) {
    if (!owner->pokedexRoot) {
        printf("No Pokemon to release.\n");
        return;
    }
    int idToRelease = readIntSafe("Enter Pokemon ID to release: ");
    if (!searchPokemonBFS(owner->pokedexRoot, idToRelease)) {
        printf("No Pokemon with ID %d found.\n", idToRelease);
    } else {
        owner->pokedexRoot = removePokemonByID(owner->pokedexRoot, idToRelease);
    }
}

/* ------------------------------------------------------------
   Sorting Owners (Bubble Sort on Circular List)
   ------------------------------------------------------------ */

void sortOwners(void) {
    // if empty - already sorted
    if (!ownerHead || ownerHead->next == ownerHead) {
        printf("0 or 1 owners only => no need to sort.\n");
        return;
    }
    // break the cyclic list into a regular doubly linked list
    OwnerNode *tail = ownerHead->prev;
    if (tail) {
        tail->next = NULL;
        ownerHead->prev = NULL;
    }

    // sort the list using merge sort
    ownerHead = mergeSort(ownerHead, compareOwnerNodeByName);
    if (!ownerHead) {
        // Something went totally wrong, abort
        printf("Sorting owners by name failed.\n");
        return;
    }
    // Update linked list tail after sorting
    tail = ownerHead;
    while (tail->next) {
        tail = tail->next;
    }
    // make the list cyclic again
    tail->next = ownerHead;
    ownerHead->prev = tail;
    printf("Owners sorted by name.\n");
}

OwnerNode *splitLinkedList(OwnerNode *head) {
    OwnerNode *fast = head;
    OwnerNode *slow = head;

    // Move fast pointer two steps and slow pointer
    // one step until fast reaches the end
    while (fast && fast->next) {
        fast = fast->next->next;
        if (fast) {
            slow = slow->next;
        }
    }

    // Split the list into two halves
    OwnerNode *temp = slow->next;
    slow->next = NULL;
    return temp;
}

OwnerNode *mergeLinkedLists(OwnerNode *first, OwnerNode *second, OwnerNodeComparator cmp) {

    // If either list is empty, return the other list
    if (first == NULL) return second;
    if (second == NULL) return first;

    // Pick the smaller value between first and second nodes
    // If nodes are equal, preserve order
    if (cmp(first, second) <= 0) {
        // Recursively merge the rest of the lists and
        // link the result to the current node
        first->next = mergeLinkedLists(first->next, second, cmp);
        if (first->next) {
            first->next->prev = first;
        }
        first->prev = NULL;
        return first;
    } else {
        // Recursively merge the rest of the lists
        // and link the result to the current node
        second->next = mergeLinkedLists(first, second->next, cmp);
        if (second->next) {
            second->next->prev = second;
        }
        second->prev = NULL;
        return second;
    }
}

OwnerNode *mergeSort(OwnerNode *head, OwnerNodeComparator cmp) {
    // Base case: if the list is empty or has only one node, 
    // it's already sorted
    if (head == NULL || head->next == NULL) {
        return head;
    }
    // Split the list into two halves
    OwnerNode *second = splitLinkedList(head);

    // Recursively sort each half
    head = mergeSort(head, cmp);
    second = mergeSort(second, cmp);

    // Merge the two sorted halves
    return mergeLinkedLists(head, second, cmp);
}

int compareOwnerNodeByName(const void *a, const void *b) {
    const OwnerNode *first = (const OwnerNode *) a;
    const OwnerNode *second = (const OwnerNode *) b;
    int result = strcmp(first->ownerName, second->ownerName);
    // strcmp returns a negative value, zero or a positive value
    // return -1, 0, +1 accordingly
    return (result < 0) ? -1 : ((result > 0) ? 1 : 0);
}

/* ------------------------------------------------------------
   Circular List Linking & Searching
   ------------------------------------------------------------ */

void linkOwnerInCircularList(OwnerNode *newOwner) {
    if (!newOwner) {
        return;
    }
    if (!ownerHead) {
        // no owners in the list
        ownerHead = newOwner;
        newOwner->next = newOwner;
        newOwner->prev = newOwner;
        return;
    }
    // at least one owner in the list
    newOwner->prev = ownerHead->prev;
    newOwner->next = ownerHead;
    ownerHead->prev->next = newOwner;
    ownerHead->prev = newOwner;
}

void removeOwnerFromCircularList(OwnerNode *target) {
    if (!target) {
        return;
    }
    // If target has no next or next is target itself, then it's the only owner in the list
    if (!(target->next && target->next != target)) {
        ownerHead = NULL;
    } else {
        // at least two owners are in the list
        // update links
        target->prev->next = target->next;
        target->next->prev = target->prev;
    }
    if (target == ownerHead) {
        ownerHead = target->next;
    }
    // free the target node
    freeOwnerNode(target);
}

OwnerNode *findOwnerByName(const char *name) {
    if (!ownerHead) {
        return NULL;
    }
    OwnerNode *current = ownerHead;
    // search until got to NULL or returned to the ownerHead
    do {
        if (strcmp(current->ownerName, name) == 0) {
            return current;
        }
        current = current->next;
    } while(current && current != ownerHead);
    return NULL;
}

OwnerNode *findOwnerByIndex(int index) {
    // iterate over owners to get to the chosen one
    OwnerNode *cur = ownerHead;
    for (int i = 1; i < index && cur; ++i) {
        cur = cur->next;
    }
    return cur;
}

void printAllOwnersIndexed(void) {
    if (!ownerHead) {
        return;
    }
    OwnerNode *temp = ownerHead;
    int counter = 1;
    do {
        printf("%d. %s\n", counter, temp->ownerName);
        temp = temp->next;
        counter += 1;
    } while (temp != ownerHead);
}

/* ------------------------------------------------------------
   Owner Menus
   ------------------------------------------------------------ */

void openPokedexMenu(void) {
    printf("Your name: ");
    char *name = getDynamicInput();
    if (!findOwnerByName(name)) {
        printf("Choose Starter:\n"
               "1. Bulbasaur\n"
               "2. Charmander\n"
               "3. Squirtle\n");
        int choice = readIntSafe("Your choice: ");
        linkOwnerInCircularList(createOwner(name, createPokemonNode(starters[choice])));
        printf("New Pokedex created for %s with starter %s.\n", name, starters[choice]->name);
    } else {
        printf("Owner '%s' already exists. Not creating a new Pokedex.\n", name);
        free(name);
    }
}

void deletePokedex(void) {
    if (!ownerHead) {
        printf("No existing Pokedexes to delete.\n");
        return;
    }
    printf("\n"
           "=== Delete a Pokedex ===\n");
    printAllOwnersIndexed();
    int pokedexIndexToDelete = readIntSafe("Choose a Pokedex to delete by number: ");
    // find to the chosen one
    OwnerNode *cur = findOwnerByIndex(pokedexIndexToDelete);
    if (!cur) {
        printf("Invalid choice.\n");
        return;
    }
    printf("Deleting %s's entire Pokedex...\n", cur->ownerName);
    removeOwnerFromCircularList(cur);
    printf("Pokedex deleted.\n");
}

void mergePokedex(OwnerNode *first, OwnerNode *second) {
    // non-generic BFS cuz of the signature :(
    NodeQueue *queue = initNodeQueue(INIT_QUEUE_CAPACITY);
    insertNodeQueue(queue, second->pokedexRoot);
    while (!isEmptyNodeQueue(queue)) {
        PokemonNode *node = popNodeQueue(queue);
        insertNodeQueue(queue, node->left);
        insertNodeQueue(queue, node->right);
        // Insert new node with no connection to previous owner
        first->pokedexRoot = insertPokemonNode(first->pokedexRoot, createPokemonNode(node->data));
    }
    freeNodeQueue(queue);
    printf("Merge completed.\n");
}

void mergePokedexMenu(void) {
    if (!ownerHead || ownerHead->next == ownerHead) {
        printf("Not enough owners to merge.\n");
        return;
    }
    printf("\n"
           "=== Merge Pokedexes ===\n");
    printf("Enter name of first owner: ");
    char *first = getDynamicInput();
    printf("Enter name of second owner: ");
    char *second = getDynamicInput();
    printf("Merging %s and %s...\n", first ,second);
    OwnerNode *firstOwner = findOwnerByName(first);
    OwnerNode *secondOwner = findOwnerByName(second);
    mergePokedex(firstOwner, secondOwner);
    removeOwnerFromCircularList(secondOwner);
    printf("Owner '%s' has been removed after merging.\n", second);
    free(first);
    free(second);
}

/* ------------------------------------------------------------
   Printing Owners in a Circle
   ------------------------------------------------------------ */

void printOwnersCircular(void) {
    if (!ownerHead) {
        printf("No owners.\n");
        return;
    }
    printf("Enter direction (F or B): ");
    // Read all possible junk, use only the first character as per assumption of a valid input
    char *directionInput = getDynamicInput();
    char direction = (char) tolower(directionInput[0]);
    free(directionInput);
    int printsNumber = readIntSafe("How many prints? ");
    OwnerNode *current = ownerHead;
    for (int i = 1; i <= printsNumber; ++i) {
        printf("[%d] %s\n", i, current->ownerName);
        if (direction == PRINT_DIRECTION_FORWARD) {
            current = current->next;
        } else if (direction == PRINT_DIRECTION_BACKWARDS) {
            current = current->prev;
        }
    }
}

/* ------------------------------------------------------------
   Cleanup All Owners at Program End
   ------------------------------------------------------------ */

void freeAllOwners(void) {
    if (ownerHead == NULL) {
        return;
    }
    // break the cyclic list into a regular doubly linked list
    if (ownerHead->prev) {
        ownerHead->prev->next = NULL;
        ownerHead->prev = NULL;
    }
    OwnerNode *current = ownerHead;
    // free all nodes until tail
    while (current->next) {
        current = current->next;
        freeOwnerNode(current->prev);
    }
    // free tail
    freeOwnerNode(current);
    // set ownerHead to NULL
    ownerHead = NULL;
}
