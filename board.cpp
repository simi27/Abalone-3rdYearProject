/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "board.h"
#include <iomanip>
#include <iostream>
string traverse(const cell* start,const int& pd,const int& sd,const int& td){
    string ts;
    const cell* linep = start;
    while(linep!=nullptr/*linep->getAdjacent(pd)!=nullptr || linep->getAdjacent(sd)!=nullptr*/){
        const cell* cellp = linep;
        while(cellp!=nullptr){
            ts+=(cellp->getLocation()+' ');
            cellp=cellp->getAdjacent(td);
        }
        if(linep->getAdjacent(pd)==nullptr)
            linep=linep->getAdjacent(sd);
        else
            linep=linep->getAdjacent(pd);
    }
    /*const cell* cellp = linep;
    while(cellp!=nullptr){
        ts+=(cellp->getLocation()+' ');
        cellp=cellp->getAdjacent(td);
    }*/
    return ts;
}
void setMarbles(cell* row,const int& z, const char& m){
    for(int i=0;i<z;i++){
        row[i].setMarble(m);
    }
    if(z==7){
        char pos[4]={0,1,5,6};
        for(int i=0;i<4;i++){
            row[pos[i]].setMarble('+');
        }
    }
}
void connectHorizontal(cell* row, const int& rz){
    for(int i=0;i<rz-1;i++){
        row[i].setAdjacent(row+i+1,1);
    }
    //row[0].setAdjacent(row+1,1);
    //row[rz-2].setAdjacent(row+rz-1,1);    
}

void connectDiagonal(cell* lrow,cell* srow,const int& f,const int& sz,const bool& top){
    for(int i=f;i<(f+sz);i++){
        if(top){
            if(f==0){//connecting from cell 0 in longer row
                lrow[i].setAdjacent(srow+i,0);
            }
            else if(f==1){//connecting from cell 1 in longer row
                lrow[i].setAdjacent(srow+i-1,5);
            }
            else return;
        }
        else{
            if(f==0){
                lrow[i].setAdjacent(srow+i,2);
            }
            else if(f==1){
                lrow[i].setAdjacent(srow+i-1,3);
            }
            else return;            
        }
    }
}
void mapCells(cell* row,map<string,cell*>& cm,const char& r,const int& z){    
    int f=1;
    if(r>='A'&& r<='E'){
        f=1;
    }else if(r>'E'&& r<='I'){
        f=r-'E'+1;
    }else return;
    for(int i=0;i<z;i++){
        stringstream ss;
        ss<<r;
        ss<<(i+f);
        string lc = ss.str();
        row[i].setLocation(lc);
        cm[lc]=row+i;
    }
}

/*
 * woc and boc are now set to initial values of 0
 */
board::board():woc(0),boc(0){
    char r = 'I';
    for(int i=0;i<9;i++){
        if(i<5){
            rows.push_back(new cell[5+i]);
            connectHorizontal(rows[i],5+i);
            mapCells(rows[i],cells,r--,5+i);
            if(i<3){
                setMarbles(rows[i],5+i,'O');
            }else
                setMarbles(rows[i],5+i,'+');
        }
        else{
            rows.push_back(new cell[5+8-i]);
            connectHorizontal(rows[i],5+8-i);
            mapCells(rows[i],cells,r--,5+8-i);
            if(i>=6){
                setMarbles(rows[i],5+8-i,'@');
            }else
                setMarbles(rows[i],5+8-i,'+');
        }
    }
    for(int i=1;i<9;i++){
        if(i<5){
            connectDiagonal(rows[i],rows[i-1],0,5+i-1,true);
            connectDiagonal(rows[i],rows[i-1],1,5+i-1,true);
        }
        else{
            connectDiagonal(rows[i-1],rows[i],0,5+8-i,false);
            connectDiagonal(rows[i-1],rows[i],1,5+8-i,false);
        }
    }
}

/*@exercise 2
 * In the copy constructor the woc and boc variable are replaced and copied to
 * 	represent the same values as those that in the board
 */
board::board(const board& copy): board(){//constructor delegation - C++11
    woc=copy.woc;
    boc=copy.boc;
    map<string,cell*>::const_iterator it = copy.cells.begin();
    while(it!=copy.cells.end()){
        (cells[it->first])->setMarble((*(it->second)).getMarble());
        it++;
    }
}

board::~board(){
    for(int i=0;i<9;i++){
        delete[] rows[i];
        rows[i]=nullptr;
    }
    cells.clear();
    //cout<<"good bye!";
}

/*
 * Changes made such that now we are not just displaying the board but also
 * display the number number of marbles that have been knoecked off the board.
 */
