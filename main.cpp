#include <iostream>
using namespace std;

#define BYTE_SIZE 8

#define MAIN_MEMORY_SIZE 128 // No. of Blocks in Main Memory
#define CACHE_MEMORY_SIZE 8 // No. of Blocks in Cache Memory
// Total 256 Words = 1024 Bytes of Main Memory
// Let Cache can have 16 Words = 64 Bytes of Cache Memory
#define WORD_SIZE 4 // Each Word have 4 Bytes of Memory
#define BLOCK_SIZE 2 // Each Block will have 2 Bytes of Memory


typedef int BIT;
typedef BIT BYTE[8];
typedef BYTE WORD[WORD_SIZE];
typedef WORD BLOCK[BLOCK_SIZE];


BLOCK MAIN[MAIN_MEMORY_SIZE] = {0};
BLOCK CACHE[CACHE_MEMORY_SIZE];



// Last Parameter named 'choice' will be used to identify to where Data is to be Inserted
// if choice == 0, then Main Memory
// elsse Cache Memory
void insert(int new_bit_data, int block = 0, int word = 0, int byte = 0, int bit = 0, int choice = 0){
    if(choice == 0){
        MAIN[block][word][byte][bit] = new_bit_data;
    }else{
        CACHE[block][word][byte][bit] = new_bit_data;
    }
}



// choice = 0, means, Data Transfer will be from MAIN_MEMORY to CACHE_MEMORY
// choice = 1, means, Data Transfer will be from CACHE_MEMORY to MAIN_MEMORY
// ##### NOTE - No Matter we transfer data from AnyWhere to Anywhere, whole Block should be transfered. #####

// [ No Issues of Bandwidth will be there while Tranferring as whole word will be get transferred ]
void transfer(int main_block,int main_word,int cache_block, int cache_word,int choice = 0){

    if(choice == 0){
        // Main => Cache Memory
        for(int wo = 0; wo < BLOCK_SIZE; wo++){
            for(int by = 0; by < WORD_SIZE; by++){
                for(int bi = 0;  bi < BYTE_SIZE ; bi++){

                    CACHE[cache_block][wo][by][bi] = MAIN[main_block][wo][by][bi];

                }
            }
        }

    }else{
        // Cache ==> Main_Memory
        for(int wo = 0; wo < BLOCK_SIZE; wo++){
            for(int by = 0; by < WORD_SIZE; by++){
                for(int bi = 0;  bi < BYTE_SIZE ; bi++){

                    MAIN[main_block][wo][by][bi] = CACHE[cache_block][wo][by][bi];
                }
            }
        }

    }


}



// This Functions Accepts Parameter, how many Blocks of Main Memory we want to see.
// Default parameters are used as 1, if function is called without parameter loop will not execute, and no output will be generated
// So Make Sure to pass some no.
void see_cacheMemory(int to_count = 1){
    if(to_count <= 8){
        // TO Ensure Input Number is With in Limits

        cout<<"\n\t\t CACHE MEMORY MATRIX [BLOCK = "<<to_count<<"][WORD = "<<BLOCK_SIZE<<"] \t\t\n";
        for(int bl = 0; bl < to_count ; bl++){
            for(int wo = 0; wo < BLOCK_SIZE; wo++){
                for(int by = 0; by < WORD_SIZE; by++){
                    for(int bi = 0;  bi < BYTE_SIZE ; bi++){
                        cout<<CACHE[bl][wo][by][bi];
                    }
                    cout<<" | ";
                }
                cout<<"\n";
            }
            cout<<"\n\n";
        }
    }
}
// This Functions Accepts Parameter, how many Blocks of Main Memory we want to see.
// Default parameters are used as 1, if function is called without parameter loop will not execute, and no output will be generated
// So Make Sure to pass some no.
void see_mainMemory(int to_count = 1){
    if(to_count <= 128){
        // TO Ensure Input Number is With in Limits

        cout<<"\n\t\t MAIN MEMORY MATRIX [BLOCK = "<<to_count<<"][WORD = "<<BLOCK_SIZE<<"]\t\t\n";
        for(int bl = 0; bl < to_count ; bl++){
            for(int wo = 0; wo < BLOCK_SIZE; wo++){
                for(int by = 0; by < WORD_SIZE; by++){
                    for(int bi = 0;  bi < BYTE_SIZE ; bi++){
                        cout<<MAIN[bl][wo][by][bi];
                    }
                    cout<<" | ";
                }
                cout<<"\n";
            }
            cout<<"\n\n";
        }
    }
}



