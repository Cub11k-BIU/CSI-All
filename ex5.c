/******************************************
* Student name: Konstantin Ostashenko
* Student ID: 345548929
* Exercise name: ex5
******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WATCH_PLAYLISTS_MENU_OPTION 1
#define ADD_PLAYLIST_MENU_OPTION 2
#define REMOVE_PLAYLIST_MENU_OPTION 3
#define EXIT_MENU_OPTION 4

#define SHOW_PLAYLIST_MENU_OPTION 1
#define ADD_SONG_PLAYLIST_MENU_OPTION 2
#define DELETE_SONG_PLAYLIST_MENU_OPTION 3
#define SORT_PLAYLIST_MENU_OPTION 4
#define PLAY_PLAYLIST_MENU_OPTION 5
#define EXIT_PLAYLIST_MENU_OPTION 6

#define YEAR_SORT_PLAYLIST_MENU_OPTION 1
#define STREAMS_ASCENDING_SORT_PLAYLIST_MENU_OPTION 2
#define STREAMS_DESCENDING_SORT_PLAYLIST_MENU_OPTION 3
#define ALPHABETICAL_SORT_PLAYLIST_MENU_OPTION 4

#define INITIAL_STRING_ALLOCATION_SIZE 1024
#define STRING_REALLOCATION_MULTIPLIER 2

typedef struct SongDetails {
    char *title;
    char *artist;
    char *lyrics;
    int year;
    int streams;
} SongDetails;

typedef struct SongNode {
    SongDetails *details;
    struct SongNode *prev;
    struct SongNode *next;
} SongNode;

typedef struct PlaylistDetails {
    char *name;
    SongNode *songsHead;
    SongNode *songsTail;
    int songsAmount;
} PlaylistDetails;

typedef struct PlaylistNode {
    PlaylistDetails *details;
    struct PlaylistNode *prev;
    struct PlaylistNode *next;
} PlaylistNode;

typedef struct PlaylistsLibrary {
    PlaylistNode *playlistsHead;
    PlaylistNode *playlistsTail;
    int playlistsAmount;
} PlaylistsLibrary;

typedef int (*SongDetailsComparator)(SongDetails *a, SongDetails *b);

// ------Printing functions------

void printMainMenu();
void printChoosePlaylistMenu(PlaylistsLibrary *lib);
void printPlaylistMenu();
void printSortPlaylistMenu();
void printSongs(SongNode *first);

// ----------Main menu-----------

void mainMenu(PlaylistsLibrary *lib);
void watchPlaylists(PlaylistsLibrary *lib);
void addPlaylist(PlaylistsLibrary *lib);
void removePlaylist(PlaylistsLibrary *lib);

// --------Playlist menu---------

void playlistMenu(PlaylistDetails *playlist);
void showPlaylist(PlaylistDetails *playlist);
void addSong(PlaylistDetails *playlist);
void deleteSong(PlaylistDetails *playlist);
void sortPlaylist(PlaylistDetails *playlist);
void playPlaylist(PlaylistDetails *playlist);

// ------Playlist utilities------

const char *endingChars = "\r\n";

void playSong(SongDetails *song);
SongNode *getSongById(SongNode *first, int songId);
PlaylistNode *getPlaylistById(PlaylistNode *first, int playlistId);
int isEndingChar(char c);
void inputInfiniteString(char **string);

// -----Comparator functions-----

int yearComparator(SongDetails *a, SongDetails *b);
int streamsAscendingComparator(SongDetails *a, SongDetails *b);
int streamsDescendingComparator(SongDetails *a, SongDetails *b);
int alphabeticalComparator(SongDetails *a, SongDetails *b);

// -----Merge sort functions-----

SongNode *splitLinkedList(SongNode *head);
SongNode *mergeLinkedLists(SongNode *first, SongNode *second, SongDetailsComparator cmp);
SongNode *mergeSort(SongNode *head, SongDetailsComparator cmp);

// -----Allocation functions-----

void *validatedMalloc(size_t size);
void *validatedRealloc(void *ptr, size_t size);

// ---------Initializers---------

SongDetails *initSongDetails();
SongNode *initSongNode();
PlaylistDetails *initPlaylistDetails();
PlaylistNode *initPlaylistNode();
PlaylistsLibrary *initPlaylistsLibrary();

// --------Free functions--------

void freeSongDetails(SongDetails *song);
void freeSongNode(SongNode *node);
void freeSongsLinkedList(SongNode *head);
void freePlaylistDetails(PlaylistDetails *playlist);
void freePlaylistNode(PlaylistNode *node);
void freePlaylistsLinkedList(PlaylistNode *head);
void freePlaylistsLibrary(PlaylistsLibrary *lib);

// ------------------------------

int main() {
    PlaylistsLibrary *lib = initPlaylistsLibrary();
    mainMenu(lib);
    freePlaylistsLibrary(lib);
    return 0;
}

void printMainMenu() {
    printf("Please Choose:\n"
           "\t1. Watch playlists\n"
           "\t2. Add playlist\n"
           "\t3. Remove playlist\n"
           "\t4. exit\n");
}

void printChoosePlaylistMenu(PlaylistsLibrary *lib) {
    if (lib == NULL) {
        return;
    }
    printf("Choose a playlist:\n");
    PlaylistNode *currentPlaylist = lib->playlistsHead;
    for (int i = 1; i <= lib->playlistsAmount; ++i) {
        printf("\t%d. %s\n", i, currentPlaylist->details->name);
        currentPlaylist = currentPlaylist->next;
    }
    printf("\t%d. Back to main menu\n", lib->playlistsAmount + 1);
}

void printPlaylistMenu() {
    printf("\t1. Show Playlist\n"
           "\t2. Add Song\n"
           "\t3. Delete Song\n"
           "\t4. Sort\n"
           "\t5. Play\n"
           "\t6. exit\n");
}

void printSortPlaylistMenu() {
    printf("choose:\n"
           "1. sort by year\n"
           "2. sort by streams - ascending order\n"
           "3. sort by streams - descending order\n"
           "4. sort alphabetically\n");
}

void printSongs(SongNode *first) {
    SongNode *current = first;
    int songId = 1;
    while (current) {
        SongDetails *details = current->details;
        printf("%d. Title: %s\n"
               "   Artist: %s\n"
               "   Released: %d\n"
               "   Streams: %d\n"
               "\n", songId, details->title, details->artist, details->year, details->streams);
        songId += 1;
        current = current->next;
    }
}

void mainMenu(PlaylistsLibrary *lib) {
    int mainMenuOption = 0;
    while (mainMenuOption != EXIT_MENU_OPTION) {
        printMainMenu();
        scanf("%d", &mainMenuOption);
        switch (mainMenuOption) {
            case WATCH_PLAYLISTS_MENU_OPTION:
                watchPlaylists(lib);
                break;
            case ADD_PLAYLIST_MENU_OPTION:
                addPlaylist(lib);
                break;
            case REMOVE_PLAYLIST_MENU_OPTION:
                removePlaylist(lib);
                break;
            default:
                break;
        }
    }
    printf("Goodbye!\n");
}

void watchPlaylists(PlaylistsLibrary *lib) {
    if (lib == NULL) {
        return;
    }
    int playlistId = 0;
    // While not back to menu
    while (playlistId != lib->playlistsAmount + 1) {
        printChoosePlaylistMenu(lib);
        scanf("%d", &playlistId);
        if (playlistId >= 1 && playlistId <= lib->playlistsAmount) {
            playlistMenu(getPlaylistById(lib->playlistsHead, playlistId)->details);
        }
    }
}

void addPlaylist(PlaylistsLibrary *lib) {
    if (lib == NULL) {
        return;
    }
    PlaylistNode *newPlaylist = initPlaylistNode();
    printf("Enter playlist's name:\n");
    inputInfiniteString(&newPlaylist->details->name);
    if (lib->playlistsTail) {
        lib->playlistsTail->next = newPlaylist;
        newPlaylist->prev = lib->playlistsTail;
    } else {
        lib->playlistsHead = newPlaylist;
    }
    lib->playlistsTail = newPlaylist;
    lib->playlistsAmount += 1;
}

void removePlaylist(PlaylistsLibrary *lib) {
    if (lib == NULL) {
        return;
    }
    int playlistId = 0;
    printChoosePlaylistMenu(lib);
    scanf("%d", &playlistId);
    if (playlistId >= 1 && playlistId <= lib->playlistsAmount) {
        PlaylistNode *chosenPlaylist = getPlaylistById(lib->playlistsHead, playlistId);
        if (chosenPlaylist->prev) {
            chosenPlaylist->prev->next = chosenPlaylist->next;
        } else {
            // chosenPlaylist is head
            lib->playlistsHead = lib->playlistsHead->next;
        }
        if (chosenPlaylist->next) {
            chosenPlaylist->next->prev = chosenPlaylist->prev;
        } else {
            // chosenPlaylist is tail
            lib->playlistsTail = lib->playlistsTail->prev;
        }
        freePlaylistNode(chosenPlaylist);
        lib->playlistsAmount -= 1;
        printf("Playlist deleted.\n");
    }
}

void playlistMenu(PlaylistDetails *playlist) {
    if (playlist == NULL) {
        return;
    }
    printf("playlist %s:\n", playlist->name);
    int playlistMenuOption = 0;
    // While not back to menu
    while (playlistMenuOption != EXIT_PLAYLIST_MENU_OPTION) {
        printPlaylistMenu();
        scanf("%d", &playlistMenuOption);
        switch (playlistMenuOption) {
            case SHOW_PLAYLIST_MENU_OPTION:
                showPlaylist(playlist);
                break;
            case ADD_SONG_PLAYLIST_MENU_OPTION:
                addSong(playlist);
                break;
            case DELETE_SONG_PLAYLIST_MENU_OPTION:
                deleteSong(playlist);
                break;
            case SORT_PLAYLIST_MENU_OPTION:
                sortPlaylist(playlist);
                break;
            case PLAY_PLAYLIST_MENU_OPTION:
                playPlaylist(playlist);
                break;
            default:
                break;
        }
    }
}

void showPlaylist(PlaylistDetails *playlist) {
    if (playlist == NULL) {
        return;
    }
    printSongs(playlist->songsHead);
    printf("choose a song to play, or 0 to quit:\n");
    int songId = 0;
    while (scanf("%d", &songId) == 1 && songId != 0) {
        if (playlist->songsAmount == 0) {
            return;
        }
        if (songId >= 1 && songId <= playlist->songsAmount) {
            playSong(getSongById(playlist->songsHead, songId)->details);
        }
        printf("choose a song to play, or 0 to quit:\n");
    }
}

void addSong(PlaylistDetails *playlist) {
    if (playlist == NULL) {
        return;
    }
    SongNode *newSong = initSongNode();
    printf("Enter song's details\n");
    printf("Title:\n");
    inputInfiniteString(&newSong->details->title);
    printf("Artist:\n");
    inputInfiniteString(&newSong->details->artist);
    printf("Year of release:\n");
    scanf("%d", &newSong->details->year);
    printf("Lyrics:\n");
    inputInfiniteString(&newSong->details->lyrics);
    if (playlist->songsTail) {
        playlist->songsTail->next = newSong;
        newSong->prev = playlist->songsTail;
    } else {
        // playlist is empty
        playlist->songsHead = newSong;
    }
    playlist->songsTail = newSong;
    playlist->songsAmount += 1;
}

void deleteSong(PlaylistDetails *playlist) {
    if (playlist == NULL) {
        return;
    }
    printSongs(playlist->songsHead);
    printf("choose a song to delete, or 0 to quit:\n");
    int songId;
    scanf("%d", &songId);
    if (songId >= 1 && songId <= playlist->songsAmount) {
        SongNode *chosenSong = getSongById(playlist->songsHead, songId);
        if (chosenSong == NULL) {
            return;
        }
        if (chosenSong->prev) {
            chosenSong->prev->next = chosenSong->next;
        } else {
            // chosenSong is head
            playlist->songsHead = playlist->songsHead->next;
        }
        if (chosenSong->next) {
            chosenSong->next->prev = chosenSong->prev;
        } else {
            // chosenSong is tail
            playlist->songsTail = playlist->songsTail->prev;
        }
        freeSongNode(chosenSong);
        playlist->songsAmount -= 1;
        printf("Song deleted successfully.\n");
    }
}

void sortPlaylist(PlaylistDetails *playlist) {
    if (playlist == NULL) {
        return;
    }
    printSortPlaylistMenu();
    int sortPlaylistOption = 0;
    scanf("%d", &sortPlaylistOption);
    // Sorting with merge sort, using different comparators for each option
    switch (sortPlaylistOption) {
        case YEAR_SORT_PLAYLIST_MENU_OPTION:
            playlist->songsHead = mergeSort(playlist->songsHead, yearComparator);
            break;
        case STREAMS_ASCENDING_SORT_PLAYLIST_MENU_OPTION:
            playlist->songsHead = mergeSort(playlist->songsHead, streamsAscendingComparator);
            break;
        case STREAMS_DESCENDING_SORT_PLAYLIST_MENU_OPTION:
            playlist->songsHead = mergeSort(playlist->songsHead, streamsDescendingComparator);
            break;
        case ALPHABETICAL_SORT_PLAYLIST_MENU_OPTION:
        default:
            playlist->songsHead = mergeSort(playlist->songsHead, alphabeticalComparator);
    }
    // Update linked list tail after sorting
    playlist->songsTail = playlist->songsHead;
    while (playlist->songsTail && playlist->songsTail->next) {
        playlist->songsTail = playlist->songsTail->next;
    }
    printf("sorted\n");
}

void playPlaylist(PlaylistDetails *playlist) {
    if (playlist == NULL) {
        return;
    }
    SongNode *current = playlist->songsHead;
    while (current) {
        playSong(current->details);
        current = current->next;
    }
}

// ------Playlist utilities------

void playSong(SongDetails *song) {
    printf("Now playing %s:\n"
           "$ %s $\n"
           "\n", song->title, song->lyrics);
    song->streams += 1;
}

SongNode *getSongById(SongNode *first, int songId) {
    SongNode *current = first;
    for (int i = 1; i < songId; ++i) {
        if (current) {
            current = current->next;
        } else {
            return current;
        }
    }
    return current;
}

PlaylistNode *getPlaylistById(PlaylistNode *first, int playlistId) {
    PlaylistNode *current = first;
    for (int i = 1; i < playlistId; ++i) {
        if (current) {
            current = current->next;
        } else {
            return current;
        }
    }
    return current;
}

int isEndingChar(char c) {
    return strchr(endingChars, c) != NULL;
}

/*
 * Input an infinitely (theoretically) long string into a pointer passed as an argument
 * Pointer value will be overwritten, any previously allocated memory will be lost
 * */
