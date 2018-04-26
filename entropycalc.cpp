#include <iostream>
#include <iomanip>
#include <fstream>
#include <math.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

inline double log2(double x) {return log(x)/log(2);}

int main(int argc, char** argv)
{

////////////////////////////Command Line Arguments/////////////////////////////
  bool binaryMode = false;
  
  if(argc == 1)
  {
    cout<<"File Entropy Calculator v0.3"<<endl;
    cout<<"Author:   Unsal Gokdag"<<endl<<endl;
    cout<<"Counts occuring of each letter and calculates the entropy via:"<<endl<<endl;
    cout<<"    H=-P(A)3(A)"<<endl<<endl;
    cout<<"where:"<<endl;
    cout<<"P(A) is probablity of symbol"<<endl;
    cout<<"3(A) is information held by symbol = log2(A)"<<endl<<endl;
    cout<<"Update: Calcution of 1-step Markov Proecess is added."<<endl<<endl;
    cout<<"Usage:"<<endl;
    cout<<"./entropycalc <filename> (binary)"<<endl;
    cout<<"binary makes program to calculate entropy of all ascii values."<<endl<<endl;
    cout<<"Example:"<<endl;
    cout<<"./entropycalc Feeding.txt"<<endl;
    cout<<"./entropycalc 'head.bmp.zip' binary"<<endl;
    cout<<"Enjoy!"<<endl;

    return 0;
  }
  if(argc == 3)
  {
    if(strcmp(argv[2],"binary")==0) binaryMode = true;
  }

////////////////////////////////File Reading///////////////////////////////////
  ifstream fin(argv[1],ios::in|ios::binary|ios::ate); //input file
  
  // Check file exists
  if(!fin.is_open())
  {
    cout<<"Unable to open file "<<argv[1]<<endl;
    return 0;
  }

  // File Reading Part
  int fsize = fin.tellg();
  char* memblock = new char [fsize];    //All file is copied into the array
  fin.seekg (0, ios::beg);
  fin.read (memblock, fsize);
  fin.close();
  

//////////////////////////Counting the Occurences//////////////////////////////
  
  int* occ = new int[256]();
  
  //Counting 2 character occurences for Markov Model
  int** occ2 = new int*[256]();
  for (int i = 0; i < 256; ++i)
    occ2[i] = new int[256]();

  for (int i = 0; i < fsize; ++i)
  {
    //single character part
    if(memblock[i]>=0) ++occ[memblock[i]];
    else ++occ[256+memblock[i]];

    //Markov model with 1 character history
    if(i>1)
    {
      int c1 = memblock[i];
      int c2 = memblock[i-1];

      //convert signed char to unsigned
      if(c1 < 0) c1+=256;
      if(c2 < 0) c2+=256;

      ++occ2[c1][c2];
    }
  }

//////////////////////////Probability Calculations/////////////////////////////

  int total = 0;
  double* prob;         //single character probabilities
  double* ent;          //single character entropies
  double totalEnt = 0;  //Single character total Entropy

  double** prob2;       //Markov model probabilities
  double* ent2;         //Markov model entropies
  double totalEnt2 = 0; //Markov model total Entropy
  
  int startInd;         //start index of calculation
  int endInd;           //finish index of calculation

  // Converts all letters to lower case and calculates accordingly
  if(!binaryMode)
  {
    int diff = 'a'-'A';
    for (int i = 'A'; i <= 'Z'; ++i)
    {
      occ[i+diff]+=occ[i];
      total+=occ[i+diff];
    }

    startInd = 'a';
    endInd = 'z';

    //Allocation of arrays
    prob = new double[25]();
    ent = new double[25]();

    prob2 = new double*[25];
    for (int i = 0; i < 26; ++i)
      prob2[i] = new double[25]();
    ent2 = new double[25]();
  }
  else
  {
    for (int i = 0; i <= 255; ++i)
      total+=occ[i];

    startInd = 0;
    endInd = 255;
    prob = new double[256]();
    ent = new double[256]();

    prob2 = new double*[256];
    for (int i = 0; i < 256; ++i)
      prob2[i] = new double[256]();
    ent2 = new double[256]();
  }

  
////////////////////////////Statistics calculations////////////////////////////
  int totalLow = 0;             //lowercase characters
  int totalUp = 0;              //uppercase characters
  int totalPunc = 0;            //punctuation characters

  for (int i = 0; i < 26; ++i)
  {
    totalLow+=occ['a'+i];
    totalUp+=occ['A'+i];
  }
  for (int i = 33; i < 48; ++i)
  {
    totalPunc+=occ[i];
  }
  totalPunc+= occ[58]+occ[59]+occ[63]+occ[94]+occ[96]+occ[126];

//////////////////////////////File Statistics//////////////////////////////////
  if(!binaryMode)
  {
    cout<<"File Statistics:"<<endl;
    cout<<"---------------------------"<<endl;
    cout<<"Total Chararcters:     "<<total<<endl;
    cout<<"Lowercase Characters:  "<<totalLow<<endl;
    cout<<"Uppercase Characters:  "<<totalUp<<endl;
    cout<<"Punctuation Count:     "<<totalPunc<<endl;
  }

  //Single character probability and entropy calculation part
  for (int ind = startInd; ind <= endInd; ++ind)
  {
    int i = ind-startInd;
    if(occ[ind]>0)      //calculate only occuring elements
    {
      prob[i] = (double)occ[ind]/total;
      ent[i] =-log2(prob[i])*prob[i];
      totalEnt += ent[i];
    }
  }

  //Markov model probability and entropy calculation part
  for (int ind = startInd; ind <= endInd; ++ind)
  {
    int i = ind-startInd;
    for (int ind2 = startInd; ind2 <= endInd; ++ind2)
    {   
      int i2 = ind2-startInd;
      if(occ2[ind][ind2]>0)      //calculate only occuring elements
      {
        prob2[i][i2] = (double)occ2[ind][ind2]/total;
        ent2[i] += -log2(prob2[i][i2])*prob2[i][i2]/2;
      }
    }
    totalEnt2 += ent2[i];
  }

//////////////////////////////Print Results////////////////////////////////////

  cout<<endl<<endl;
  cout<<"Singular Chararcter Part:"<<endl;
  cout<<"Chararcter Entropies"<<endl;
  cout<<"---------------------------"<<endl;

  for (int ind = startInd; ind <= endInd; ++ind)
  {
    int i = ind - startInd;
    if(occ[ind]>0)
    {
      if(binaryMode)
      {
        cout<<setw(3)<<ind;
        cout<<": "<<setw(5)<<fixed<<setprecision(5)<<ent[i]<<"   ";
        if(ind % 5 == 4) cout<<endl;
      }
      else
      {
        cout<<(char)ind;
        cout<<": "<<setw(5)<<fixed<<setprecision(5)<<ent[i]<<endl;
      }
    }
  }
  cout<<endl<<"Total Entropy: "<<totalEnt<<endl<<endl;

  cout<<endl;
  cout<<"Markov Model Part:"<<endl;
  cout<<"State Entropies"<<endl;
  cout<<"---------------------------"<<endl;

  for (int ind = startInd; ind <= endInd; ++ind)
  {
    int i = ind - startInd;
    if(ent2[i]>0)
    {
      if(binaryMode)
      {
        cout<<setw(3)<<ind;
        cout<<": "<<setw(5)<<fixed<<setprecision(5)<<ent2[i]<<"   ";
        if(ind % 5 == 4) cout<<endl;
      }
      else
      {
        cout<<(char)ind;
        cout<<": "<<setw(5)<<fixed<<setprecision(5)<<ent2[i]<<endl;
      }
    }
  }
  cout<<endl<<"Total Entropy: "<<totalEnt2<<endl<<endl;
  cout<<"Comment on file:"<<endl;
  if(totalEnt2<3)
  {
    cout<<"This file has very low entropy which means it has repetitive characters"<<endl;
    cout<<"possibly an uncompressed image file or a .txt file."<<endl;
  }
  else if(totalEnt2<7)
  {
    cout<<"This file seems to have average amount of entropy. It could be a general"<<endl;
    cout<<"purpose document."<<endl;
  }
  else
  {
    cout<<"This file has high entropy which means it has very low amount of redundant"<<endl;
    cout<<"information. Possibly a lossy or lossless compressed file."<<endl;
  }

	return 0;
}