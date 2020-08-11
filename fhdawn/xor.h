// XOR encryption
#ifndef  XORENC
#define XORENC
#include "fhdawn.h"

std::string XOR(std::string data, std::string encrypted_key) {
    std::string output = data;
    
    for (int i = 0; i < data.size(); i++){
        output[i] = data[i] ^ encrypted_key[i % encrypted_key.size()];
	}
    return output;
}

#endif // ! XORENC