void inputInfiniteString(char **string) {
    // scanf trailing white-space characters from the buffer
    scanf(" ");
    int allocatedSize = INITIAL_STRING_ALLOCATION_SIZE;
    *string = (char *) validatedMalloc(allocatedSize * sizeof(char));
    int length = 0;
    char c = '\0';
    while (scanf("%c", &c) == 1 && !isEndingChar(c)) {
        (*string)[length] = c;
        length += sizeof(char);
        // If input is longer than the allocated memory - reallocate
        if (length == allocatedSize) {
            allocatedSize *= STRING_REALLOCATION_MULTIPLIER;
            *string = (char *) validatedRealloc(*string, allocatedSize * sizeof(char));
        }
    }
    (*string)[length] = '\0';
}

// -----Comparator functions-----

int yearComparator(SongDetails *a, SongDetails *b) {
    // Years are equal or first one is earlier
    return a->year <= b->year;
}

int streamsAscendingComparator(SongDetails *a, SongDetails *b) {
    // Streams are equal or first one has less
    return a->streams <= b->streams;
}

int streamsDescendingComparator(SongDetails *a, SongDetails *b) {
    // Streams are equal or first one has more
    return a->streams >= b->streams;
}

int alphabeticalComparator(SongDetails *a, SongDetails *b) {
    // Titles are equal or the first one is alphabetically first
    return strcmp(a->title, b->title) <= 0;
}