board::operator std::string() const{
    stringstream ss;
    char r ='I'; //row letter
    int d=9;	//diagonal number
    for(int i=0;i<9;i++){
        if(i<5){ //if upper part of the baord that is above E
            ss<<setw(5-i)<<r<<" "; //print 5-i number of spaces then r then a space
            for(int k=0;k<5+i;k++){	//get a marble  of current cell and print it to screen
                ss<<rows[i][k].getMarble()<<" ";
            }
            ss<<endl;
            r--; //move down to the next row
        }
        else{//lower part of the board
            ss<<setw(i-3)<<r<<" ";
            for(int k=0;k<5+8-i;k++){
                ss<<rows[i][k].getMarble()<<" ";
            }
            ss<<d<<endl;
            r--;
            d--;
        }
    }
    d=1;
    ss<<setw(8)<<d++<<" ";
    for(;d<6;d++)
        ss<<d<<" ";
    ss<<endl;
    for(int i=0;i<woc;i++)
        ss<<'O'<<" ";
    ss<<endl;
    for(int i=0;i<boc;i++)
        ss<<'@'<<" ";
    ss<<endl;
    return ss.str();
}

string board::traverseDiagonal() const{
    //backward diagonal - nw to se
    /*
    map<string,cell*>::const_iterator it = cells.find(string("A1"));
    cell* start = it->second;
    return traverse(start,1,0,5); 
    */ 
    //forward diagonal - sw to ne
    /**/
    map<string,cell*>::const_iterator it = cells.find(string("E1"));
    cell* start = it->second;
    return traverse(start,0,1,2); 
    /**/
}
string board::traverseHorizontal() const{
    map<string,cell*>::const_iterator it = cells.find(string("A1"));
    cell* start = it->second;
    return traverse(start,5,0,1);
}

/*
 * We need to make sure that the provided location is valid that is it exits in the board
 * In our cell code we already validated the locations of the marble.
 * Go to cell class for more information on what happened, they have new method validate location
 */
bool board::validateMove(const string& l,const int& n, const int& fd, const int& md, int& mtype, bool& scoreMove) const{
    if(!cell::validateLocation(l))//confirm existence of location
        //erromsg
        return false;
    else{        
        cell* cellpointer = cells.at(l);//initialize to leading cell (get the cell)
        const char marble = cellpointer->getMarble(); // get the marble on it
        if(marble=='+')//trying to move a space
            return false;
        for(int i=1;i<n;i++){ //confirm that the marbles involved in the move are actually next to each other
            cellpointer=cellpointer->getAdjacent(fd);
            if(cellpointer==nullptr || marble!=cellpointer->getMarble())//confirm availability and type of claimed resources
                return false;
        }
        if(fd==getAdjacentIndex(md)){//in-line move
            cellpointer = cells.at(l);//initialize to leading cell
            mtype=0;//how many opponent marbles are in front of the leading marble i.e in the move direction
            for(int i=0;i<n;i++){ //this loop is counting how many opponent marbles are in the move direction
                cellpointer=cellpointer->getAdjacent(md);
                if(cellpointer!=nullptr){//confirm availability of required resources
                    if(marble==cellpointer->getMarble())//met own marble instead of space
                        return false;
                    if('+'!=cellpointer->getMarble())//if it belongs to opponent
                        mtype++;//count opponent marbles
                    else
                        break;
                }
                else{
                    if(mtype>0)//opponent's marble will be pushed off the board
                        scoreMove=true;
                    else//if trying to push your own marble off the board
                        return false;
                    break;
                }
            }
            if(mtype>=n)//should only push fewer than n of the opponent's marbles
                return false;
        }
        else{//broad-side move
            mtype=-1;
            if(n<=1)//broad-side move must use at least 2 marbles
                return false;
            cellpointer = cells.at(l);
            for(int i=0;i<n;i++){
                cell* dp=cellpointer->getAdjacent(md);//destination pointer (destination cell)
                if(dp!=nullptr && '+' == dp->getMarble())//is there an empty space to move to?
                    cellpointer=cellpointer->getAdjacent(fd);
                else
                    return false;
            }
        }
        return true;
    }
}

bool board::executeMove(const string& l,const int& n, const int& fd, const int& md){
    int moveType = 0;//default to in-line with no following marbles
    bool scoreMove = false;
    bool valid = validateMove(l,n,fd,md,moveType,scoreMove);
    if(valid){
        if(moveType==-1){//broad-side
            cell* cellpointer = cells.at(l);
            for(int i=0;i<n;i++){
                cell* dp=cellpointer->getAdjacent(md);//destination pointer
                dp->setMarble(cellpointer->getMarble());
                cellpointer->setMarble('+');
                cellpointer=cellpointer->getAdjacent(fd);
            }
        }
        else{//in-line
            cell* cellpointer = cells.at(l), *rearp=cellpointer;
            const char marble = cellpointer->getMarble();
            cell* frontp=cellpointer->getAdjacent(md); //pointer point to move direction of the marbles moving
            for(int i=1;i<n;i++){
                rearp=rearp->getAdjacent(fd); //we iterate through the moving marbles and find the tail of the moving marbles
            }
            if(scoreMove){                
                frontp->setMarble(rearp->getMarble());
                rearp->setMarble('+');
                if(marble=='O')
                    boc++;
                if(marble=='@')
                    woc++;
            }
            else{
                cell* tipp = frontp;
                for(int i=0;i<moveType;i++){
                    tipp=tipp->getAdjacent(md);
                }
                tipp->setMarble(frontp->getMarble());
                frontp->setMarble(rearp->getMarble());
                rearp->setMarble('+');
            }
        }
    }
    return valid;
}
