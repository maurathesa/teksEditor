
#include "teksEditor.h" // Mengimpor definisi struct dan fungsi dari header file
#include <iostream>     // Untuk input/output standar
#include <fstream>      // Untuk operasi file

using namespace std;

// Fungsi untuk membuat list kosong
void createList(list &L) {
    L.first = NULL;
    L.last = NULL;
}

// Fungsi untuk mengalokasikan memori untuk elemen list baru
elmList* allocate(string data) {
    elmList* P = new elmList;
    P->info = data;
    P->next = NULL;
    P->prev = NULL;
    return P;
}

// Fungsi untuk menghapus elemen list
void deallocate(elmList* P) {
    delete P;
}

// Fungsi untuk menyisipkan elemen di awal list
void insertFirst(list &L, elmList* P) {
    if (L.first == NULL) {
        L.first = P;
        L.last = P;
    } else {
        P->next = L.first;
        L.first->prev = P;
        L.first = P;
    }
}

// Fungsi untuk menyisipkan elemen di akhir list
void insertLast(list &L, elmList* P) {
    if (L.first == NULL) {
        L.first = P;
        L.last = P;
    } else {
        L.last->next = P;
        P->prev = L.last;
        L.last = P;
    }
}

// Fungsi untuk menyisipkan elemen setelah elemen tertentu
void insertAfter(list &L, elmList* Prec, elmList* P) {
    if (Prec != NULL) {
        P->next = Prec->next;
        P->prev = Prec;
        if (Prec->next != NULL) {
            Prec->next->prev = P;
        } else {
            L.last = P;
        }
        Prec->next = P;
    }
}

// Fungsi untuk menghapus elemen pertama dari list
void deleteFirst(list &L, elmList* &P) {
    if (L.first != NULL) {
        P = L.first;
        L.first = L.first->next;
        if (L.first != NULL) {
            L.first->prev = NULL;
        } else {
            L.last = NULL;
        }
        P->next = NULL;
    }
}

// Fungsi untuk menghapus elemen terakhir dari list
void deleteLast(list &L, elmList* &P) {
    if (L.last != NULL) {
        P = L.last;
        if (L.first == L.last) {
            L.first = NULL;
            L.last = NULL;
        } else {
            L.last = L.last->prev;
            L.last->next = NULL;
            P->prev = NULL;
        }
    }
}

// Fungsi untuk menghapus elemen setelah elemen tertentu
void deleteAfter(elmList* Prec, elmList* &P) {
    if (Prec != NULL && Prec->next != NULL) {
        P = Prec->next;
        Prec->next = P->next;
        if (P->next != NULL) {
            P->next->prev = Prec;
        }
        P->next = NULL;
        P->prev = NULL;
    }
}

// Fungsi-fungsi untuk stack (untuk undo/redo)

// Fungsi untuk membuat stack kosong
void createStack(Stack &S) {
    S.top = NULL;
}

// Fungsi untuk memeriksa apakah stack kosong
bool isStackEmpty(const Stack &S) {
    return S.top == NULL;
}

// Fungsi untuk menambahkan elemen ke stack
void push(Stack &S, const list &L) {
    StackNode* newNode = new StackNode;
    newNode->data = copyList(L); // Menyimpan salinan list ke dalam stack
    newNode->next = S.top;
    S.top = newNode;
}

// Fungsi untuk mengambil elemen dari stack
void pop(Stack &S, list &L) {
    if (!isStackEmpty(S)) {
        StackNode* temp = S.top;
        L = temp->data; // Mengembalikan list dari stack
        S.top = S.top->next;
        delete temp;
    }
}

// Fungsi untuk menampilkan isi list (isi file)
void printList(list L) {
    if (L.first == NULL) {  // Periksa apakah list kosong
        cout << "File Kosong. Silakan isi teks." << endl;
    } else {
        elmList* current = L.first;
        int lineNumber = 1;
        while (current != NULL) {
            cout << lineNumber << ". " << current->info << endl;
            current = current->next;
            lineNumber++;
        }
    }
}

// Fungsi-fungsi untuk file

// Fungsi untuk membuat file baru
void createFile(const string &filename) {
    ofstream file(filename);
    if (file.is_open()) {
        cout << "File " << filename << " berhasil dibuat." << endl;
        file.close();
    } else {
        cout << "Gagal membuat file " << filename << endl;
    }
}

// Fungsi untuk membuka file yang ada
void openFile(list &L, const string &fileName) {
    ifstream inFile(fileName);

    if (!inFile) { // Jika file tidak ditemukan
        cout << "File '" << fileName << "' tidak ditemukan. Membaca input dari pengguna.\n";
        string line;
        cout << "Masukkan isi file (baris kosong untuk mengakhiri):\n";
        while (getline(cin, line) && !line.empty()) {
            elmList* P = allocate(line);
            insertLast(L, P);
        }
        cout << "File baru berhasil dibuat dari input pengguna.\n";
        return;
    }

    // Jika file ditemukan, baca datanya
    string line;
    while (getline(inFile, line)) {
        elmList* P = allocate(line);
        insertLast(L, P);
    }

    inFile.close();
    cout << "File '" << fileName << "' berhasil dibuka.\n";
}

