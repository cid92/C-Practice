#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define BIT_32 4
#define BIT_16 2

typedef struct Packet {
    uint16_t length;
    uint8_t time_live;
    uint8_t protocol;
    char *source_addr; // should be 32bits
    char *dest_addr; // should be 32bits
    char *data[64];
} Packet;

Packet receive;

//Function to decode 16 bits
uint16_t decode16 (char *array){
     uint16_t temp;
    for (int i=0;i<BIT_16;i++){
        temp =  (uint16_t)(*array)-48; //convert the char to integer
        temp |= temp << (BIT_16-1-i)*8;
        array++;

    }
    return temp;
}

//Function to decode 32 bits
char* decode32 (char *array){ // should be uint32_t
    static char temp[5] = {0}; //static to retain memory when returning
    for (int i=0;i<BIT_32;i++){
        temp[i] = *array;
        array++;
    }
    /* Will be using this if the data not sent converted to char 
    for (int i=0;i<BIT_32;i++){
        uint32_t temp;
        temp =  (uint16_t)(*array);
        data |= temp << (BIT_32-1-i)*8;
        array++;

    }
    */
   return temp;
}

// Decode a pack as receive 
void decodeData(char *data){
      printf("Receive pack: %s\n",data);
      receive.length = decode16 (data); // I would use pointer for the length 
      data+=2; // I may need to modify decode16 to track the change in the address of data
      printf("Data length: %d\n",receive.length);
      receive.time_live = *data;
      data++;
      receive.protocol = *data;
      data++;
      receive.source_addr = decode32(data); 
      printf("Source address: %s\n",receive.source_addr);
      data+=4;
      receive.dest_addr = decode32 (data);
      printf("Destination address: %s\n",receive.dest_addr);
      data+=4;
      for (int i = 0; i< receive.length; i++){ 
        receive.data[i] = decode32(data);
        //decode32 (data);
        data+=4;
        printf("Data (32bits) %d: %s\n", i, receive.data[i]);
      }

}
 
int main()
{
    // Create a data packet "0200x0ABx0CD1234ABCD";
    char len[]= "0400";
    char sourceAdd [] = "0xAB";
    char destAdd[] = "0xCD";
    char data[]= "1234ABCD5678EFGH";
    char p[50] = {0}; //emtpy packet
    strcpy(p, len); //form a data packet for tranmission
    strncat(p, sourceAdd, 4);
    strncat(p, destAdd, 4);
    strncat(p, data, 32); //send 4 32bits data
    strncat(p, "\0", 1);

    printf("Send pack: %s\n",p);
    decodeData(p);

    return 0;
}
