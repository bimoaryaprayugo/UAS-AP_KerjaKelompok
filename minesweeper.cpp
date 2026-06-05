#include <iostream>
#include <cstdlib>
#include <ctime>

// ──────────────────────────────────────────────
//  Constants
// ──────────────────────────────────────────────
const int MAX_SIZE = 10;

// ──────────────────────────────────────────────
//  Class Game  –  handles ALL game logic & state
// ──────────────────────────────────────────────
class Game {
private:
    // --- board state ---
    int  N;                          // board dimension (N x N)
    int  totalBombs;                 // total bombs placed
    bool hasBomb [MAX_SIZE][MAX_SIZE]; // true if cell has bomb
    int  adjCount[MAX_SIZE][MAX_SIZE]; // number of adjacent bombs
    bool isOpen  [MAX_SIZE][MAX_SIZE]; // true if cell has been opened
    bool isFlagged[MAX_SIZE][MAX_SIZE];// true if cell is flagged

    // --- counters ---
    int  flagsLeft;   // bombs not yet correctly flagged
    int  safeClosed;  // safe cells still closed

    // --- timer ---
    time_t startTime;

    // ── helpers ──────────────────────────────
    bool inBounds(int r, int c) const {
        return r >= 0 && r < N && c >= 0 && c < N;
    }

    // Count adjacent bombs for every cell
    void computeAdjacency() {
        for (int r = 0; r < N; r++)
            for (int c = 0; c < N; c++) {
                int cnt = 0;
                for (int dr = -1; dr <= 1; dr++)
                    for (int dc = -1; dc <= 1; dc++)
                        if ((dr || dc) && inBounds(r+dr, c+dc) && hasBomb[r+dr][c+dc])
                            cnt++;
                adjCount[r][c] = cnt;
            }
    }

    // Flood-fill open (recursive) – opens all connected 0-adj cells
    void floodOpen(int r, int c) {
        if (!inBounds(r, c) || isOpen[r][c] || isFlagged[r][c]) return;
        isOpen[r][c] = true;
        safeClosed--;
        if (adjCount[r][c] == 0)
            for (int dr = -1; dr <= 1; dr++)
                for (int dc = -1; dc <= 1; dc++)
                    if (dr || dc) floodOpen(r+dr, c+dc);
    }

    // Reveal all bombs (called on game-over)
    void revealAllBombs() {
        for (int r = 0; r < N; r++)
            for (int c = 0; c < N; c++)
                if (hasBomb[r][c]) isOpen[r][c] = true;
    }

    // ── display ──────
    void printBoard(bool revealBombs = false) const {
        std::cout << "\n";
        // Column header
        std::cout << "     ";
        for (int c = 0; c < N; c++) {
            if (c + 1 < 10) std::cout << " ";
            std::cout << (c+1) << " ";
        }
        std::cout << "\n";
        std::cout << "    +";
        for (int c = 0; c < N; c++) std::cout << "---";
        std::cout << "+\n";

        for (int r = 0; r < N; r++) {
            if (r + 1 < 10) std::cout << " ";
            std::cout << (r+1) << "  | ";
            for (int c = 0; c < N; c++) {
                if (revealBombs && hasBomb[r][c]) {
                    std::cout << (isFlagged[r][c] ? "[*]" : " * ");
                } else if (isOpen[r][c]) {
                    int a = adjCount[r][c];
                    if (a == 0) std::cout << " . ";
                    else        std::cout << " " << a << " ";
                } else if (isFlagged[r][c]) {
                    std::cout << "[F]";
                } else {
                    std::cout << " # ";
                }
            }
            std::cout << "|\n";
        }
        std::cout << "    +";
        for (int c = 0; c < N; c++) std::cout << "---";
        std::cout << "+\n";
    }

    void printStatus() const {
        time_t now = time(nullptr);
        int elapsed = (int)difftime(now, startTime);
        std::cout << "  Bom tersisa untuk ditandai : " << flagsLeft
                  << "  |  Waktu : " << elapsed << " detik\n";
    }

