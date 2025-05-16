#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MEMORIES 100
#define MAX_KEYWORD_LEN 30
#define MAX_CONTENT_LEN 100

typedef struct MemoryNode {
    char keyword[MAX_KEYWORD_LEN];
    char content[MAX_CONTENT_LEN];
    struct MemoryNode *child1;
    struct MemoryNode *child2;
    struct MemoryNode *parent;
} MemoryNode;

// Array to store all memories entered
MemoryNode *memories[MAX_MEMORIES];
int memoryCount = 0;

// Utility to create a new memory node
MemoryNode *createMemory(const char *keyword, const char *content) {
    MemoryNode *node = malloc(sizeof(MemoryNode));
    if (!node) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    strncpy(node->keyword, keyword, MAX_KEYWORD_LEN - 1);
    node->keyword[MAX_KEYWORD_LEN - 1] = '\0';
    strncpy(node->content, content, MAX_CONTENT_LEN - 1);
    node->content[MAX_CONTENT_LEN - 1] = '\0';
    node->child1 = NULL;
    node->child2 = NULL;
    node->parent = NULL;
    return node;
}

// Print all memories input so far
void printAllMemories() {
    printf("\nAll Memories:\n");
    for (int i = 0; i < memoryCount; i++) {
        printf("%d) Keyword: %s\n   Content: %s\n", i + 1, memories[i]->keyword, memories[i]->content);
    }
}

// Print mind map recursively with indentation
void printMindMap(MemoryNode *node, int indent) {
    if (!node) return;
    for (int i = 0; i < indent; i++) printf("  ");
    printf("- %s: %s\n", node->keyword, node->content);
    printMindMap(node->child1, indent + 1);
    printMindMap(node->child2, indent + 1);
}

// Find memory by keyword, return first match or NULL
MemoryNode *searchMemory(const char *keyword) {
    for (int i = 0; i < memoryCount; i++) {
        if (strcmp(memories[i]->keyword, keyword) == 0) {
            return memories[i];
        }
    }
    return NULL;
}

// Check if child slot is available (max 2 children)
int canAddChild(MemoryNode *parent) {
    return !(parent->child1 && parent->child2);
}

// Add child node to parent node (if possible)
int addChild(MemoryNode *parent, MemoryNode *child) {
    if (!canAddChild(parent)) return 0; // full
    if (parent->child1 == NULL) {
        parent->child1 = child;
    } else if (parent->child2 == NULL) {
        parent->child2 = child;
    }
    child->parent = parent;
    return 1;
}

// Remove a child from its parent
void removeFromParent(MemoryNode *node) {
    if (!node || !node->parent) return;
    MemoryNode *p = node->parent;
    if (p->child1 == node) p->child1 = NULL;
    else if (p->child2 == node) p->child2 = NULL;
    node->parent = NULL;
}

// Move a memory node to new parent (if constraints allow)
int moveNode(MemoryNode *node, MemoryNode *newParent) {
    if (!node || !newParent) return 0;
    if (node == newParent) return 0; // can't move to itself

    // prevent cycles - can't move node into its descendant
    MemoryNode *temp = newParent;
    while (temp) {
        if (temp == node) return 0;
        temp = temp->parent;
    }

    if (!canAddChild(newParent)) return 0; // no slot
    removeFromParent(node);
    addChild(newParent, node);
    return 1;
}

