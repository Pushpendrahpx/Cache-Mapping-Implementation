#include <iostream>
#include <string.h>
#include <math.h>
#include <cstdlib>

using namespace std;

#define BYTE_SIZE 8

#define MAIN_MEMORY_SIZE 128 // No. of Blocks in Main Memory
#define CACHE_MEMORY_SIZE 16 // No. of Blocks in Cache Memory
// Total 256 Words = 1024 Bytes of Main Memory
// Let Cache can have 16 Words = 64 Bytes of Cache Memory
#define WORD_SIZE 4 // Each Word have 4 Bytes of Memory
#define BLOCK_SIZE 2 // Each Block will have 2 Words of Memory

#define K 4

typedef int BIT;
typedef BIT BYTE[8];
typedef BYTE WORD[WORD_SIZE];
typedef WORD BLOCK[BLOCK_SIZE];

typedef BLOCK SETS[4]; //            CACHE_MEMORY_SIZE / 4        ( as 4 way Set Associative)

BLOCK MAIN[MAIN_MEMORY_SIZE] = {0};
SETS CACHE[4] = {0};

int CACHE_BLOCKS[4][4] = {{-1,-1,-1,-1},{-1,-1,-1,-1},{-1,-1,-1,-1},{-1,-1,-1,-1}};
int recent_position[4] = {-1,-1,-1,-1};

int HIT = 0, MISS = 0;


// Last Parameter named 'choice' will be used to identify to where Data is to be Inserted
// if choice == 0, then Main Memory
// elsse Cache Memory
void insert(int new_bit_data, int block = 0, int word = 0, int byte = 0, int bit = 0, int choice = 0,int set = 0){
    if(choice == 0){
        MAIN[block][word][byte][bit] = new_bit_data;
    }else{
        CACHE[set][block][word][byte][bit] = new_bit_data;
    }
}



// choice = 0, means, Data Transfer will be from MAIN_MEMORY to CACHE_MEMORY
// choice = 1, means, Data Transfer will be from CACHE_MEMORY to MAIN_MEMORY
// ##### NOTE - No Matter we transfer data from AnyWhere to Anywhere, whole Block should be transfered. #####
int char2int(char c){
    if(c == '0'){
        return 0;
    }else{
        if(c == '1'){
            return 1;
        }else{
            return -1;
        }
    }
}
// [ No Issues of Bandwidth will be there while Tranferring as whole word will be get transferred ]
void transfer(int main_block,int SET,int choice = 0){
    cout<<" transfer << Main_BLOCK = "<<main_block<<" position = "<<recent_position[SET]<<" ## ";
    if(choice == 0){
        // Main => Cache Memory
        for(int wo = 0; wo < BLOCK_SIZE; wo++){
            for(int by = 0; by < WORD_SIZE; by++){
                for(int bi = 0;  bi < BYTE_SIZE ; bi++){

                    CACHE[SET][recent_position[SET]][wo][by][bi] = MAIN[main_block][wo][by][bi];


                }
            }
        }
        CACHE_BLOCKS[SET][recent_position[SET]] = main_block;
        cout<<"THIS = "<<CACHE_BLOCKS[SET][recent_position[SET]];

    }else{
        // Cache ==> Main_Memory
        for(int wo = 0; wo < BLOCK_SIZE; wo++){
            for(int by = 0; by < WORD_SIZE; by++){
                for(int bi = 0;  bi < BYTE_SIZE ; bi++){

                    MAIN[main_block][wo][by][bi] = CACHE[SET][recent_position[SET]][wo][by][bi];
                }
            }
        }

    }


}