// Fungsi untuk menyimpan file
void saveFile(const list& L, const string& fileName) {
    ofstream outFile(fileName); // Membuka file untuk ditulis
    if (!outFile) {
        cerr << "Error: Tidak dapat membuka file " << fileName << " untuk ditulis." << endl;
        return;
    }

    elmList* current = L.first;
    cout << "Isi file yang akan disimpan:\n";
    while (current != NULL) {
        cout << current->info << endl;
        outFile << current->info << endl;
        current = current->next;
    }

    outFile.close();
    if (outFile.good()) {
        cout << "File berhasil disimpan dengan nama " << fileName << endl;
    } else {
        cerr << "Error: Terjadi masalah saat menyimpan file." << endl;
    }
}

// Fungsi untuk menghapus file
void deleteFile(string &filename, list &L) {
    // Hapus isi list dari memori
    elmList* current = L.first;
    while (current != NULL) {
        elmList* next = current->next;
        delete current;
        current = next;
    }
    L.first = NULL;
    L.last = NULL;

    // Hapus file dari disk
    if (!filename.empty() && remove(filename.c_str()) == 0) {
        cout << "File '" << filename << "' berhasil dihapus dari disk.\n";
    } else {
        cerr << "Gagal menghapus file '" << filename << "'. File mungkin tidak ada.\n";
    }

    filename = "";
}

// Fungsi untuk menghitung jumlah baris dalam list
int length(const list &L) {
    int count = 0;
    elmList* current = L.first;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

// Fungsi untuk mengedit baris tertentu
void editLine(list &L, int lineNumber, string newContent) {
    if (lineNumber <= 0 || lineNumber > length(L)) {
        cout << "Nomor baris tidak valid.\n";
        return;
    }

    elmList* current = L.first;
    for (int i = 0; i < lineNumber - 1; i++) {
        current = current->next;
    }

    current->info = newContent;
}

// Fungsi untuk mencari baris berdasarkan nomor baris
elmList* findLine(const list &L, int lineNumber) {
    elmList* current = L.first;
    int count = 1;
    while (current != NULL && count < lineNumber) {
        current = current->next;
        count++;
    }
    return current;
}

// Fungsi untuk menghapus baris tertentu
void deleteLine(list &L, int lineNumber) {
    elmList* lineToDelete = findLine(L, lineNumber);
    if (lineToDelete != NULL) {
        if (lineToDelete == L.first) {
            deleteFirst(L, lineToDelete);
        } else if (lineToDelete == L.last) {
            deleteLast(L, lineToDelete);
        } else {
            deleteAfter(lineToDelete->prev, lineToDelete);
        }
        deallocate(lineToDelete);
    } else {
        cout << "Baris tidak ditemukan." << endl;
    }
}

// Fungsi untuk menyisipkan baris baru
void insertLine(list &L, int lineNumber, string content) {
    if (lineNumber <= 0 || lineNumber > length(L) + 1) {
        cout << "Nomor baris tidak valid.\n";
        return;
    }

    if (lineNumber == 1) {
        elmList* newNode = allocate(content);
        insertFirst(L, newNode);
    } else if (lineNumber == length(L) + 1) {
        elmList* newNode = allocate(content);
        insertLast(L, newNode);
    } else {
        elmList* prec = findLine(L, lineNumber - 1);
        elmList* newNode = allocate(content);
        insertAfter(L, prec, newNode);
    }
}

// Fungsi untuk menyalin list
list copyList(const list &L) {
    list newL;
    createList(newL);
    elmList* current = L.first;
    while (current != NULL) {
        elmList* newNode = allocate(current->info);
        insertLast(newL, newNode);
        current = current->next;
    }
    return newL;
}

// Fungsi untuk undo
void undo(list &L, Stack &undoStack, Stack &redoStack) {
    if (!isStackEmpty(undoStack)) {
        push(redoStack, L);
        pop(undoStack, L);
        cout << "Undo berhasil!" << endl;
    } else {
        cout << "Tidak ada aksi yang dapat di-undo." << endl;
    }
}

// Fungsi untuk redo
void redo(list &L, Stack &undoStack, Stack &redoStack) {
    if (!isStackEmpty(redoStack)) {
        push(undoStack, L);
        pop(redoStack, L);
        cout << "Redo berhasil!" << endl;
    } else {
        cout << "Tidak ada aksi yang dapat di-redo." << endl;
    }
}

// Fungsi untuk mencari kata dalam list
elmList* searchContent(const list &L, const string &keyword) {
    elmList* current = L.first;
    while (current != NULL) {
        if (current->info.find(keyword) != string::npos) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Fungsi untuk pergi ke baris tertentu
void goToLine(const list &L, int noLine) {
    if (noLine <= 0 || noLine > length(L)) {
        cout << "Nomor baris tidak valid.\n";
        return;
    }
    elmList* current = L.first;
    for (int i = 1; i < noLine; i++) {
        current = current->next;
    }
    cout << "Isi baris ke-" << noLine << ": " << current->info << endl;
}

// Fungsi untuk menduplikasi baris
void duplicateLine(list &L, int noLine) {
    elmList* lineToDuplicate = findLine(L, noLine);
    if (lineToDuplicate != NULL) {
        elmList* newLine = allocate(lineToDuplicate->info);
        insertAfter(L, lineToDuplicate, newLine);
        cout << "Baris ke-" << noLine << " berhasil diduplikasi." << endl;
    } else {
        cout << "Baris tidak ditemukan." << endl;
    }
}
