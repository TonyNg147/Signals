#include <iostream>
#define __IOSTREEAM__
#include <connection.h>
void print1(){
    std::cout<<"Hello 1\n";
}
void print2(){
    std::cout<<"Hello 2\n";
}
void print3(){
    std::cout<<"Hello 3\n";
}
int main(){   
    Signal<void()> connect;
    ConnectObject a = connect.connect(std::bind(&print1));
    ConnectObject b = connect.connect(std::bind(&print2));
    {
        ConnectObject c = connect.connect(std::bind(&print3));
        connect();
    }
    
}