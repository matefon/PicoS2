#include <string> // enable strings
#include <map> // enable maps

using namespace std;

// map of codes: first is the keycode, second is the key itself (US layout probably)
map<string,string> keycodes = {
    {"76","ESC"},
    {"05","F1"},
    {"06","F2"},
    {"04","F3"},
    {"0C","F4"},
    {"03","F5"},
    {"0B","F6"},
    {"83","F7"},
    {"0A","F8"},
    {"01","F9"},
    {"09","F10"},
    {"78","F11"},
    {"07","F12"},
// printscreen and pause/break are not supported due to incompatibility with the code (pressing these keys are either interpreted as others or messes up the queue, so every read throws an error)
    //{"E012E07C","Prt Scr"},
    {"7E","Scroll Lock"},
    //{"E11477E1F014E077","Pause/Break"},
    {"0E","`"},
    {"16","1"},
    {"1E","2"},
    {"26","3"},
    {"25","4"},
    {"2E","5"},
    {"36","6"},
    {"3D","7"},
    {"3E","8"},
    {"46","9"},
    {"45","0"},
    {"4E","-"},
    {"55","="},
    {"66","Backspace"},
    {"0D","Tab"},
    {"15","Q"},
    {"1D","W"},
    {"24","E"},
    {"2D","R"},
    {"2C","T"},
    {"35","Y"},
    {"3C","U"},
    {"43","I"},
    {"44","O"},
    {"4D","P"},
    {"54","["},
    {"5B","]"},
    {"5D","\\"},
    {"58","Caps Lock"},
    {"1C","A"},
    {"1B","S"},
    {"23","D"},
    {"2B","F"},
    {"34","G"},
    {"33","H"},
    {"3B","J"},
    {"42","K"},
    {"4B","L"},
    {"4C",";"},
    {"52","'"},
    {"5A","Enter"},
    {"12","Shift (Left)"},
    {"1A","Z"},
    {"22","X"},
    {"21","C"},
    {"2A","V"},
    {"32","B"},
    {"31","N"},
    {"3A","M"},
    {"41",","},
    {"49","."},
    {"4A","/"},
    {"59","Shift (Right)"},
    {"14","Ctrl (left)"},
    {"E01F","Windows (left)"},
    {"11","Alt (left)"},
    {"29","Spacebar"},
    {"E011","Alt (right)"},
    {"E027","Windows (right)"},
    {"E02F","Menus"},
    {"E014","Ctrl (right)"},
    {"E070","Insert"},
    {"E06C","Home"},
    {"E07D","Page Up"},
    {"E071","Delete"},
    {"E069","End"},
    {"E07A","Page Down"},
    {"E075","Up Arrow"},
    {"E06B","Left Arrow"},
    {"E072","Down Arrow"},
    {"E074","Right Arrow"},
    {"77","Num Lock"},
    {"E04A","Num /"},
    {"7C","Num *"},
    {"7B","Num -"},
    {"6C","Num 7"},
    {"75","Num 8"},
    {"7D","Num 9"},
    {"79","Num +"},
    {"6B","Num 4"},
    {"73","Num 5"},
    {"74","Num 6"},
    {"69","Num 1"},
    {"72","Num 2"},
    {"7A","Num 3"},
    {"70","Num 0"},
    {"71","Num ."},
    {"E05A","Num Enter"},
    {"F0","rel"}, // release
    {"E0","spec"}, // special (e.g. arrow keys)
    {"E0F0","spec_rel"} // releasing special key
};

// binary to hex converter (custom made to convert 2x4 bits separately)
string bintohex(int arr[]){
    int sum1 = 0, sum2 = 0;
    string hex1 = "", hex2 = "";
    for (int i = 0; i < 8; i++){
        if (arr[i] == 1){
            switch(i){
                case 0:sum1 += 8;break;
                case 1:sum1 += 4;break;
                case 2:sum1 += 2;break;
                case 3:sum1 += 1;break;
                case 4:sum2 += 8;break;
                case 5:sum2 += 4;break;
                case 6:sum2 += 2;break;
                case 7:sum2 += 1;break;
            }
        }
    }
    if (sum1 > 9){
        switch(sum1){
            case 10:hex1 = "A";break;
            case 11:hex1 = "B";break;
            case 12:hex1 = "C";break;
            case 13:hex1 = "D";break;
            case 14:hex1 = "E";break;
            case 15:hex1 = "F";break;
        }
    } else {hex1 = to_string(sum1);}
    if (sum2 > 9){
        switch(sum2){
            case 10:hex2 = "A";break;
            case 11:hex2 = "B";break;
            case 12:hex2 = "C";break;
            case 13:hex2 = "D";break;
            case 14:hex2 = "E";break;
            case 15:hex2 = "F";break;
        }
    } else {hex2 = to_string(sum2);}
    return hex1 + hex2;
}

// keycode getter
string getkey(int input[], bool spec){
// convert input array containing ones and zeros to hex
    string keycode = bintohex(input);
// if special is true, then search for E0 and keycode
    if (spec){keycode = "E0" + keycode;}
// search for keycode in map and return its name
    string output = keycodes.at(keycode);
    return output;
}