// This Functions Accepts Parameter, how many Blocks of Main Memory we want to see.
// Default parameters are used as 1, if function is called without parameter loop will not execute, and no output will be generated
// So Make Sure to pass some no.
void see_cacheMemory(int to_count = 1){

    if(to_count <= 4){
        // TO Ensure Input Number is With in Limits
        cout<<" Tag Directory of Cache ["<<CACHE_MEMORY_SIZE<<"] - ";
        for(int o = 0; o < 4; o++){
            for(int i = 0; i < 4; i++){
                if(CACHE_BLOCKS[o][i] == -1){
                    cout<<"NULL";
                }else
                    {
                        cout<<CACHE_BLOCKS[o][i];
                    }
                cout<<", ";
            }
        }
        cout<<"\n\t\t  =========  CACHE MEMORY MATRIX  ========= \n\t\t\t\t [BLOCK = "<<to_count-1<<"][Each Row Represents a word] \t\t\n";
        for(int se = 0; se < to_count; se++){
            for(int bl = 0; bl < to_count; bl++){
                for(int wo = 0; wo < BLOCK_SIZE; wo++){
                    cout<<"\t\t Word "<<wo<<" => ";
                    for(int by = 0; by < WORD_SIZE; by++){
                        for(int bi = 0;  bi < BYTE_SIZE ; bi++){
                            cout<<CACHE[se][bl][wo][by][bi];
                        }
                        cout<<" | ";
                    }
                    cout<<"\n";
                }
                cout<<"\n\n";
            }
        }
    }
}
// This Functions Accepts Parameter, how many Blocks of Main Memory we want to see.
// Default parameters are used as 1, if function is called without parameter loop will not execute, and no output will be generated
// So Make Sure to pass some no.
void see_mainMemory(int to_count = 1){
    if(to_count <= 128){
        // TO Ensure Input Number is With in Limits
        cout<<"\n\t\t There are more blocks but we have printed only 1 \n\n";
        cout<<"\n\t\t ========= MAIN MEMORY MATRIX  ========= \n\t\t\t\t [BLOCK = "<<to_count-1<<"][Each Row Represents a word]\t\t\n";
        for(int bl = 0; bl < to_count ; bl++){
            for(int wo = 0; wo < BLOCK_SIZE; wo++){
                cout<<"\t\t Word "<<wo<<" => ";
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
    int chip = 0,set_no = 0,block_no = 0, word_no = 0, byte_no = 0, bit_no = 0, data_no = 0;
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
    reenter_set :
    cout<<"\n Enter SET No. [ Indexing started from 0 to 3 ] :- ";
    cin>>set_no;

    // -------------------------------------------------------------
    reenter_block :
    cout<<"\n Enter Block No. [ Indexing started from 0 to 3 ] :- ";
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

    insert(data_no,set_no,block_no,word_no,byte_no,bit_no,chip-1);// chip -1 for 0 to Main Memory or 1 to Cache Memory

}

int b2d(char *str){
    int len = strlen(str);
    int decimal = 0;
    for(int i = len-1; i >= 0; i--){
        int digit = str[i] == '0' ? 0 : 1;
        decimal += digit * pow(2,len-i-1);
    }
    return decimal;
}

void read_request(){


    int read;char status = 'Y';
    do{
        char name[100];
        cout<<"\n Direct Mapping [ 5 Tag Bits +  2 Set Bits + 1 Offset Bit ] ";
        cout<<"\n Enter Address No. [ 8-Bits ] :- ";
        cin>>name;
        int DECIMAL = b2d(name);



        read = DECIMAL;
        // cout<<" Enter Block No.[ 0 to "<< MAIN_MEMORY_SIZE -1 <<"]  :- ";cin>>read;// reading block no. of main Memory to be there on Cache or not

        bool found = false;
                    // for(int i = 0; i < CACHE_MEMORY_SIZE; i++){
                    //     if(CACHE_BLOCKS[i] == read){





                    //         found = true;
                    //         printf("\x1B[32m \n This was Hit \033[0m\t\t"); //
                    //         cout<<"\n Data Was Present in Cache \n";
                    //         HIT++;

                    //         break;
                    //     }
                    // }

        // int line_number = read % CACHE_MEMORY_SIZE;
        // // int tag =
        // int tag_bit_size = 3,line_bit_size = 4,offset_bit_size = 1;
        // int TAG = 0, LINE = 0;
        // TAG = 4*char2int(name[0]) + 2*char2int(name[1]) + 1*char2int(name[2]);
        // LINE = 8*char2int(name[3]) + 4*char2int(name[4]) + 2*char2int(name[5]) + 1*char2int(name[6]);
        // cout<<" TAG VALUE = "<<TAG<<" p \n";


         // CACHE_BLOCKS[LINE] <=== TAGS
        //  if(CACHE_BLOCKS[][LINE] == TAG){
        //      found = true;
        //                 printf("\x1B[32m \n This was Hit \033[0m\t\t"); //
        //                 cout<<"\n Data Was Present in Cache \n";
        //                 HIT++;
        //  }
        //  for(int i = 0; i < CACHE_MEMORY_SIZE; i++){
        //      if(TAG == CACHE_BLOCKS[i]){

        //                 break;
        //      }
        //  }
        // int k = 4;
        // K  Macro Exists
        int TAG = 0,SET = 0;
        TAG = 16*char2int(name[0]) + 8*char2int(name[1]) + 4*char2int(name[2]) + 2*char2int(name[3]) + 1*char2int(name[4]);
        SET = 2*char2int(name[5]) + 1*char2int(name[6]);
        // As this is 4 way set Associative, 4 blocks will be inside 1 set
        cout<<" TAG = "<<TAG<<", SET = "<<SET<<endl;
        for(int i = 0; i < 4 ; i++){
            if(CACHE_BLOCKS[SET][i] == TAG){
                // Hit
                        found = true;
                        printf("\x1B[32m \n This was Hit \033[0m\t\t"); //
                        cout<<"\n Data Was Present in Cache \n";
                        HIT++;
            }
        }




        if(found == false){

            MISS++;
            cout<<"\n This was Miss [TOTAL MISS = "<<MISS<<" ] \n";
            cout<<"\n MOVED BLOCK FROM MAIN MEMORY TO CACHE \n";
            // read var has block number of main Memory to be shifted from MM to CM

            recent_position[SET] = recent_position[SET] + 1;
            if(recent_position[SET] == 4){
                recent_position[SET] = 0;
            }
            cout<<" TRANSFER read = "<<read;
            transfer(read,SET,0);// MM to CM

        }

        cout<<"\n =========== STATUS  ===========  \n";

        cout<<" TOTAL ACTIONS = "<<HIT + MISS<<"\n";


        cout<<" HITS = "<<HIT;

        cout<<", MISS = "<<MISS;

        cout<<"\nHit Ratio = "<< (float) ((float) (HIT))/((float)(HIT+MISS));
        cout<<"\n\n Want to Read Again [Y/N] :- ";
        cin>>status;
    }while((status == 'Y') || (status == 'y'));
}


int main(){

        // cout<<" Temporary Store address 00000001 ";
        // cin>>MAIN[0][0][0][1];

        int decide = 0;
        START_MENU:
        see_mainMemory();
        see_cacheMemory();
        cout<<"\t\t ====================== Main Menu [Set Associative Mapping] ====================== "
            <<"\n1. Insert Numbers at Positions \t\t [ Press 1 ]"
            <<"\n2. Read Numbers \t\t\t [ Press 2 ]"
            <<"\n3. Exit \t\t\t\t [ Press 3 ]";
        cout<<"\n Please Enter Your Choice :- "; cin>>decide;
        switch(decide)
        {
            case 1: INSERT_CLI(); goto START_MENU;
            case 2: read_request(); goto START_MENU;
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