#include "translator.h"

struct StackNode* newNode(int data) {
    struct StackNode* stackNode = (struct StackNode*) malloc(sizeof(struct StackNode));
    stackNode->data = data;
    stackNode->next = NULL;
    return stackNode;
}

bool isEmpty(struct StackNode *root) {
    return !root;
}

void push(struct StackNode** root, int data) {
    struct StackNode* stackNode = newNode(data);
    stackNode->next = *root;
    *root = stackNode;
    printf("%d pushed to stack\n", data);
}

int pop(struct StackNode** root) {
    if (isEmpty(*root))
        return INT_MIN;
    struct StackNode* temp = *root;
    *root = (*root)->next;
    int popped = temp->data;
    printf("%d popped\n", popped);
    free(temp);
 
    return popped;
}
 
int peek(struct StackNode* root) {
    if (isEmpty(root))
        return INT_MIN;
    return root->data;
}

void print(struct StackNode *root) {
    while (root != NULL) {
        printf("VALUE: %d\n", root -> data);
        root = root -> next;
    }
}

void parseCommands(struct StackNode **root, FILE *inputFile, FILE *outputFile) {
    
    ASSERT(inputFile, "input file not open on parse")
    ASSERT(outputFile, "output file not open on parse")
    char line[MAX_LINE_LENGTH];
    
    while (fgets(line, MAX_LINE_LENGTH, inputFile)) {
        if ((line[0] == '/' && line[1] == '/') || line[0] == '\n'
                                               || line[0] == '\r'
                                               || line[0] == '\0')
            continue;

        char *instr;
        int sol;
        for (sol = 0; sol < strlen(line); sol++) {
            if (line[sol] != ' ' && line[sol] != '\t')
                break;
        }
        int eol;
        for (eol = sol; sol < strlen(line); eol++) {
            if (line[eol] == '\0' || line[eol] == '\n' || line[eol] == '/' || line[eol] == '\r')
                break;
        }
    
        instr = malloc(((eol) - sol) + 1);
        strncpy(instr, line, (eol) - sol);
        instr[(eol) - sol] = '\0';

        printf("INSTR FOUND: %s\n", instr);
        int countSpaces = 0;
        for (int i = 0; i < strlen(instr); i++) {
            if (instr[i] == ' ')
                countSpaces++;
        }
        ASSERT(countSpaces == 2 || countSpaces == 0, "instruction formatted poorly")
        if (countSpaces == 2) {
            printf("THIS IS A STACK INSERT/DELETE\n");
        } else if (countSpaces == 0) {
            printf("THIS IS A STACK OP\n");
            handleStackOp(root, instr, outputFile);
        }
    }
}

void handleStackOp(struct StackNode **root, char *instr, FILE *outputFile) {
    int p1, p2, res;
    if (strncmp(instr, "add", strlen(instr)) == 0) {
        p1 = pop(root);
        p2 = pop(root);
        res = p1 + p2;
        push(root, res);
        return;
    } else if (strncmp(instr, "sub", strlen(instr)) == 0) {
        p1 = pop(root);
        p2 = pop(root);
        res = p2 - p1;
        push(root, res);
        return;
    } else if (strncmp(instr, "add", strlen(instr)) == 0) {
        p1 = pop(root);
        p2 = pop(root);
        res = p1 + p2;
        push(root, res);
        return;
    } else if (strncmp(instr, "add", strlen(instr)) == 0) {
        p1 = pop(root);
        p2 = pop(root);
        res = p1 + p2;
        push(root, res);
        return;
    } else if (strncmp(instr, "add", strlen(instr)) == 0) {
        p1 = pop(root);
        p2 = pop(root);
        res = p1 + p2;
        push(root, res);
        return;
    }
} 