    // ── input helpers ───
    // Read integer in [lo, hi] with prompt
    int readInt(const std::string& prompt, int lo, int hi) const {
        int v;
        while (true) {
            std::cout << prompt;
            if (std::cin >> v && v >= lo && v <= hi) return v;
            std::cout << "  Input tidak valid. Masukkan angka antara "
                      << lo << " dan " << hi << ".\n";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
    }

    // Read row/col coordinate (1-based), convert to 0-based
    bool readCoord(int& r, int& c) {
        std::cout << "  Masukkan baris (1-" << N << "): ";
        if (!(std::cin >> r) || r < 1 || r > N) {
            std::cout << "  Koordinat tidak valid.\n";
            std::cin.clear(); std::cin.ignore(10000, '\n');
            return false;
        }
        std::cout << "  Masukkan kolom (1-" << N << "): ";
        if (!(std::cin >> c) || c < 1 || c > N) {
            std::cout << "  Koordinat tidak valid.\n";
            std::cin.clear(); std::cin.ignore(10000, '\n');
            return false;
        }
        r--; c--;  // convert to 0-based
        return true;
    }

public:
    // ── setup ──
    void setup() {
        std::cout << "\n=== Pengaturan Permainan Baru ===\n";
        N          = readInt("  Ukuran papan (4-10): ", 4, 10);
        int maxB   = N * N - 1;
        totalBombs = readInt("  Jumlah bom (1-" + std::to_string(maxB) + "): ", 1, maxB);

        // Reset arrays
        for (int r = 0; r < N; r++)
            for (int c = 0; c < N; c++) {
                hasBomb[r][c] = isFlagged[r][c] = isOpen[r][c] = false;
                adjCount[r][c] = 0;
            }

        // Place bombs randomly
        srand((unsigned)time(nullptr));
        int placed = 0;
        while (placed < totalBombs) {
            int r = rand() % N, c = rand() % N;
            if (!hasBomb[r][c]) { hasBomb[r][c] = true; placed++; }
        }

        computeAdjacency();

        flagsLeft = totalBombs;
        safeClosed = N * N - totalBombs;
        startTime = time(nullptr);
    }

    // ── main game loop ────────────────────────
    // Returns true  → player won
    // Returns false → player lost
    bool play() {
        while (true) {
            printBoard();
            printStatus();

            // Check win condition: all bombs flagged, no flag on safe cell
            if (flagsLeft == 0) {
                // verify no flag on safe cell
                bool wrongFlag = false;
                for (int r = 0; r < N && !wrongFlag; r++)
                    for (int c = 0; c < N && !wrongFlag; c++)
                        if (isFlagged[r][c] && !hasBomb[r][c])
                            wrongFlag = true;
                if (!wrongFlag) return true;  // WIN
            }

            std::cout << "\n  Aksi: 1=Buka Kotak  2=Tandai/Hapus Tanda  0=Kembali ke Menu\n";
            int aksi = readInt("  Pilihan: ", 0, 2);

            if (aksi == 0) return false; // back to menu (treat as loss? no – just quit round)

            int r, c;
            if (!readCoord(r, c)) continue;

            if (aksi == 1) {
                // Open cell
                if (isOpen[r][c]) {
                    std::cout << "  Kotak sudah terbuka.\n";
                } else if (isFlagged[r][c]) {
                    std::cout << "  Hapus tanda dahulu sebelum membuka.\n";
                } else if (hasBomb[r][c]) {
                    // GAME OVER
                    revealAllBombs();
                    printBoard(true);
                    time_t now = time(nullptr);
                    int elapsed = (int)difftime(now, startTime);
                    std::cout << "\n  *** BOOM! Kena bom! GAME OVER ***\n";
                    std::cout << "  Waktu bermain: " << elapsed << " detik\n";
                    return false;
                } else {
                    floodOpen(r, c);
                }
            } else {
                // Flag / unflag
                if (isOpen[r][c]) {
                    std::cout << "  Tidak bisa menandai kotak yang sudah terbuka.\n";
                } else {
                    if (isFlagged[r][c]) {
                        isFlagged[r][c] = false;
                        if (hasBomb[r][c]) flagsLeft++;
                        std::cout << "  Tanda dihapus.\n";
                    } else {
                        isFlagged[r][c] = true;
                        if (hasBomb[r][c]) flagsLeft--;
                        std::cout << "  Kotak ditandai sebagai bom.\n";
                    }
                }
            }
        }
    }

    // ── win/lose screen ───────────────────────
    void showResult(bool won) {
        time_t now = time(nullptr);
        int elapsed = (int)difftime(now, startTime);
        if (won) {
            printBoard(true);
            std::cout << "\n  *** SELAMAT! Semua bom berhasil ditandai! ***\n";
        }
        std::cout << "  Waktu bermain: " << elapsed << " detik\n";
    }
};

// ──────────────────────────────────────────────
//  Main Menu
// ──────────────────────────────────────────────
int main() {
    srand((unsigned)time(nullptr));

    std::cout << "=========================================\n";
    std::cout << "          MINESWEEPER++  v1.0            \n";
    std::cout << "=========================================\n";
    std::cout << "  Legenda papan:\n";
    std::cout << "  #  = kotak tertutup\n";
    std::cout << "  .  = kotak terbuka (0 bom di sekitar)\n";
    std::cout << "  1-8= kotak terbuka (jumlah bom sekitar)\n";
    std::cout << " [F] = ditandai sebagai bom\n";
    std::cout << "  *  = bom (terungkap saat kalah/menang)\n";
    std::cout << "=========================================\n";

    Game game;

    while (true) {
        std::cout << "\n--- MENU UTAMA ---\n";
        std::cout << "  1. Mulai Permainan Baru\n";
        std::cout << "  2. Keluar\n";
        std::cout << "Pilihan: ";

        int pilihan;
        if (!(std::cin >> pilihan)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        if (pilihan == 1) {
            game.setup();
            bool won = game.play();
            game.showResult(won);
        } else if (pilihan == 2) {
            std::cout << "\n  Terima kasih telah bermain Minesweeper++!\n\n";
            break;
        } else {
            std::cout << "  Pilihan tidak valid.\n";
        }
    }

    return 0;
}
