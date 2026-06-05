#include <iostream>
#include <vector>
#include <string>
using namespace std;

const int KAPASITAS = 20;
const int TARIF_PER_JAM = 5000;

struct Petugas {
    string nama;
    string id;
};

struct Kendaraan {
    string plat;
    string merk;
    string warna;
};

void garis() {
    cout << "==========================================" << endl;
}

void inputPetugas(Petugas *p) {
    cout << "Masukkan nama petugas : ";
    getline(cin, p->nama);
    cout << "Masukkan ID petugas   : ";
    getline(cin, p->id);
}

void tampilPetugas(Petugas p) {
    cout << "Nama Petugas : " << p.nama << endl;
    cout << "ID Petugas   : " << p.id << endl;
}

bool parkiranPenuh(vector<Kendaraan> mobil) {
    return mobil.size() >= KAPASITAS;
}

void tambahKendaraan(vector<Kendaraan> *mobil) {
    if (parkiranPenuh(*mobil)) {
        cout << "\nMaaf, parkiran sudah penuh!" << endl;
        return;
    }

    Kendaraan k;

    cout << "\nMasukkan plat mobil  : ";
    getline(cin, k.plat);
    cout << "Masukkan merk mobil  : ";
    getline(cin, k.merk);
    cout << "Masukkan warna mobil : ";
    getline(cin, k.warna);
    mobil->push_back(k);

    cout << "\nKendaraan berhasil ditambahkan." << endl;
    cout << "Jumlah kendaraan sekarang: " << mobil->size() << endl;
}

void tampilKendaraan(vector<Kendaraan> mobil) {
    if (mobil.empty()) {
        cout << "\nBelum ada kendaraan yang parkir." << endl;
        return;
    }

    cout << "\nDAFTAR KENDARAAN YANG SEDANG PARKIR" << endl;
    garis();
    for (int i = 0; i < mobil.size(); i++) {
        cout << "Mobil ke-" << i + 1 << endl;
        cout << "Plat  : " << mobil[i].plat << endl;
        cout << "Merk  : " << mobil[i].merk << endl;
        cout << "Warna : " << mobil[i].warna << endl;
        garis();
    }
}

void pembayaran(vector<Kendaraan> *mobil) {
    if (mobil->empty()) {
        cout << "\nBelum ada kendaraan yang parkir." << endl;
        return;
    }

    string platCari;
    bool ditemukan = false;
    cout << "\nMasukkan plat mobil yang ingin dibayar: ";
    getline(cin, platCari);

    for (int i = 0; i < mobil->size(); i++) {
        if ((*mobil)[i].plat == platCari) {
            ditemukan = true;

            string inputLama;
            int lama;

            cout << "Masukkan lama parkir dalam jam: ";
            getline(cin, inputLama);

            lama = stoi(inputLama);

            if (lama <= 0) {
                lama = 1;
            }

            int biaya = lama * TARIF_PER_JAM;

            cout << "\nPLAT DITEMUKAN" << endl;
            cout << "Plat        : " << (*mobil)[i].plat << endl;
            cout << "Merk        : " << (*mobil)[i].merk << endl;
            cout << "Warna       : " << (*mobil)[i].warna << endl;
            cout << "Lama Parkir : " << lama << " jam" << endl;
            cout << "Biaya       : Rp" << biaya << endl;
            mobil->erase(mobil->begin() + i);

            cout << "\nPembayaran berhasil. Mobil keluar dari parkiran." << endl;
            break;
        }
    }

    if (!ditemukan) {
        cout << "\nPlat mobil tidak ditemukan." << endl;
    }
}

void menu() {
    cout << "\nMENU PARKIR MALL PRIENAL" << endl;
    garis();
    cout << "1. Tambah Kendaraan" << endl;
    cout << "2. Tampilkan Kendaraan" << endl;
    cout << "3. Pembayaran" << endl;
    cout << "4. Keluar" << endl;
    cout << "Pilih menu: ";
}

int main() {
    Petugas petugas;
    vector<Kendaraan> daftarMobil;
    string inputPilihan;
    int pilihan = 0;
    garis();
    cout << "SELAMAT DATANG DI PARKIR MALL PRIENAL" << endl;
    garis();

    inputPetugas(&petugas);

    do {
        cout << endl;
        garis();
        tampilPetugas(petugas);
        cout << "Slot parkir: " << daftarMobil.size() << "/" << KAPASITAS << endl;

        if (parkiranPenuh(daftarMobil)) {
            cout << "Pemberitahuan: Parkiran penuh!" << endl;
        }

        menu();
        getline(cin, inputPilihan);

        if (inputPilihan == "1") {
            pilihan = 1;
        } else if (inputPilihan == "2") {
            pilihan = 2;
        } else if (inputPilihan == "3") {
            pilihan = 3;
        } else if (inputPilihan == "4") {
            pilihan = 4;
        } else {
            pilihan = 0;
        }

        switch (pilihan) {
            case 1:
                tambahKendaraan(&daftarMobil);
                break;
            case 2:
                tampilKendaraan(daftarMobil);
                break;
            case 3:
                pembayaran(&daftarMobil);
                break;
            case 4:
                cout << "\nTerima kasih. Program selesai." << endl;
                break;
            default:
                cout << "\nPilihan tidak valid." << endl;
        }

    } while (pilihan != 4);

    return 0;
}