#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

const int MAX_SIZE = 10;

class Game {
private:
    int N;
    int totalBombs;
    bool hasBomb[MAX_SIZE][MAX_SIZE];
    int adjCount[MAX_SIZE][MAX_SIZE];
    bool isOpen[MAX_SIZE][MAX_SIZE];
    bool isFlagged[MAX_SIZE][MAX_SIZE];

    int flagsLeft;
    int safeClosed;

    time_t startTime;

    bool inBounds(int r, int c) const {
        return r >= 0 && r < N && c >= 0 && c < N;
    }

    void computeAdjacency() {
        for (int r = 0; r < N; r++) {
            for (int c = 0; c < N; c++) {
                int cnt = 0;
                for (int dr = -1; dr <= 1; dr++) {
                    for (int dc = -1; dc <= 1; dc++) {
                        if ((dr || dc) && inBounds(r + dr, c + dc) && hasBomb[r + dr][c + dc]) {
                            cnt++;
                        }
                    }
                }
                adjCount[r][c] = cnt;
            }
        }
    }

    void floodOpen(int r, int c) {
        if (!inBounds(r, c) || isOpen[r][c] || isFlagged[r][c]) return;

        isOpen[r][c] = true;
        safeClosed--;

        if (adjCount[r][c] == 0) {
            for (int dr = -1; dr <= 1; dr++) {
                for (int dc = -1; dc <= 1; dc++) {
                    if (dr || dc) floodOpen(r + dr, c + dc);
                }
            }
        }
    }

    void revealAllBombs() {
        for (int r = 0; r < N; r++) {
            for (int c = 0; c < N; c++) {
                if (hasBomb[r][c]) isOpen[r][c] = true;
            }
        }
    }

    void printBoard(bool revealBombs = false) const {
        cout << "\n";

        cout << "     ";
        for (int c = 0; c < N; c++) {
            if (c + 1 < 10) cout << " ";
            cout << (c + 1) << " ";
        }
        cout << "\n";

        cout << "    +";
        for (int c = 0; c < N; c++) cout << "---";
        cout << "+\n";

        for (int r = 0; r < N; r++) {
            if (r + 1 < 10) cout << " ";
            cout << (r + 1) << "  | ";

            for (int c = 0; c < N; c++) {
                if (revealBombs && hasBomb[r][c]) {
                    cout << (isFlagged[r][c] ? "[*]" : " * ");
                } else if (isOpen[r][c]) {
                    int a = adjCount[r][c];
                    if (a == 0) cout << " . ";
                    else cout << " " << a << " ";
                } else if (isFlagged[r][c]) {
                    cout << "[F]";
                } else {
                    cout << " # ";
                }
            }
            cout << "|\n";
        }

        cout << "    +";
        for (int c = 0; c < N; c++) cout << "---";
        cout << "+\n";
    }

    void printStatus() const {
        time_t now = time(nullptr);
        int elapsed = (int)difftime(now, startTime);
        cout << "  Bom tersisa untuk ditandai : " << flagsLeft
             << "  |  Waktu : " << elapsed << " detik\n";
    }

