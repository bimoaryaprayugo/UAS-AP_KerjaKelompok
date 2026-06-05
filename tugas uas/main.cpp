#include "TebakKata.h"

int main()
{
    srand(time(0));

    char mainLagi;

    do
    {
        KataGame game;

        game.kataAsli = ambilKataAcak();

        game.statusTebakan = "";

        for(int i = 0; i < game.kataAsli.length(); i++)
        {
            game.statusTebakan += '_';
        }

        game.jumlahSalah = 0;
        game.skor = 0;
        game.hintDipakai = false;

        pilihKesulitan(&game);

        while(game.nyawa > 0 && !cekMenang(&game))
        {
            bersihkanLayar();

            tampilkanGame(&game);

            string input;

            cout << "Masukkan huruf (atau ketik hint): ";
            cin >> input;

            if(input == "HINT" || input == "hint")
            {
                gunakanHint(&game);

                cout << "\nTekankan dulu Enter...";
                cin.ignore();
                cin.get();

                continue;
            }
            char huruf = input[0];
            if(sudahDitebak(&game, huruf))
            {
                cout << "\nHuruf udah pernah ditebak!\n";
                cout << "Tekan Enter untuk lanjut...";
                cin.ignore();
                cin.get();
                continue;
            }

            prosesTebakan(&game, huruf);
        }

        bersihkanLayar();

        tampilkanGame(&game);

        if(cekMenang(&game))
    {
        hitungSkor(&game);

        cout << "Yeyyy kamu menangg!\n";
        cout << "Skor Kamu : "
             << game.skor << endl;
    }
    else
    {
        cout << "Oalah kalah pulak!\n";
        cout << "Kata Rahasia : "
             << game.kataAsli << endl;
    }

        cout << "\nMain lagi gak? (Y/N) : ";
        cin >> mainLagi;

    } while(mainLagi == 'Y' || mainLagi == 'y');

    cout << "\nTerima kasih telah bermain!\n";

    return 0;
}