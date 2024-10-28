# Ambawick
```
                    _                    _      _    
    /\             | |                  (_)    | |   
   /  \   _ __ ___ | |__   __ ___      ___  ___| | __
  / /\ \ | '_ ` _ \| '_ \ / _` \ \ /\ / / |/ __| |/ /
 / ____ \| | | | | | |_) | (_| |\ V  V /| | (__|   < 
/_/    \_\_| |_| |_|_.__/ \__,_| \_/\_/ |_|\___|_|\_\
======================================================
```
Ambawick is a turn based game made with C that runs in your terminal.

# Compilation

Just compile `game.c` file with your favorite compiler
### GCC
`gcc -o game game.c`
### Microsoft Visual Compiler
`cl game.c`

## For OS other than Windows
If you check on `game.c`	 at function `ClearScreen` it depends on your system, so if you're on windows or `_WIN32` is defined, `system("cls")` will be used, else or `_WIN32` is not defined, `system("clear")`.

## GameData
Game datas are stored inside `GameData` folder, it contains 3 files and you can modify it as you like using this format:
### GameData/Characters.txt
Format: `PlayerName,Health,Attack,Defense,Price`
### GameData/Weapons.txt
Format: `WeaponName,Damage,Price`
### GameData/Defenses.txt
Format: `DefenseName,Value,Price`


## GameEtc
GameEtc contains stuff that is going to be shown to user such as logo and credits.

## Credits
Thanks to all of my team working on this project, they're very talented individuals.
* Muhammad Daffa Rahman (L0124062)
* Phyrurizqi Altiano Firdauzan (L0124069)
* Resyad Agung Wicaksono (L0124072)
* Raditya Adi Pradana (L0124070)
* Daffa Arkan Taqiya (L0124047)
