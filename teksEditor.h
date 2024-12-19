#ifndef TEKSEDITOR_H
#define TEKSEDITOR_H

#include <iostream>
#include <fstream>
using namespace std;

typedef string infoType;

struct elmList {
    infoType info;
    elmList *next;
    elmList *prev;
};
struct list {
    elmList *first;
    elmList *last;
};

struct StackNode {
    list data;
    StackNode* next;
};

struct Stack {
    StackNode* top;
};

void createList(list& L);
elmList* allocate(string data);
void deallocate(elmList* P);
void insertFirst(list& L, elmList* P);
void insertLast(list& L, elmList* P);
void insertAfter(list& L, elmList* Prec, elmList* P);
void deleteFirst(list& L, elmList*& P);
void deleteLast(list& L, elmList*& P);
void deleteAfter(elmList* Prec, elmList*& P);
void createStack(Stack& S);
bool isStackEmpty(const Stack& S);
void push(Stack& S, const list& L);
void pop(Stack& S, list& L);
void printList(list L);
void createFile(const string& filename);
void openFile(list &L, const string &fileName);
void saveFile(const list &L, const string &fileName);
void deleteFile(string& filename, list& L) ;
int length(const list &L);
void editLine(list &L, int lineNumber, string newContent) ;
void deleteLine(list &L, int lineNumber);
void insertLine(list &L, int lineNumber, string content);
list copyList(const list& L);
void undo(list& L, Stack& undoStack, Stack& redoStack);
void redo(list& L, Stack& undoStack, Stack& redoStack);
elmList* findLine(const list &L, int lineNumber);
int countLines(const list &L);
elmList* searchContent(const list &L, const string &keyword);
void goToLine(const list& L, int noLine);
void duplicateLine(list& L, int noLine);
void showAllFiles();

#endif
