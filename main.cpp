/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: MSCS
 *
 * Created on 12 March 2020, 8:38 AM
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include "board.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
	printf("argc = %d\n", argc);
	printf("What is in argv[]\n");

	for (int var = 0; var < argc; ++var) {
		printf("argv[%d] = %s\n", var, argv[var]);
	}

    if(argc==2){
        board abalone;
        string ibs(abalone);
        cout<<"Initial Abalone board state:"<<endl;
        cout<<ibs<<endl;        
        ifstream log(argv[1]);//sequence of game moves
        string l;
        int n, fd, md;
        while(log>>l>>n>>fd>>md){
            cout<<"Move: "<<l<<" "<<n<<" "<<fd<<" "<<md;
            bool valid = abalone.executeMove(l,n,fd,md);
            if(valid){
                string nbs(abalone);
                cout<<" - Next Abalone board state:"<<endl;
                cout<<nbs<<endl;
            }
            else{
                cout<<" - Invalid move!"<<endl;
            }
        }    
        return 0;
    }
    return -1;
}