    int readInt(const string& prompt, int lo, int hi) const {
        int v;
        while (true) {
            cout << prompt;
            if (cin >> v && v >= lo && v <= hi) return v;

            cout << "  Input tidak valid. Masukkan angka antara "
                 << lo << " dan " << hi << ".\n";
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }

    bool readCoord(int& r, int& c) {
        cout << "  Masukkan baris (1-" << N << "): ";
        if (!(cin >> r) || r < 1 || r > N) {
            cout << "  Koordinat tidak valid.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            return false;
        }

        cout << "  Masukkan kolom (1-" << N << "): ";
        if (!(cin >> c) || c < 1 || c > N) {
            cout << "  Koordinat tidak valid.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            return false;
        }

        r--;
        c--;
        return true;
    }

public:
    void setup() {
        cout << "\n=== Pengaturan Permainan Baru ===\n";
        N = readInt("  Ukuran papan (4-10): ", 4, 10);
        int maxB = N * N - 1;
        totalBombs = readInt("  Jumlah bom (1-" + to_string(maxB) + "): ", 1, maxB);

        for (int r = 0; r < N; r++) {
            for (int c = 0; c < N; c++) {
                hasBomb[r][c] = false;
                isFlagged[r][c] = false;
                isOpen[r][c] = false;
                adjCount[r][c] = 0;
            }
        }

        srand((unsigned)time(nullptr));
        int placed = 0;
        while (placed < totalBombs) {
            int r = rand() % N;
            int c = rand() % N;
            if (!hasBomb[r][c]) {
                hasBomb[r][c] = true;
                placed++;
            }
        }

        computeAdjacency();

        flagsLeft = totalBombs;
        safeClosed = N * N - totalBombs;
        startTime = time(nullptr);
    }

    bool play() {
        while (true) {
            printBoard();
            printStatus();

            if (flagsLeft == 0) {
                bool wrongFlag = false;
                for (int r = 0; r < N && !wrongFlag; r++) {
                    for (int c = 0; c < N && !wrongFlag; c++) {
                        if (isFlagged[r][c] && !hasBomb[r][c]) {
                            wrongFlag = true;
                        }
                    }
                }
                if (!wrongFlag) return true;
            }

            cout << "\n  Aksi: 1=Buka Kotak  2=Tandai/Hapus Tanda  0=Kembali ke Menu\n";
            int aksi = readInt("  Pilihan: ", 0, 2);

            if (aksi == 0) return false;

            int r, c;
            if (!readCoord(r, c)) continue;

            if (aksi == 1) {
                if (isOpen[r][c]) {
                    cout << "  Kotak sudah terbuka.\n";
                } else if (isFlagged[r][c]) {
                    cout << "  Hapus tanda dahulu sebelum membuka.\n";
                } else if (hasBomb[r][c]) {
                    revealAllBombs();
                    printBoard(true);
                    time_t now = time(nullptr);
                    int elapsed = (int)difftime(now, startTime);
                    cout << "\n  *** BOOM! Kena bom! GAME OVER ***\n";
                    cout << "  Waktu bermain: " << elapsed << " detik\n";
                    return false;
                } else {
                    floodOpen(r, c);
                }
            } else {
                if (isOpen[r][c]) {
                    cout << "  Tidak bisa menandai kotak yang sudah terbuka.\n";
                } else {
                    if (isFlagged[r][c]) {
                        isFlagged[r][c] = false;
                        if (hasBomb[r][c]) flagsLeft++;
                        cout << "  Tanda dihapus.\n";
                    } else {
                        isFlagged[r][c] = true;
                        if (hasBomb[r][c]) flagsLeft--;
                        cout << "  Kotak ditandai sebagai bom.\n";
                    }
                }
            }
        }
    }

    void showResult(bool won) {
        time_t now = time(nullptr);
        int elapsed = (int)difftime(now, startTime);
        if (won) {
            printBoard(true);
            cout << "\n  *** SELAMAT! Semua bom berhasil ditandai! ***\n";
        }
        cout << "  Waktu bermain: " << elapsed << " detik\n";
    }
};

int main() {
    srand((unsigned)time(nullptr));

    cout << "=========================================\n";
    cout << "          MINESWEEPER++  v1.0            \n";
    cout << "=========================================\n";
    cout << "  Legenda papan:\n";
    cout << "  #  = kotak tertutup\n";
    cout << "  .  = kotak terbuka (0 bom di sekitar)\n";
    cout << "  1-8= kotak terbuka (jumlah bom sekitar)\n";
    cout << " [F] = ditandai sebagai bom\n";
    cout << "  *  = bom (terungkap saat kalah/menang)\n";
    cout << "=========================================\n";

    Game game;

    while (true) {
        cout << "\n--- MENU UTAMA ---\n";
        cout << "  1. Mulai Permainan Baru\n";
        cout << "  2. Keluar\n";
        cout << "Pilihan: ";

        int pilihan;
        if (!(cin >> pilihan)) {
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        if (pilihan == 1) {
            game.setup();
            bool won = game.play();
            game.showResult(won);
        } else if (pilihan == 2) {
            cout << "\n  Terima kasih telah bermain Minesweeper++!\n\n";
            break;
        } else {
            cout << "  Pilihan tidak valid.\n";
        }
    }

    return 0;
}
