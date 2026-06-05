#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cstdlib>

using namespace std;


string keUppercase(string str) {
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

string buatTebakan(string negara) {
    string hasil = negara;
    int panjang = negara.length();
    
    int jumlahSensor = panjang / 2; 
    if (jumlahSensor == 0) jumlahSensor = 1; 

    int tersembunyi = 0;
    while (tersembunyi < jumlahSensor) {
        int indexAcak = rand() % panjang;
        
        if (hasil[indexAcak] != '_' && hasil[indexAcak] != ' ') {
            hasil[indexAcak] = '_';
            tersembunyi++;
        }
    }
    
    string tampilanRapi = "";
    for (int i = 0; i < hasil.length(); i++) {
        tampilanRapi += hasil[i];
        if (i != hasil.length() - 1) {
            tampilanRapi += " ";
        }
    }
    
    return tampilanRapi;
}

int main() {
    srand(time(0));

    vector<string> daftarNegara = {
        // Asia Tenggara
        "INDONESIA", "MALAYSIA", "SINGAPURA", "THAILAND", "FILIPINA", 
        "VIETNAM", "BRUNEI", "KAMBOJA", "LAOS", "MYANMAR", "TIMORLESTE",
        
        // Asia Lainnya
        "JEPANG", "KOREA", "CHINA", "INDIA", "ARABSAUDI", "PALESTINA", 
        "TURKI", "IRAN", "IRAQ", "PAKISTAN", "QATAR", "MALADEWA",
        
        // Eropa
        "JERMAN", "PERANCIS", "INGGRIS", "ITALIA", "SPANYOL", "BELANDA", 
        "RUSIA", "PORTUGAL", "SWEDIA", "SWISS", "YUNANI", "BELGIA",
        
        // Amerika
        "BRAZIL", "ARGENTINA", "MEKSIKO", "KANADA", "KOLOMBIA", "CHILE", 
        "PERU", "URUGUAY", "KUBA", "JAMAIKA", "AMERIKA",
        
        // Afrika
        "MESIR", "MAROKO", "AFRIKASELATAN", "NIGERIA", "GHANA", "KENYA", 
        "ALGERIA", "TUNISIA", "SENEGAL", "KAMERUN",
        
        // Oseania
        "AUSTRALIA", "SELANDIABARU", "FIJI", "SAMOA"
    };

    int poin = 0;
    char pilihanBermain;

    do {
        int indexNegara = rand() % daftarNegara.size();
        string negaraAsli = daftarNegara[indexNegara];
        
        string negaraDisensor = buatTebakan(negaraAsli);


        cout << "\nTebak Negara: " << negaraDisensor << endl;
        cout << "Negara apakah yang dimaksud ? ";
        
        string jawabanUser;
        cin >> jawabanUser;

        jawabanUser = keUppercase(jawabanUser);

        if (jawabanUser == negaraAsli) {
            poin++;
            cout << "Selamat! Tebakanmu BENAR." << endl;
        } else {
            cout << "Maaf, Tebakanmu SALAH. Yang benar adalah: " << negaraAsli << endl;
        }

        cout << "Total Poin Kamu: " << poin << endl;

        cout << "\nIngin bermain lagi ? (y/n): ";
        cin >> pilihanBermain;
        pilihanBermain = tolower(pilihanBermain);

    } while (pilihanBermain == 'y');

    cout << "Terima kasih telah bermain! Poin akhirmu: " << poin << endl;

    return 0;
}