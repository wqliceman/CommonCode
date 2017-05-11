/* rc4.h */
#ifndef RC4_H
#define RC4_H


#if defined(__cplusplus)
extern "C"
{
#endif

typedef struct rc4_key
{      
     unsigned char state[256];       
     unsigned char x;        
     unsigned char y;
} rc4_key;

void prepare_key(unsigned char *key_data_ptr,int key_data_len,rc4_key *key);
void rc4(unsigned char *buffer_ptr,int buffer_len,rc4_key * key);
   
#if defined(__cplusplus)
}
#endif 

#endif