void INSERT_CLI(){
    int chip = 0,block_no = 0, word_no = 0, byte_no = 0, bit_no = 0, data_no = 0;
    re_select_chip:
    // ------------------------------------------------------------
    cout<<"\n\n 1. Main Memory \t[ Press 1 ]"
        <<"\n 2. Cache Memory \t[ Press 2 ]";
    cin>>chip;

    if(!(chip == 1 || chip == 2)){
        cout<<"\n  Enter valid Key !";
        goto re_select_chip;
    }
    // -------------------------------------------------------------
    reenter_block :
    cout<<"\n Enter Block No. [ Indexing started from 0 ] :- ";
    cin>>block_no;

    if((chip == 1) && (block_no >= 0 || block_no < 128)){
        // chip 1 for Main Memory


    }else if((chip == 2) && (block_no >= 0 || block_no < 8)){
        // chip 2

    }else{
        cout<<"\n  Enter valid Key !";
        goto reenter_block;
    }
    // --------------------------------------------------------------
    reenter_word :
    cout<<"Enter Word No. [ 0 to "<<BLOCK_SIZE-1<<" ] :- ";
    cin>>word_no;
    if(!(word_no >= 0 && word_no < BLOCK_SIZE)){
        goto reenter_word;
    }

    // --------------------------------------------------------------
    reenter_byte :
    cout<<"Enter Byte No. [ 0 to "<<WORD_SIZE-1<<" ] :- ";
    cin>>byte_no;
    if(!(byte_no >= 0 && byte_no < WORD_SIZE)){
        goto reenter_byte;
    }

    // --------------------------------------------------------------
    reenter_bit :
    cout<<"Enter Bit No. [ 0 to "<<BYTE_SIZE-1<<"] :- ";
    cin>>bit_no;
    if(!(bit_no >= 0 && bit_no < BYTE_SIZE)){
        goto reenter_bit;
    }


    // --------------------------------------------------------------
    data_bit :
    cout<<"Enter Data  [ 0 or 1] :- ";
    cin>>data_no;
    if(!(data_no == 0 || data_no == 1)){
        goto data_bit;
    }

    insert(data_no,block_no,word_no,byte_no,bit_no,chip-1);// chip -1 for 0 to Main Memory or 1 to Cache Memory





}
int main(){

        cout<<" Temporary Store address 00000001 ";
        cin>>MAIN[0][0][0][1];

        int decide = 0;

        START_MENU:
        see_mainMemory();
        cout<<"\t\t Main Menu"
            <<"\n1. Insert Numbers at Positions \t\t [ Press 1 ]"
            <<"\n2. Read Numbers \t\t\t [ Press 2 ]"
            <<"\n3. Exit \t\t\t\t [ Press 3 ]";
        cout<<"\n Please Enter Your Choice :- "; cin>>decide;
        switch(decide)
        {
            case 1: INSERT_CLI(); goto START_MENU;
            case 2: goto START_MENU;
            case 3: goto exit;
            default : cout<<"\n Incorrect Key Pressed \n"; goto START_MENU;
        }
        // see_mainMemory();
        // transfer(0,0,0,0,0);
        // cout<<"\n Transfer Done \n";
        // see_cacheMemory();



    exit:
    cout<<"\n\nHope You Liked this! \nBye Bye, Stay Safe and keep Learning :) by Pushpendra Vishwakarma \n";
}