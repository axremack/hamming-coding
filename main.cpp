#include <iostream>      
#include <fstream>
#include <string>         
#include <bitset>        
#include <vector>
#include <cmath>

/**
 * Hamming (7,4)
 **/ 
#define N 4
#define HAMMING_7 7

/**
 * Debug macros
 **/ 
#define DEBUG_RF true // Debug Information: Read File
#define DEBUG_HE true // Debug Information: Hamming Encoding

using namespace std; 

/**
 * vector<bitset<N> > readFile(string filename)
 * Read a file in binary and create a vector of bitset wih a width of 4 for each bitset
 * Return a vector bitset
 **/ 
vector<bitset<N> > readFile(string filename)
{
	vector<bitset<N> > content;
	ifstream reader;
	char buffer;
	reader.open(filename.c_str(), ios::binary|ios::in);

	if(DEBUG_RF)
		cout << "Read : \t";

	if(reader != NULL && reader.is_open())
	{
		while(!reader.eof())
		{
			reader.read(&buffer, 1);
			bitset<N> bsBufferLSB(buffer);
			bitset<N> bsBufferMSB(buffer>>4);
			
			content.push_back(bsBufferMSB);
			content.push_back(bsBufferLSB);
	
			if(DEBUG_RF)
			{
				cout << " |" << bsBufferMSB.to_string();
				cout << " |" << bsBufferLSB.to_string();
			}
		}
	}
	
	if(DEBUG_RF)
		cout << endl;

	reader.close();
	return content;
} 

/**
 * vector<bitset<HAMMING_7> > HammingEncoding(vector<bitset<N> > bitsetVector)
 * Convert a vector of bitset<4> into a hamming vector of bitset<7>
 **/ 
vector<bitset<HAMMING_7> > HammingEncoding(vector<bitset<N> > bitsetVector)
{
	vector<bitset<HAMMING_7> > encodedBitset;
	
	if(DEBUG_HE)
		std::cout << "Encode : \t";
		
	for(vector<bitset<N> >::iterator i = bitsetVector.begin(); i != bitsetVector.end();++i)
	{		
		bitset<N> inBuffer = *i;
		bitset<HAMMING_7> outBuffer;

		outBuffer[0] = inBuffer[0] ^ inBuffer[1] ^ inBuffer[3];
		outBuffer[1] = inBuffer[0] ^ inBuffer[2] ^ inBuffer[3];
		outBuffer[2] = inBuffer[0];
		outBuffer[3] = inBuffer[1] ^ inBuffer[2] ^ inBuffer[3];
		outBuffer[4] = inBuffer[1];
		outBuffer[5] = inBuffer[2];
		outBuffer[6] = inBuffer[3];
		
		if(DEBUG_HE)
			cout << " | " << outBuffer.to_string();
		
		encodedBitset.push_back(outBuffer);
	}
	
	if(DEBUG_HE)
		cout << endl;
	
	return encodedBitset;
}


vector<bitset<N> > HammingDecoding(vector<bitset<HAMMING_7> > bitsetVector)
{
	vector<bitset<N> > decodedBitset;

	if(DEBUG_HE)
		std::cout << "Decode : \t";
		
	for(vector<bitset<HAMMING_7> >::iterator i = bitsetVector.begin(); i != bitsetVector.end();++i)
	{		
		bitset<HAMMING_7> inBuffer = *i;
		bitset<N> outBuffer;
		bitset<3> s;

		s[0] = inBuffer[3] ^ inBuffer[4] ^ inBuffer[5] ^ inBuffer[6];
		s[1] = inBuffer[1] ^ inBuffer[2] ^ inBuffer[5] ^ inBuffer[6];
		s[2] = inBuffer[0] ^ inBuffer[2] ^ inBuffer[4] ^ inBuffer[6];

		int valS = 0;
		cout << " S = " << s.to_string();
		for(int j = 0; j < N - 1; j++) {
			valS += s[j] * pow(2, j);
		}
		cout << " Val parité : " << valS << endl;

		if(valS != 0) {
			inBuffer[valS - 1] = !inBuffer[valS];
		}

		outBuffer[0] = inBuffer[2];
		outBuffer[1] = inBuffer[4];
		outBuffer[2] = inBuffer[5];
		outBuffer[3] = inBuffer[6];
		
		if(DEBUG_HE)
			cout << " | " << outBuffer.to_string();
		
		decodedBitset.push_back(outBuffer);
	}

	if(DEBUG_HE)
		cout << endl;
	
	return decodedBitset;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                     Main                                                       //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	vector< bitset<N> > input_data;
	vector< bitset<HAMMING_7> > encode_data; 
	vector< bitset<N> > decode_data; 

	// Read data to encode
	input_data = readFile("test.txt");
	cout << endl;
	
	// Encode by Hamming (7,4) coding
	encode_data = HammingEncoding(input_data);
	cout << endl;

	// Introduction d'une erreur
	encode_data.back()[5] = !encode_data.back()[5]; // Marche pas pour des trucs aléatoires
	cout << encode_data.back() << endl;
	//encode_data.front()[5] = !encode_data.front()[5]; 
	//cout << encode_data.front() << endl;
	cout << endl;

	// Decode Hamming (7,4)
	decode_data = HammingDecoding(encode_data);
}



