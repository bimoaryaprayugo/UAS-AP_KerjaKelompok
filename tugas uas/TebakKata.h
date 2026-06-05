#ifndef TEBAKKATA_H
#define TEBAKKATA_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

struct KataGame
{
    string kataAsli;
    string statusTebakan;
    int nyawa;
    int skor;
    bool hintDipakai;
    char tebakanSalah[26];
    int jumlahSalah;
};

string ambilKataAcak()
{
    string daftarKata[15] =
    {
        "komputer",
        "program",
        "algoritma",
        "keyboard",
        "monitor",
        "internet",
        "database",
        "pointer",
        "struktur",
        "variabel",
        "terminal",
        "compiler",
        "fungsi",
        "operator",
        "perulangan"
    };

    int index = rand() % 15;
    return daftarKata[index];
}

void gunakanHint(KataGame *game)
{
    if(game->hintDipakai)
    {
        cout << "\nHint sudah digunakan!\n";
        return;
    }

    if(game->nyawa <= 1)
    {
        cout << "\nNyawa tidak cukup untuk menggunakan hint!\n";
        return;
    }

    char hurufPertama = game->kataAsli[0];

    for(int i = 0; i < game->kataAsli.length(); i++)
    {
        if(game->kataAsli[i] == hurufPertama)
        {
            game->statusTebakan[i] = hurufPertama;
        }
    }

    game->nyawa--;
    game->hintDipakai = true;

    cout << "\nHint digunakan! Huruf pertama terbuka.\n";
}

void pilihKesulitan(KataGame *game)
{
    int pilihan;

    cout << "===== PILIH LEVEL DONGG =====\n";
    cout << "1. Easy (8 Nyawa)\n";
    cout << "2. Medium (6 Nyawa)\n";
    cout << "3. Hard (4 Nyawa)\n";
    cout << "Pilihan : ";
    cin >> pilihan;

    switch(pilihan)
    {
        case 1:
            game->nyawa = 8;
            break;
        case 2:
            game->nyawa = 6;
            break;
        case 3:
            game->nyawa = 4;
            break;
        default:
            game->nyawa = 8;
    }
}

void bersihkanLayar()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

bool sudahDitebak(KataGame *game, char huruf)
{
    for(int i = 0; i < game->statusTebakan.length(); i++)
    {
        if(game->statusTebakan[i] == huruf)
            return true;
    }

    for(int i = 0; i < game->jumlahSalah; i++)
    {
        if(game->tebakanSalah[i] == huruf)
            return true;
    }

    return false;
}

void prosesTebakan(KataGame *game, char huruf)
{
    bool ditemukan = false;

    for(int i = 0; i < game->kataAsli.length(); i++)
    {
        if(game->kataAsli[i] == huruf)
        {
            game->statusTebakan[i] = huruf;
            ditemukan = true;
        }
    }

    if(!ditemukan)
    {
        game->tebakanSalah[game->jumlahSalah] = huruf;
        game->jumlahSalah++;
        game->nyawa--;
    }
}

bool cekMenang(KataGame *game)
{
    return game->kataAsli == game->statusTebakan;
}

void tampilkanGame(KataGame *game)
{
    cout << "\n===== GAME TEBAK KATA =====\n\n";

    cout << "Kata : ";
    for(int i = 0; i < game->statusTebakan.length(); i++)
    {
        cout << game->statusTebakan[i] << " ";
    }

    cout << "\n";
    cout << "Nyawa : " << game->nyawa << "\n";

    cout << "Huruf Salah : ";
    for(int i = 0; i < game->jumlahSalah; i++)
    {
        cout << game->tebakanSalah[i] << " ";
    }

    cout << "\n\n";
    cout << "Hint : ";

    if(game->hintDipakai)
    {
        cout << "Sudah Digunakan";
    }
    else
    {
        cout << "Tersedia";
    }

    cout << endl;
}

void hitungSkor(KataGame *game)
{
    int poinDasar;

    if(game->nyawa <= 4)
        poinDasar = 300; // Hard
    else if(game->nyawa <= 6)
        poinDasar = 200; // Medium
    else
        poinDasar = 100; // Easy

    game->skor = poinDasar + (game->nyawa * 20);
}

#endif