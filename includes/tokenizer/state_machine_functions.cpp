#include "state_machine_functions.h"

//Fill all cells of the array with -1
void init_table(int _table[][MAX_COLUMNS]){
    for (int i = 0; i < MAX_ROWS; i++){
        for (int j = 0; j < MAX_COLUMNS; j++){
            _table[i][j] = -1;
        }
    }
}

//Mark this state (row) with a 1 (success)
void mark_success(int _table[][MAX_COLUMNS], int state){
    _table[state][0] = 1;
}

//Mark this state (row) with a 0 (fail)
void mark_fail(int _table[][MAX_COLUMNS], int state){
    _table[state][0] = 0;
}

//true if state is a success state
bool is_success(int _table[][MAX_COLUMNS], int state){
    return _table[state][0] == 1;
}

//Mark a range of cells in the array. 
void mark_cells(int row, int _table[][MAX_COLUMNS], int from, int to, int state){
    for (int i = from; i <= to; i++){
        _table[row][i] = state;
    }
}

//Mark columns represented by the string columns[] for this row
void mark_cells(int row, int _table[][MAX_COLUMNS], const char columns[], int state){
    int pos = 0;
    while (columns[pos] != '\0'){
        _table[row][columns[pos]] = state;
        pos++;
    }
}

//Mark this row and column
void mark_cell(int row, int table[][MAX_COLUMNS], int column, int state){
    table[row][column] = state;
}

//This can realistically be used on a small table
void print_table(int _table[][MAX_COLUMNS]){
    for (int i = 0; i < MAX_ROWS; i++){
        for (int j = 0; j < MAX_COLUMNS; j++){
            cout << setw(3) << _table[i][j] << " ";
        }
        cout << endl;
    }
}

//show string s and mark this position on the string:
//hello world   pos: 7
//       ^
void show_string(char s[], int _pos){
    string marker; 
    //cout << "marker is " << marker << endl;
    int pos = 0;
    while(pos != _pos){
        marker+= " ";
        pos++;
    }
    marker+= "^";
    cout << s << "   " << "pos: " << _pos << endl;
    cout << marker << endl;
}

bool getToken(char input[], int& pos, int& state, int table[MAX_ROWS][MAX_COLUMNS], string& t) {
    string temp;
    while (input[pos] != '\0') {
        char current_input = input[pos]; 
        // if (current_input < 0){
        //     t += current_input;
        //     state = TOKEN_UNKNOWN;
        //     pos++;
        // }
        // else{
        state = table[state][current_input]; 
        if (state != -1){
            temp += current_input;
            if (is_success(table, state)){
                t = temp;
            }
            else{
                if (current_input == '.' && !isdigit(input[pos+1])){
                    temp.pop_back(); //gets rid of the period
                    t = temp;
                    return true;
                }
            }
        } 
        else{ //if state is -1, return t as it is n dont increment pos
            return true;  
        } 
        pos++;
        // }
    }

    if (temp.empty()){
        return false;
    }
    return true; //if temp isnt empty then itll return true
}






// bool getToken(char input[], int& pos, int& state, int table[MAX_ROWS][MAX_COLUMNS], string& t) {
//     string temp;
//     while (input[pos] != '\0') {
//         char current_input = input[pos]; 
//         state = table[state][current_input];
//         if (state != -1){
//             temp += current_input;
//             if (is_success(table, state)){
//                 t = temp;
//                 if (isdigit(current_input) && !isalnum(input[pos+1]) && !isdigit(input[pos+2])){
//                     pos++;
//                     return true;
//                 }
//                 // if (!isalnum(current_input) && !isalnum(input[pos+1])){ //separates puncs
//                 //     pos++;
//                 //     return true;
//                 // }
//                  pos++;
//             }
//             else{
//                  pos++;
//             }
//         } 
//         else{
//             // pos++;
//             return true;  
             
//         }
    
        
//     }
//     if (temp.empty()){
//         return false;
//     }
//     return true; //if temp isnt empty then itll return true
// }

