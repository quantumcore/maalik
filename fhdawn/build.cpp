#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

std::string XOR(std::string data, std::string key) {
    std::string output = data;
    
    for (int i = 0; i < data.size(); i++){
        output[i] = data[i] ^ key[i % key.size()];
	}
    return output;
}


void build(
   std::string host,
    std::string port,
    std::string ExeToBuild,
    std::string KEY
) {
    std::ofstream outfile;
    std::ostringstream vals;
    vals << XOR(host, KEY) << "^" << XOR(port, KEY) << "^" << KEY;
    outfile.open(ExeToBuild, std::ios::app | std::ios::binary);
    if(outfile.is_open()){
		outfile << "\n\n";  
        outfile << vals.str().c_str();
        outfile.close();
        //std::cout << vals.str().c_str() << " " << host<< " " << port <<  " " << KEY;
        std::cout << "\n\nBuilt FHDAWN Client : " << ExeToBuild << std::endl;
    }
}

int main()
{
    std::string host, port, key, file;
    std::cout << "Enter HOST : ";
    std::getline(std::cin, host);

    std::cout << "Enter PORT : ";
    std::getline(std::cin, port);
    
    std::cout << "Enter KEY  : ";
    std::getline(std::cin, key);

    std::cout << "Enter FHDAWN Path : ";
    std::getline(std::cin, file);
    
    build(
    host,
    port,
    file,
    key
    );

}