void reminisceMindmap() {
    char keyword[MAX_KEYWORD_LEN];
    char content[MAX_CONTENT_LEN];
    int choice;

    printf("Enter memories (keyword and content). Enter empty keyword to stop.\n");
    while (memoryCount < MAX_MEMORIES) {
        printf("Memory %d keyword: ", memoryCount + 1);
        fgets(keyword, sizeof(keyword), stdin);
        if (keyword[0] == '\n') break;
        keyword[strcspn(keyword, "\n")] = 0;

        printf("Memory %d content: ", memoryCount + 1);
        fgets(content, sizeof(content), stdin);
        content[strcspn(content, "\n")] = 0;

        memories[memoryCount++] = createMemory(keyword, content);
    }

    if (memoryCount == 0) {
        printf("No memories entered.\n");
        return;
    }

    printAllMemories();

    // Select root node
    int rootIndex;
    printf("\nSelect the root memory node by number (1-%d): ", memoryCount);
    scanf("%d", &rootIndex);
    getchar(); // consume newline
    if (rootIndex < 1 || rootIndex > memoryCount) {
        printf("Invalid selection. Exiting.\n");
        return;
    }
    MemoryNode *root = memories[rootIndex - 1];

    // Connect other memories to form mind map
    printf("\nConnect memories to form mind map. You can connect up to 2 children per node.\n");
    printf("Enter pairs: ParentKeyword ChildKeyword. Enter empty parent keyword to stop.\n");

    while (1) {
        char parentKey[MAX_KEYWORD_LEN];
        char childKey[MAX_KEYWORD_LEN];

        printf("Parent keyword (empty to finish): ");
        fgets(parentKey, sizeof(parentKey), stdin);
        if (parentKey[0] == '\n') break;
        parentKey[strcspn(parentKey, "\n")] = 0;

        printf("Child keyword: ");
        fgets(childKey, sizeof(childKey), stdin);
        childKey[strcspn(childKey, "\n")] = 0;

        MemoryNode *parent = searchMemory(parentKey);
        MemoryNode *child = searchMemory(childKey);

        if (!parent) {
            printf("Parent keyword '%s' not found.\n", parentKey);
            continue;
        }
        if (!child) {
            printf("Child keyword '%s' not found.\n", childKey);
            continue;
        }
        if (parent == child) {
            printf("Cannot connect node to itself.\n");
            continue;
        }
        if (!canAddChild(parent)) {
            printf("Parent '%s' already has 2 children.\n", parentKey);
            continue;
        }
        if (child->parent) {
            printf("Child '%s' is already connected to parent '%s'.\n", childKey, child->parent->keyword);
            continue;
        }

        addChild(parent, child);
        printf("Connected '%s' -> '%s'\n", parentKey, childKey);
    }

    // Print final mind map
    printf("\nFinal Memory Mind Map:\n");
    printMindMap(root, 0);

    // Bonus: Search memories by keyword
    while (1) {
        char searchKey[MAX_KEYWORD_LEN];
        printf("\nSearch memories by keyword (empty to exit): ");
        fgets(searchKey, sizeof(searchKey), stdin);
        if (searchKey[0] == '\n') break;
        searchKey[strcspn(searchKey, "\n")] = 0;

        MemoryNode *found = searchMemory(searchKey);
        if (found) {
            printf("Found memory:\n- %s: %s\n", found->keyword, found->content);
        } else {
            printf("Memory with keyword '%s' not found.\n", searchKey);
        }
    }

    // Bonus: Move nodes
    while (1) {
        char nodeKey[MAX_KEYWORD_LEN];
        char newParentKey[MAX_KEYWORD_LEN];

        printf("\nMove a memory node to new parent.\n");
        printf("Enter node keyword to move (empty to finish): ");
        fgets(nodeKey, sizeof(nodeKey), stdin);
        if (nodeKey[0] == '\n') break;
        nodeKey[strcspn(nodeKey, "\n")] = 0;

        printf("Enter new parent keyword: ");
        fgets(newParentKey, sizeof(newParentKey), stdin);
        newParentKey[strcspn(newParentKey, "\n")] = 0;

        MemoryNode *node = searchMemory(nodeKey);
        MemoryNode *newParent = searchMemory(newParentKey);

        if (!node || !newParent) {
            printf("Node or new parent keyword not found.\n");
            continue;
        }

        if (moveNode(node, newParent)) {
            printf("Moved '%s' under '%s'.\n", nodeKey, newParentKey);
            printf("Updated Mind Map:\n");
            printMindMap(root, 0);
        } else {
            printf("Failed to move '%s' under '%s'. Check constraints.\n", nodeKey, newParentKey);
        }
    }

    // Free allocated memories
    for (int i = 0; i < memoryCount; i++) {
        free(memories[i]);
    }
}

int main(void) {
    reminisceMindmap();
    return 0;
}
