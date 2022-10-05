#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>

//TODO: lidar com linhas comentadas

using namespace std;
int main (){
	
	ifstream file;
	fstream outfile;
	
	string fileName;
    string myline;
    string format;
	string max;
    string temp;
    
    int rc[2]{};
    int numrows = 0, numcols = 0, maxvalue = 0;
    int j = 0;

    // (1) ler o nome de uma imagem ".pgm" do console
    cout << "Image: ";
    cin >> fileName;
	
	// abre imagem .pgm
	file.open((fileName).c_str()); // c_str() retorna const char * para usar no open
	if (file.is_open()) {
		
		// pega o formato
		getline (file, format);
		cout << "Format: " <<format << endl;
		
		// pega as linhas e colunas
		getline (file, temp);
		string s = "";
		for (char x : temp)
	    {
	        if (x == ' ')
	        {
	        	numcols = stoi(s);
	            s = "";
	        }
	        else {
	            s = s + x;
	        }
	    }
	    numrows = stoi(s);
	    cout << "Rows: " << numrows << " Cols: " << numcols << endl;
	    
	    // pega o valor maximo
	    getline (file, max);
	    maxvalue = stoi(max);
	    cout << "Max value: " << maxvalue << endl;
	    
	    // inicia a matrix
		int matrix[numrows][numcols]{};
		int matrixResult[numrows][numcols]{};
		
		// (2) abrir a imagem e salvar seus dados em uma estrutura de dados que represente uma matriz
		for (int i = 0; i < numrows; ++i){
			j = 0;
			s = "";
			getline (file, myline);
			for (char x : myline)
		    {
		        if (x == ' ')
		        {
		        	matrix[i][j] = stoi(s);
		        	++j;
		            s = "";
		        }
		        else {
		            s = s + x;
		        }
		    }
		    if (!(s == ""))
	    		matrix[i][j] = stoi(s);  
		}
		
		// (3) aplicar um filtro de media
		
		// lidando com as bordas
		int maxrow = numrows-1;
		int maxcol = numcols-1;
		// borda esq superior
		matrixResult[0][0] = (matrix[0][1] + matrix[1][0] + matrix[1][1]) / 3;
		// borda esq inferior
		matrixResult[maxrow][0] = (matrix[maxrow][1] + matrix[maxrow-1][0] + matrix[maxrow-1][1]) / 3;
		// borda dir superior
		matrixResult[0][maxcol] = (matrix[0][maxcol-1] + matrix[1][maxcol] + matrix[1][maxcol-1]) / 3;
		// borda dir inferior
		matrixResult[maxrow][maxcol] = (matrix[maxrow][maxcol-1] + matrix[maxrow-1][maxcol] + matrix[maxrow-1][maxcol-1]) / 3;
		
		//lateral esquerda
		for (int i = 1; i < numrows; ++i){
			matrixResult[i][0] = (matrix[i-1][0] + matrix[i-1][1] + matrix[i][1] + matrix[i+1][1] + matrix[i+1][0]) / 5;
		}
		
		//lateral direita
		for (int i = 1; i < numrows; ++i){
			matrixResult[i][maxcol] = (matrix[i-1][maxcol] + matrix[i-1][maxcol-1] + matrix[i][maxcol-1] + matrix[i+1][maxcol-1] + matrix[i+1][maxcol]) / 5;
		}
		
		// teto
		for (int j = 1; j < numcols; ++j){
			matrixResult[0][j] = (matrix[0][j-1] + matrix[1][j-1] + matrix[1][j] + matrix[0][j+1] + matrix[1][j+1]) / 5;
			}
		
		// chao
		for (int j = 1; j < numcols; ++j){
			matrixResult[maxrow][j] = (matrix[maxrow][j-1] + matrix[maxrow-1][j-1] + matrix[maxrow-1][j] + matrix[maxrow-1][j+1] + matrix[maxrow][j+1]) / 5;
			}
			
		// meio
		for (int i = 1; i < maxrow; ++i){
			for (int j = 1; j < maxcol; ++j){
				matrixResult[i][j] = (matrix[i-1][j] + matrix[i+1][j] + matrix[i][j-1] + matrix[i][j+1] + matrix[i-1][j-1] + matrix[i-1][j+1] + matrix[i+1][j-1] + matrix[i+1][j+1]) / 8;
			}
		}	
		
		// exporta imagem
		string rowline = "";
		string text("Large text stored as a string\n");
		
		FILE *o_file = fopen((fileName + " (Done).pgm").c_str(), "w+");
		
	    if (o_file){
	    	// write formato
	    	rowline = format + "\n";
	    	fwrite(rowline.c_str(), 1, rowline.size(), o_file);
	    	
	    	// write colunas e linhas
	    	rowline = to_string(numcols) + " " + to_string(numrows) + "\n";
	    	fwrite(rowline.c_str(), 1, rowline.size(), o_file);
	    	
	    	// write valor maximo
	    	rowline = max + "\n";
	    	fwrite(rowline.c_str(), 1, rowline.size(), o_file);
	    	
	    	rowline = "";
	    	for (int i = 0; i < numrows; ++i){
				for (int j = 0; j < numcols; ++j){
					rowline = rowline + to_string(matrixResult[i][j]) + " ";
				}
				rowline += "\n";
				fwrite(rowline.c_str(), 1, rowline.size(), o_file);
				rowline = "";
			}
	        cerr << "Salt And Pepper Done!" << endl;
	    }
		
//		printa arquivo
//		while ( file ) { 
//			getline (file, myline);
//			cout << myline << endl;
//		}
	}
	else {
		cout << "Couldn't open file\n";
	}
	
	
	return 0; 
}


		// printa matrix
//		cout << endl;
//		for (int i = 0; i < numrows; ++i){
//			for (int j = 0; j < numcols; ++j){
//				cout << matrix[i][j] << " ";
//			}
//			cout << endl;
//		}
