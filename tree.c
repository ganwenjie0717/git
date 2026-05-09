#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>
#include<sys/stat.h>
#include<unistd.h>
typedef struct FileNode {
     char *name;
     int isDir;
     struct FileNode *firstChild;
     struct FileNode *nextSibling;
 } FileNode;
 FileNode* createNode(const char *name, int isDir) {
     FileNode *node = (FileNode*)malloc(sizeof(FileNode));
     node->name = (char*)malloc(strlen(name) + 1);
     strcpy(node->name, name);
     node->isDir = isDir;
     node->firstChild = NULL;
     node->nextSibling = NULL;
     return node;
 }
 int cmpNode(const void *a, const void *b) {
     FileNode *n1 = *(FileNode**)a;
     FileNode *n2 = *(FileNode**)b;
     return strcmp(n1->name, n2->name);
 }
 FileNode* buildTree(const char *path) {
     DIR *dir = opendir(path);
     if (!dir) return NULL;
     FileNode *root = createNode(path, 1);
     struct dirent *entry;
     FileNode **children = NULL;
     int childCnt = 0;
     while ((entry = readdir(dir)) != NULL) {
         if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
             continue;
         char fullPath[1024];
         snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entry->d_name);
         struct stat st;
         if (stat(fullPath, &st) == -1) continue;
         FileNode *node;
         if (S_ISDIR(st.st_mode)) {
             node = buildTree(fullPath);
         } else {
             node = createNode(entry->d_name, 0);
         }
         if (node) {
             children = (FileNode**)realloc(children, (childCnt + 1) * sizeof(FileNode*));
             children[childCnt++] = node;
         }
     }
     closedir(dir);
     qsort(children, childCnt, sizeof(FileNode*), cmpNode);
     FileNode *prev = NULL;
     for (int i = 0; i < childCnt; i++) {
         if (i == 0)
             root->firstChild = children[i];
         else
             prev->nextSibling = children[i];
         prev = children[i];
     }
     if (children) free(children);
     return root;
 }
 void printTree(FileNode *node, const char *prefix, int isLast) {
     if (!node) return;
     printf("%s", prefix);
     if (isLast)
         printf("`-- ");
     else
         printf("|-- ");
     if (node->isDir)
         printf("%s/\n", node->name);
     else
         printf("%s\n", node->name);
     char newPrefix[1024];
     snprintf(newPrefix, sizeof(newPrefix), "%s%s", prefix, isLast ? "    " : "|   ");
     FileNode *child = node->firstChild;
     while (child) {
         int last = (child->nextSibling == NULL);
         printTree(child, newPrefix, last);
         child = child->nextSibling;
     }
 }
 int countNodes(FileNode *root) {
     if (!root) return 0;
     return 1 + countNodes(root->firstChild) + countNodes(root->nextSibling);
 }
 int countLeaves(FileNode *root) {
     if (!root) return 0;
     if (!root->firstChild)
         return 1;
     return countLeaves(root->firstChild) + countLeaves(root->nextSibling);
 }
 int treeHeight(FileNode *root) {
     if (!root) return 0;
     int maxH = 0;
     FileNode *child = root->firstChild;
     while (child) {
         int h = treeHeight(child);
         if (h > maxH) maxH = h;
         child = child->nextSibling;
     }
     return 1 + maxH;
 }
 void countDirFile(FileNode *root, int *dirs, int *files) {
     if (!root) return;
     if (root->isDir)
         (*dirs)++;
     else
         (*files)++;
     countDirFile(root->firstChild, dirs, files);
     countDirFile(root->nextSibling, dirs, files);
 }
 void freeTree(FileNode *root) {
     if (!root) return;
     freeTree(root->firstChild);
     freeTree(root->nextSibling);
     free(root->name);
     free(root);
 }
 int main(int argc, char *argv[]) {
     const char *path = ".";
     if (argc > 1)
         path = argv[1];
     FileNode *root = buildTree(path);
     if (!root) {
         fprintf(stderr, "目录打开失败\n");
         return 1;
     }
     printf("%s/\n", path);
     FileNode *child = root->firstChild;
     while (child) {
         int last = (child->nextSibling == NULL);
         printTree(child, "", last);
         child = child->nextSibling;
     }
     int dirs = 0, files = 0;
     countDirFile(root, &dirs, &files);
     int nodes = countNodes(root);
     int leaves = countLeaves(root);
     int height = treeHeight(root);
     printf("\n%d 个目录, %d 个文件\n", dirs, files);
     printf("二叉树结点总数: %d\n", nodes);
     printf("叶子结点数: %d\n", leaves);
     printf("树的高度: %d\n", height);
     freeTree(root);
     return 0;
 }