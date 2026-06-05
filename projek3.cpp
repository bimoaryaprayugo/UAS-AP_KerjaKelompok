#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

struct barang {
    string kodeBarang;
    string namaBarang;
    double harga;
    int stok;
};

vector<barang> inventaris;

void tambahBarang() {
    barang b;

    cout << "\nKode Barang : ";
    cin >> b.kodeBarang;

    for (const auto &barang : inventaris) {
        if (barang.kodeBarang == b.kodeBarang) {
            cout << "Kode barang sudah digunakan!" << endl;
            return;
        }
    }

    cin.ignore();
    cout << "Nama Barang : ";
    getline(cin, b.namaBarang);

    do {
        cout << "Harga : "; cin >> b.harga;
        if (b.harga < 0)
            cout << "Harga tidak boleh negatif" << endl;
    } while (b.harga < 0);

    do {
        cout << "Stok : "; cin >> b.stok;
        if (b.stok < 0)
            cout << "Stok tidak boleh negatif" << endl;
    } while (b.stok < 0);

    inventaris.push_back(b);
    cout << "Barang berhasil ditambahkan" << endl;
}

void tampilkanBarang() {
    if (inventaris.empty()) {
        cout << "\nData barang kosong\n";
        return;
    }

    cout << "\n=======================================================\n";
    cout << left << setw(12) << "Kode" 
         << setw(20) << "Nama" 
         << setw(12) << "Harga" 
         << setw(8) << "Stok" << endl;
    cout << "=======================================================\n";

    for (const auto &b : inventaris) {
        cout << left << setw(12) << b.kodeBarang 
             << setw(20) << b.namaBarang 
             << setw(12) << b.harga 
             << setw(8) << b.stok << endl;
    }
}

void barangTermahal() {
    if (inventaris.empty()) {
        cout << "\nData barang kosong\n";
        return;
    }

    int idx = 0;

    for (int i = 1; i < inventaris.size(); i++) {
        if (inventaris[i].harga > inventaris[idx].harga) {
            idx = i;
        }
    }

    cout << "\nBarang Termahal\n";
    cout << "Kode      : " << inventaris[idx].kodeBarang << endl;
    cout << "Nama      : " << inventaris[idx].namaBarang << endl;
    cout << "Harga     : " << inventaris[idx].harga << endl;
    cout << "Stok      : " << inventaris[idx].stok << endl;
}

void totalInventaris() {
    double total = 0;

    for (const auto &b : inventaris) {
        total += b.harga * b.stok;
    }
    cout << "\nTotal Nilai Inventaris = Rp " << total << endl;
}

void cekRestock() {
    bool ada = false;

    cout << "\n=== Barang dengan stok < 5 ===\n";

    for (const auto &b : inventaris) {
        if (b.stok < 5){
            cout << "- " << b.namaBarang 
                 << " (Stok : " << b.stok << ")\n";
            ada = true;
        }
    }

    if (!ada) {
        cout << "Tidak ada barang yang perlu restock\n";
    }
}

void updateStok() {
    string kode;
    cout << "\nMasukkan kode barang : ";
    cin >> kode;

    for (auto &b : inventaris) {
        if (b.kodeBarang == kode) {
            int *ptrStok = &b.stok;

            int perubahan;
            cout << "Masukkan perubahan stok (+/-) : ";
            cin >> perubahan;

            if((*ptrStok + perubahan) < 0) {
                cout << "Stok tidak boleh negatif\n";
                return;
            }
            
            *ptrStok += perubahan;

            cout << "Stok berhasil diperbarui\n";
            cout << "Stok sekarang : " << *ptrStok << endl;
            return;
        }
    }

    cout << "Barang tidak ditemukan\n";
}

int main() {
    int pilihan;

    do {
        cout << "\n==== SISTEM MANAJEMEN INVENTARIS ====\n";
        cout << "1. Tambah Barang" << endl;
        cout << "2. Tampilkan Semua Barang" << endl;
        cout << "3. Cari Barang Termahal" << endl;
        cout << "4. Hitung Total Nilai Inventaris" << endl;
        cout << "5. Cek Restock" << endl;
        cout << "6. Update Stok" << endl;
        cout << "7. Keluar" << endl;
        cout << "Pilih menu : ";
        cin >> pilihan;

        switch (pilihan) {
            case 1: tambahBarang(); system("pause"); system("cls"); break;
            case 2: tampilkanBarang();  system("pause"); system("cls"); break;
            case 3: barangTermahal(); system("pause"); system("cls"); break;
            case 4: totalInventaris(); system("pause"); system("cls"); break;
            case 5: cekRestock(); system("pause"); system("cls"); break; 
            case 6: updateStok(); system("pause"); system("cls"); break;
            case 7: cout << "Program selesai\n"; system("pause"); system("cls"); break;
            default: cout << "Pilihan Tidak Valid!\n"; system("pause"); system("cls");
        }
    } while (pilihan != 7);
}