// -----Merge sort functions-----

// Split the doubly linked list into two halves
SongNode *splitLinkedList(SongNode *head) {
    SongNode *fast = head;
    SongNode *slow = head;

    // Move fast pointer two steps and slow pointer
    // one step until fast reaches the end
    while (fast && fast->next) {
        fast = fast->next->next;
        if (fast) {
            slow = slow->next;
        }
    }

    // Split the list into two halves
    SongNode *temp = slow->next;
    slow->next = NULL;
    return temp;
}

// Merge two sorted doubly linked lists
SongNode *mergeLinkedLists(SongNode *first, SongNode *second, SongDetailsComparator cmp) {

    // If either list is empty, return the other list
    if (first == NULL) return second;
    if (second == NULL) return first;

    // Pick the smaller value between first and second nodes
    if (cmp(first->details, second->details)) {
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

// Merge sort on a doubly linked list with varying comparator
SongNode *mergeSort(SongNode *head, SongDetailsComparator cmp) {
    // Base case: if the list is empty or has only one node, 
    // it's already sorted
    if (head == NULL || head->next == NULL) {
        return head;
    }
    // Split the list into two halves
    SongNode *second = splitLinkedList(head);

    // Recursively sort each half
    head = mergeSort(head, cmp);
    second = mergeSort(second, cmp);

    // Merge the two sorted halves
    return mergeLinkedLists(head, second, cmp);
}

// -----Allocation functions-----

/*
 * Allocate memory using malloc
 * Exit with failure code if malloc failed
 * Memory leaks possible if exited here
 * */
void *validatedMalloc(size_t size) {
    void *temp = malloc(size);
    if (temp == NULL) {
        exit(EXIT_FAILURE);
    }
    return temp;
}

/*
 * Allocate memory using realloc
 * Exit with failure code if realloc failed
 * Memory leaks possible if exited here
 * */
void *validatedRealloc(void *ptr, size_t size) {
    void *temp = realloc(ptr, size);
    if (temp == NULL) {
        exit(EXIT_FAILURE);
    }
    return temp;
}

// ---------Initializers---------

SongDetails *initSongDetails() {
    SongDetails *details = (SongDetails *) validatedMalloc(sizeof(SongDetails));
    details->title = NULL;
    details->lyrics = NULL;
    details->artist = NULL;
    details->streams = 0;
    details->year = 0;
    return details;
}

SongNode *initSongNode() {
    SongNode *newSong = (SongNode *) validatedMalloc(sizeof(SongNode));
    newSong->details = initSongDetails();
    newSong->prev = NULL;
    newSong->next = NULL;
    return newSong;
}

PlaylistDetails *initPlaylistDetails() {
    PlaylistDetails *details = (PlaylistDetails *) validatedMalloc(sizeof(PlaylistDetails));
    details->name = NULL;
    details->songsHead = NULL;
    details->songsTail = NULL;
    details->songsAmount = 0;
    return details;
}

PlaylistNode *initPlaylistNode() {
    PlaylistNode *newPlaylist = (PlaylistNode *) validatedMalloc(sizeof(PlaylistNode));
    newPlaylist->details = initPlaylistDetails();
    newPlaylist->prev = NULL;
    newPlaylist->next = NULL;
    return newPlaylist;
}

PlaylistsLibrary *initPlaylistsLibrary() {
    PlaylistsLibrary *lib = (PlaylistsLibrary *) validatedMalloc(sizeof(PlaylistsLibrary));
    lib->playlistsHead = NULL;
    lib->playlistsTail = NULL;
    lib->playlistsAmount = 0;
    return lib;
}

// --------Free functions--------

void freeSongDetails(SongDetails *song) {
    if (song == NULL) {
        return;
    }
    free(song->artist);
    free(song->lyrics);
    free(song->title);
    free(song);
}

void freeSongNode(SongNode *node) {
    if (node == NULL) {
        return;
    }
    freeSongDetails(node->details);
    free(node);
}

void freeSongsLinkedList(SongNode *head) {
    if (head == NULL) {
        return;
    }
    SongNode *current = head;
    // free all nodes until tail
    while (current->next) {
        current = current->next;
        freeSongNode(current->prev);
    }
    // free tail
    freeSongNode(current);
}

void freePlaylistDetails(PlaylistDetails *playlist) {
    if (playlist == NULL) {
        return;
    }
    free(playlist->name);
    freeSongsLinkedList(playlist->songsHead);
    free(playlist);
}

void freePlaylistNode(PlaylistNode *node) {
    if (node == NULL) {
        return;
    }
    freePlaylistDetails(node->details);
    free(node);
}

void freePlaylistsLinkedList(PlaylistNode *head) {
    if (head == NULL) {
        return;
    }
    PlaylistNode *current = head;
    // free all nodes until tail
    while (current->next) {
        current = current->next;
        freePlaylistNode(current->prev);
    }
    // free tail
    freePlaylistNode(current);
}

void freePlaylistsLibrary(PlaylistsLibrary *lib) {
    if (lib == NULL) {
        return;
    }
    freePlaylistsLinkedList(lib->playlistsHead);
    free(lib);
}
