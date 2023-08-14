/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define BIT_32 4
#define BIT_16 2

typedef struct Packet {
    uint16_t length;
    uint8_t time_live;
    uint8_t protocol;
    uint32_t source_addr;
    uint32_t dest_addr;
    char *data;
} Packet;

Packet receive;

//Function to decode 16 bits
uint16_t decode16 (char *array, uint16_t data){
    for (int i=0;i<BIT_16;i++){
        uint16_t temp;
        temp =  (uint16_t)(*array)-48;
        data |= temp << (BIT_16-1-i)*8;
        array++;

    }
    return data;
}

//Function to decode 32 bits
uint32_t decode32 (char *array, uint32_t data){
    for (int i=0;i<BIT_32;i++){
        printf("%c",*array);
        array++;

    }
    /* Will be using this if the data not sent converted to char 
    for (int i=0;i<BIT_32;i++){
        uint32_t temp;
        temp =  (uint16_t)(*array)-48;
        data |= temp << (BIT_32-1-i)*8;
        array++;

    }
    */ 
    return data;
}


void decodeData(char *data){
      printf("Receive pack: %s\n",data);
      receive.length = decode16 (data, receive.length); // I would use pointer to modify data 
      data+=2; // I may need to modify decode16 to track the change in the address of data
      printf("Data length: %d\n",receive.length);
      receive.time_live = *data;
      data++;
      receive.protocol = *data;
      data++;
      printf("Source address: ");
      receive.source_addr = decode32(data, receive.source_addr); 
      data+=4;
      printf("\nDestination address: ");
      receive.dest_addr = decode32 (data, receive.dest_addr);
      data+=4;
      for (int i = 0; i< receive.length; i++){ 
        printf("\nPrint Data %d: ",i);
        receive.data = decode32 (data, receive.data);
        data+=4;
      }

}
 
int main()
{
    // Create a data packet "0200x0ABx0CD1234ABCD";
    char len[]= "0200";
    char sourceAdd [] = "0xAB";
    char destAdd[] = "0xCD";
    char data[]= "1234ABCD";
    char p[50] = {0}; //emtpy packet
    strncat(p, len, 4); //form a data packet for tranmission
    strncat(p, sourceAdd, 4);
    strncat(p, destAdd, 4);
    strncat(p, data, 8); //send two 32bits data
    strncat(p, "\0", 1);

    printf("Send pack: %s\n",p);
    decodeData(p);

    return 0;
}