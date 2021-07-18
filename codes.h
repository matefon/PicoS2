#include <string>
#include <map>

using namespace std;

map<string,string> keycodes = { // First is the keycode, second is the TinyUSB send code
    {"1C","A"},
    {"F0","release"},
    {"E0","special"}
};

string bintohex(int arr[]){
    int sum1 = 0, sum2 = 0;
    string hex1 = "", hex2 = "";

    for (int i = 0; i < 8; i++){
        if (arr[i] == 1){
            switch(i){
                case 0:
                    sum1 += 8;
                    break;
                case 1:
                    sum1 += 4;
                    break;
                case 2:
                    sum1 += 2;
                    break;
                case 3:
                    sum1 += 1;
                    break;

                case 4:
                    sum2 += 8;
                    break;
                case 5:
                    sum2 += 4;
                    break;
                case 6:
                    sum2 += 2;
                    break;
                case 7:
                    sum2 += 1;
                    break;
            }
        }
    }
    if (sum1 > 9){
        switch(sum1){
            case 10:
                hex1 = "A";
                break;
            case 11:
                hex1 = "B";
                break;
            case 12:
                hex1 = "C";
                break;
            case 13:
                hex1 = "D";
                break;
            case 14:
                hex1 = "E";
                break;
            case 15:
                hex1 = "F";
                break;
        }
    } else {
        hex1 = to_string(sum1);
    }
    if (sum2 > 9){
        switch(sum2){
            case 10:
                hex2 = "A";
                break;
            case 11:
                hex2 = "B";
                break;
            case 12:
                hex2 = "C";
                break;
            case 13:
                hex2 = "D";
                break;
            case 14:
                hex2 = "E";
                break;
            case 15:
                hex2 = "F";
                break;
        }
    } else {
        hex2 = to_string(sum2);
    }
    return hex1 + hex2;
}

string getkeycode(int input[]){
    string output = keycodes.at(bintohex(input));
    return output;
}