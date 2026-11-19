#include "util.h"

// Primitive helper utilities, clamping, and screen control
#include <iostream>
 using namespace std;

 
    int clampInt(int val, int minVal, int maxVal){

        if(val<minVal){
            return minVal;
        }
        else if(val>maxVal){
            return maxVal;
        }
        else{
            return val;
        }
    }
void trimWhitespace(char* str){

    if(str==nullptr){
        return;
    }
    int len=0;
    while(str[len]!='\0'){

    len++;
   
    }
    
    while(len>0 && (str[len-1] == ' ' || str[len-1] == '\t' || str[len-1] == '\n' || str[len-1] == '\r')){
        len--;
    }
    str[len]='\0';
    int start=0;
    
    while((str[start] == ' ' || str[start] == '\t' || str[start] == '\n' || str[start] == '\r')&& (str[start]!='\0')){
        start++;
    }
    int i=0;
    while(str[start+i]!='\0'){
        str[i] = str[start + i];
        i++;
    }
    str[i]='\0';
}

bool stringEquals(const char* a, const char* b){
 int i=0;
 if(a==nullptr || b==nullptr){
        return false;
    }
 while(true){
    
    if(a[i]!=b[i]){
        return false;
    }
    if(a[i]=='\0' && b[i]=='\0'){
        return true;
    }
    i++;
}

}
void printHeader(const char* title){
    for(int i=0;i<=10;i++){
        cout<<"==";

    }
    cout<<endl;
    cout<<"TITLE "<<title<<endl;
    for(int i=0;i<=10;i++){
        cout<<"==";

    }
    cout<<endl;
}
void clearScreen(){
    for(int i=0;i<30;i++){
        cout<<"\n";
    }
}
 