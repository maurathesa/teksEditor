#include "teksEditor.h"
#include <iostream>

using namespace std;

int main() {
    list L;
    createList(L);
    Stack undoStack, redoStack;
    createStack(undoStack);
    createStack(redoStack);
    string fileName;
    int pilihan = -1;

    cout << "SELAMAT DATANG DI TEKS EDITOR WARLOK ASLI!" << endl;
    cout << "Masukkan nama file yang sudah dibuat: ";
    cin >> fileName;

    if (system(("test -f " + fileName).c_str()) == 0) {
        cout << "File '" << fileName << "' ditemukan. Membuka file...\n";
        openFile(L, fileName);
    } else {
        cout << "File " << fileName << " tidak ditemukan, silahkan buat file baru";
    }

    do {
        cout << "\nMenu Editor Teks:" << endl;
        cout << "1. Buat File Baru" << endl;
        cout << "2. Buka File yang Ada" << endl;
        cout << "3. Simpan File" << endl;
        cout << "4. Edit Teks" << endl;
        cout << "5. Hapus Teks" << endl;
        cout << "6. Input Teks " << endl;
        cout << "7. Print teks dalam file" << endl;
        cout << "8. Undo Teks" << endl;
        cout << "9. Redo Teks" << endl;
        cout << "10. Cari Kata" << endl;
        cout << "11. Pergi ke Baris Tertentu" << endl;
        cout << "12. Copy Baris" << endl;
        cout << "13. Hapus File" << endl;
        cout << "0. Keluar" << endl;
        cout << "Pilih menu (0-13): ";
        cin >> pilihan;

        switch (pilihan) {
            case 1: {
                cout << "Anda memilih untuk membuat file baru.\n";
                cout << "Masukkan nama file baru yang ingin dibuat: ";
                cin >> fileName;
                createFile(fileName);
                break;
            }
            case 2: {
                cout << "Anda memilih untuk membuka file yang sudah ada.\n";
                cout << "Masukkan nama file yang ingin dibuka: ";
                cin >> fileName;
                openFile(L, fileName);
                break;
            }
            case 3:
                saveFile(L, fileName);
                cout << "File '" << fileName << "' berhasil disimpan.\n";
                break;
            case 4: {
                int lineNumber;
                string newContent;
                cout << "Anda memilih untuk mengedit baris.\n";
                cout << "Masukkan nomor baris yang ingin diedit: ";
                cin >> lineNumber;
                
                if (cin.fail() || lineNumber <= 0 || lineNumber > length(L)) {
                    cout << "Input tidak valid. Nomor baris harus berupa angka positif dan tidak melebihi jumlah baris.\n";
                    cin.clear();
                    cin.ignore(10000, '\n');
                    break;
                }
                
                cin.ignore();
                cout << "Masukkan konten baru untuk baris " << lineNumber << ": ";
                getline(cin, newContent);
                push(undoStack, L);
                editLine(L, lineNumber, newContent);
                cout << "Baris " << lineNumber << " berhasil diedit.\n";
                break;
            }
            case 5: {
                int lineNumber;
                cout << "Anda memilih untuk menghapus baris.\n";
                cout << "Masukkan nomor baris yang ingin dihapus: ";
                cin >> lineNumber;
                
                if (cin.fail() || lineNumber <= 0 || lineNumber > length(L)) {
                    cout << "Input tidak valid. Nomor baris harus berupa angka positif dan tidak melebihi jumlah baris.\n";
                    cin.clear();
                    cin.ignore(10000, '\n');
                    break;
                }
                
                push(undoStack, L);
                deleteLine(L, lineNumber);
                cout << "Baris " << lineNumber << " berhasil dihapus.\n";
                break;
            }
            case 6: {
                int lineNumber;
                string newContent;
                cout << "Anda memilih untuk menginput teks.\n";
                cout << "Masukkan nomor baris tempat teks akan disisipkan: ";
                cin >> lineNumber;
                
                if (cin.fail() || lineNumber <= 0 || lineNumber > length(L) + 1) {
                    cout << "Input tidak valid. Nomor baris harus berupa angka positif dan tidak melebihi jumlah baris + 1.\n";
                    cin.clear();
                    cin.ignore(10000, '\n');
                    break;
                }
                
                cin.ignore();
                cout << "Masukkan teks yang ingin disisipkan: ";
                getline(cin, newContent);
                push(undoStack, L);
                insertLine(L, lineNumber, newContent);
                cout << "Teks berhasil disisipkan pada baris " << lineNumber << ".\n";
                break;
            }
            case 7:
                cout << "Isi file '" << fileName << "':\n";
                printList(L);
                break;
            case 8:
                undo(L, undoStack, redoStack);
                break;
            case 9:
                redo(L, undoStack, redoStack);
                break;
            case 10: {
                string keyword;
                cout << "Masukkan kata yang ingin dicari: ";
                cin.ignore();
                getline(cin, keyword);
                elmList* foundLine = searchContent(L, keyword);
                if (foundLine != NULL) {
                    cout << "Kata '" << keyword << "' ditemukan pada baris: " << endl;
                    // Anda perlu menambahkan fungsi untuk mendapatkan nomor baris dari elmList*
                } else {
                    cout << "Kata '" << keyword << "' tidak ditemukan dalam file.\n";
                }
                break;
            }
            case 11: {
                int lineNumber;
                cout << "Masukkan nomor baris yang ingin dituju: ";
                cin >> lineNumber;
                
                if (cin.fail() || lineNumber <= 0 || lineNumber > length(L)) {
                    cout << "Input tidak valid. Nomor baris harus berupa angka positif dan tidak melebihi jumlah baris.\n";
                    cin.clear();
                    cin.ignore(10000, '\n');
                    break;
                }
                
                goToLine(L, lineNumber);
                break;
            }
            case 12: {
                int lineNumber;
                cout << "Masukkan nomor baris yang ingin diduplikasi: ";
                cin >> lineNumber;
                
                if (cin.fail() || lineNumber <= 0 || lineNumber > length(L)) {
                    cout << "Input tidak valid. Nomor baris harus berupa angka positif dan tidak melebihi jumlah baris.\n";
                    cin.clear();
                    cin.ignore(10000, '\n');
                    break;
                }
                
                duplicateLine(L, lineNumber);
                break;
            }
            case 13:
                deleteFile(fileName, L);
                break;
            case 0:
                                cout << "\nAnda memilih untuk keluar dari program." << endl;
                                cout << "Terima kasih telah menggunakan Teks Editor Warlok Asli. Sampai jumpa!\n";
                                exit(0); // Keluar dari program secara langsung
                                break;

            default:
                cout << "Pilihan tidak valid. Silakan pilih menu antara 0-13.\n";
                break;
        }

    } while (pilihan != 0);

    return 0;
}
