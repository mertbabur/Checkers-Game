/*
	BÝLGÝ !!!

	dama oynanýrken matris ve dosya uzerinde es zamanli guncellemeler yapilacaktir ...

	-Matris uzerinde ;
		0 -- boþluðu temsil eder ...
		1 -- beyaz taþý temsil eder ...
		2 -- siyah taþý temsil eder ...
		11 -- beyaz dama taþý ...
		22 -- siyah dama taþý ...

*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

int selectedNum; // menü seçimi için ...

int matris[8][8] = { 0 }; // dosyadaki taþlarýn yerleri ayný zamanda matriste de tutmak için ...

char orderderChar[2]; // siranin kimde oldugunu tutar ...
int orderderInt; // sirakimdenin int karsiligi ...
int rivalPieceInt; // rakip tasi tutar(matristeki karsiligini) ...

int whiteCount = 0; // tas sayilari ...
int blackCount = 0;

int currentLoc[2]; // kullanýcýdan alýnan koordinatlarý tutacak ... nereden ...
int destination[2]; // kullanýcýdan alýnan koordinatlarý tutacak ... nereye ...

FILE* fptr; // dosya pointeri ...

char command[6]; // nereden
char dest[3]; // nereye  tasi oynatmak istedigimiz yeri tutar ...


void createFile() { // new game yapýldýðýnda ilk dosya düzeni ...

	fptr = fopen("gameTable.txt", "w");
	char temp[25] = "                       \n";
	char temp1[25] = "b  b  b  b  b  b  b  b \n";
	char temp2[25] = "w  w  w  w  w  w  w  w \n";

	fputs(temp, fptr);
	fputs(temp1, fptr);
	fputs(temp1, fptr);
	fputs(temp, fptr);
	fputs(temp, fptr);
	fputs(temp2, fptr);
	fputs(temp2, fptr);
	fputs(temp, fptr);
	fputs("W", fptr);

	fclose(fptr);

}

void showGameBoard() { // ekranda gözükecek oyun tahtasýný oluþurur ...

	char temp[3]; // dosyadaki verileri alýrken dama taþlarýný tutar ...
	int count = 8; // tahtaya 8 7 6 5 4 3 2 1 yazmak için counter ...

	fptr = fopen("gameTable.txt", "r");

	printf("   -----------------------------------------\n");
	for (int i = 1; i < 9; i++) {

		printf(" %d ", count--);

		for (int j = 0; j < 23; j = j + 3) {

			fseek(fptr, (i - 1) * 25 + j, SEEK_SET); // (satir-1)*25 + sütun --> burada 25 bir satirdaki toplam karakter sayisi ...

			fgets(temp, 3, fptr);
			printf("| %s ", temp);

		}

		printf("|\n");
		printf("   -----------------------------------------\n");

	}

	printf("     A    B    C    D    E    F    G    H\n\n");

	fseek(fptr, -1, SEEK_END);
	fgets(temp, 2, fptr);
	printf("Player 1: White\n");
	printf("Player 2: Black\n");
	printf("Play Order : %s\n\n", temp);

	fclose(fptr);

}

void fileToMatris() { // dosyadaki verileri matrise aktarýr ...

	char temp[3]; // dosyadaki taþlarý tutar ...
	fptr = fopen("gameTable.txt", "r");
	int row = 0;
	int column = 0; // matris icin satir ve sutunlari tutar ...
	whiteCount = 0; // beyaz tas sayisini tutar ...
	blackCount = 0; // siyah tas sayisini tutar ...

	for (int i = 1; i < 9; i++) {
		for (int j = 0; j < 22; j = j + 3) {

			fseek(fptr, (i - 1) * 25 + j, SEEK_SET); // (satir-1)*25 + sütun --> burada 25 bir satirdaki toplam karakter sayisi ...
			fgets(temp, 2, fptr);

			if (strcmp(temp, " ") == 0) // bosluk ise matrise 0 yerlestir ...
				matris[row][column] = 0;

			else if (strcmp(temp, "w") == 0) { // w ise matrise 1 yerlestir ...
				matris[row][column] = 1;
				whiteCount++;
			}

			else if (strcmp(temp, "b") == 0) {  // b ise matrise 2 yerlestir ...
				matris[row][column] = 2;
				blackCount++;
			}

			else if (strcmp(temp, "W") == 0) {  // W ise matrise 11 yerlestir ...
				matris[row][column] = 11;
				whiteCount++;
			}

			else {  // B ise matrise 22 yerlestir ...
				matris[row][column] = 22;
				blackCount++;
			}
			column++;

		}

		//satir++;

	}

	fseek(fptr, -1, SEEK_END);
	fgets(orderderChar, 2, fptr); // siranin kimde oldugunu tutmak icin ...

	if (strcmp(orderderChar, "W") == 0) { // sira beyaz tasta ise 
		orderderInt = 1; // sira beyaz tas
		rivalPieceInt = 2; // rakip siyah tas
	}
	else { // sira siyah tasta ise
		orderderInt = 2; // sira siyah tas
		rivalPieceInt = 1; // rakip beyaz tas
	}

	fclose(fptr);

}

void commandToConvertNum() { // koordinati matriste kontrol edebilmek icin int donusturur ...

	currentLoc[0] = 9 - ((int)command[1] - 48);
	currentLoc[1] = (int)command[0] - 96;

	destination[0] = 9 - ((int)command[3] - 48);
	destination[1] = (int)command[2] - 96;

}

bool commandControl(int sira, int damaSira) { // oyuncu kendi tasi icin dogru koordinat girdi mi ...

	// damaSira --> sira kimdeyse onun matristeki dama tasinin karsiligini tutar (11 veya 22) ...
	// sira --> sira kimdeyse onun matristeki normal tasinin karsiligini tutar (1 veya 2) ...

	int row = currentLoc[0] - 1;
	int col = currentLoc[1] - 1; // matriste tasi bulmak icin row ve columnlar ...

	if (matris[row][col] == sira || matris[row][col] == damaSira) 
		return true;

	return false;

}

void updateFile(int row, int col, int newVal) { // hamlelerden sonra dosyayi guncelleme kismi ...

	// row ve col--> guncellenen yerin koordinatlari ...
	// koordinatta bulunan ne varsa degisen degiskeni ile degistirilecek ...

	char temp[3]; // dosyaya kaydedilecek karakteri tutacak ...

	fptr = fopen("gameTable.txt", "r+");

	int calculate = (row * 25) + (col * 3); // her satirda \n ile birlikte 25 karakter var ...
											// ve her 3 karakterde bir dosya da baska bir oyun tasina gecis yapacagiz ...

	fseek(fptr, calculate, SEEK_SET);

	if (newVal == 0) { // eger 0 ise bosluk koyulacak ...
		strcpy(temp, "  ");
		fputs(temp, fptr);
	}
	else if (newVal == 1) { // 1 ise w ...
		strcpy(temp, "w ");
		fputs(temp, fptr);
	}
	else if (newVal == 2) { // 2 ise b ...
		strcpy(temp, "b ");
		fputs(temp, fptr);
	}
	else if (newVal == 11) { // 11 ise WW ...
		strcpy(temp, "WW");
		fputs(temp, fptr);
	}
	else if (newVal == 22) { // 22 ise BB ...
		strcpy(temp, "BB");
		fputs(temp, fptr);
	}
	else {

		// bir sey yapma ...

	}

	fclose(fptr);

}

void updateFile_rival(char* siraKimde) { // sira icin dosyayi gunceller ...

	fptr = fopen("gameTable.txt", "r+");

	fseek(fptr, 200, SEEK_SET);

	fputs(siraKimde, fptr);

	fclose(fptr);

}

void moveFileWrite(char order[2], int fileMode) { // dosyaModu == 1 ise temiz dosya, dosyaModu == 2 ise uzerine yaz ...

	char temp[8]; // yazilacak coordinati tutar ...

	if (fileMode == 1) { // dosya modu 1 ise yazmak icin yeni dosya ac ...
		fptr = fopen("coordinat.txt", "w");

	}
	else { // degilse append modda ac ...
		fptr = fopen("coordinat.txt", "a");
	}

	if (strcmp("W", order) == 0) { // sira beyazda ise
		strcpy(temp, "w:");
		strcat(temp, command);
		strcat(temp, "\n");
		fputs(temp, fptr);
	}
	else if (strcmp("B", order) == 0) { // sira siyahta ise

		strcpy(temp, "b:");
		strcat(temp, command);
		strcat(temp, "\n");
		fputs(temp, fptr);

	}
	else {
		// devam ...
	}

	fclose(fptr);

}

bool movePiece() { // tas hareket ettirme komutu ...

	int rowCurr = currentLoc[0] - 1;
	int colCurr = currentLoc[1] - 1; // matriste tasi bulmak icin row ve columnlar ...

	int rowDest = destination[0] - 1;
	int colDest = destination[1] - 1; // gidecegi yerin matris indisleri ...

	int movePiece = matris[rowCurr][colCurr]; // hangi tasi oynancagini tutar ...

	if (movePiece == 1 || movePiece == 2) { // normal tas icin hareket ...

		if ((rowCurr - 1) == rowDest && colCurr == colDest && matris[rowDest][colDest] == 0 && movePiece == 1) {  // yatay yön hareket beyaz icin ...
			
		// hamle yapilacak beyaz tasin onu bos olacak ve hamle yapilacak yerin column = gidilecek yerin column ve hamle tasi beyaz tas olacak ... 
																												
			matris[rowCurr][colCurr] = 0; // hamle yerine 0 koy
			updateFile(rowCurr, colCurr, 0);

			if (rowDest == 0) { // row 0 gelmiþse beyaz tas dama tasi olur ... for white ...
				matris[rowCurr - 1][colCurr] = 11;
				updateFile(rowCurr - 1, colCurr, 11);
			}
			else { // gidilecek yerde dama tasi olmamissa ...
				matris[rowCurr - 1][colCurr] = 1;
				updateFile(rowCurr - 1, colCurr, 1);
			}

			return true;

		}

		else if ((rowCurr + 1) == rowDest && colCurr == colDest && matris[rowDest][colDest] == 0 && movePiece == 2) { // yatay yön hareket siyah icin ...

			// hamle yapilacak siyah tasin onu bos olacak ve hamle yapilacak yerin column = gidilecek yerin column ve hamle tasi siyah tas olacak ...

			matris[rowCurr][colCurr] = 0; // hamle yerine 0 koy ...
			updateFile(rowCurr, colCurr, 0);

			if (rowDest == 7) { // gidilecek row 7 ise dama tasi olur ... for black ...
				matris[rowCurr + 1][colCurr] = 22;
				updateFile(rowCurr + 1, colCurr, 22);
			}
			else { // gidilecek yerde  dama tasi olmayacaksa ...
				matris[rowCurr + 1][colCurr] = 2;
				updateFile(rowCurr + 1, colCurr, 2);
			}

			return true;

		}


		else if ((colCurr - 1) == colDest && rowCurr == rowDest && matris[rowDest][colDest] == 0) { // sola hareket beyaz ve siyah icin ortak ...

			// hamle yapilacak tasin onu bos olacak ve hamle yapilacak yerin row = gidilecek yerin row ...

			matris[rowCurr][colCurr] = 0; // hamle yerine 0 koy ...
			updateFile(rowCurr, colCurr, 0);

			if (movePiece == 1) { // hamle tasi beyaz ise ...
				matris[rowDest][colDest] = 1; // gidilecek yere 1 koy ...
				updateFile(rowDest, colDest, 1);
			}
			else { // hamle tasi siyah ise ...
				matris[rowDest][colDest] = 2; // gidilecek yere 2 koy ...
				updateFile(rowDest, colDest, 2);
			}

			return true;

		}
		else if ((colCurr + 1) == colDest && rowCurr == rowDest && matris[rowDest][colDest] == 0) { // saga hareket beyaz ve siyah icin ortak ...

			// hamle yapilacak tasin onu bos olacak ve hamle yapilacak yerin row = gidilecek yerin row ...

			matris[rowCurr][colCurr] = 0; // hamle yerine 0 koy ...
			updateFile(rowCurr, colCurr, 0);

			if (movePiece == 1) { // hamle tasi beyaz ise ...
				matris[rowDest][colDest] = 1; // gidilecek yere 1 koy ...
				updateFile(rowDest, colDest, 1);
			}
			else { // hamle tasi siyah ise ...
				matris[rowDest][colDest] = 2; // gidilecek yere 2 koy ...
				updateFile(rowDest, colDest, 2);
			}

			return true;

		}

		else {

			return false; // gecersiz hamle girildi ...

		}


	}


	else if (movePiece == 11 || movePiece == 22) { // dama tasi icin hareket ... ***

		int counter = 0; // dama tasinin önündeki yerlerin boslugunu sayacak ...

		if (colCurr == colDest && matris[rowDest][colDest] == 0) { // dikey yonde hareket ...

			if (rowCurr - rowDest > 0) { // yukarý yönde hareket ... ***

				for (int i = rowCurr - 1; i >= rowDest; i--) { //oynanan yerin bir önünden oyananacak yere kadar bosluk var mi say ...

					if (matris[i][colCurr] == 0) // bosluk varsa 
						counter++;

				}

				if (counter == abs(rowCurr - rowDest)) { // dama tasinin gidecegi yere kadar bosluk kontrolu ...

					matris[rowCurr][colCurr] = 0; // hamle yerine yere 0 koy ...
					updateFile(rowCurr, colCurr, 0);

					if (movePiece == 11) { // beyaz dama ise ...
						matris[rowDest][colDest] = 11; // gidilecek yere 11 koy ...
						updateFile(rowDest, colDest, 11);
					}
					else { // siyah dama ise ...
						matris[rowDest][colDest] = 22; //gidilecek yere 22 koy ...
						updateFile(rowDest, colDest, 22);
					}

					return true;

				}

				return false; // yukaridaki islem gerceklesmezse ...

			}

			else { // asagi yonde hareket ...

				if (rowCurr - rowDest < 0) { // asagi yönde hareket ... ***

					for (int i = rowCurr + 1; i <= rowDest; i++) { //oynanan yerin bir önünden oyananacak yere kadar bosluk var mi say ...

						if (matris[i][colCurr] == 0) // bosluk varsa ...
							counter++;

					}

					if (counter == abs(rowCurr - rowDest)) { // dama tasinin gidecegi yere kadar bosluk kontrolu ...

						matris[rowCurr][colCurr] = 0; // hamle yerine 0 koy ...
						updateFile(rowCurr, colCurr, 0);

						if (movePiece == 11) { // beyaz dama ise ...
							matris[rowDest][colDest] = 11; // gidilecek yere 11 koy ...
							updateFile(rowDest, colDest, 11);
						}
						else { // siyah dama ise ...
							matris[rowDest][colDest] = 22; // gidilecek yere 22 koy ...
							updateFile(rowDest, colDest, 22);
						}

						return true;

					}

					return false; // yukaridaki islem gerceklesmezse ...

				}

			} // else sonu





		}
		else if (rowCurr == rowDest && matris[rowDest][colDest] == 0) { // yatay yonde hareket ... ***



			if (colCurr - colDest > 0) { // sagdan sola dogru hareket ... ***

				for (int i = colCurr - 1; i >= colDest; i--) { //oynanan yerin bir önünden oyananacak yere kadar bosluk var mi say ...

					if (matris[rowCurr][i] == 0)
						counter++;

				}

				if (counter == abs(colCurr - colDest)) { // dama tasinin gidecegi yere kadar bosluk kontrolu ...

					matris[rowCurr][colCurr] = 0; // hamle yerine 0 koy ...
					updateFile(rowCurr, colCurr, 0);

					if (movePiece == 11) { // beyaz dama ise ...
						matris[rowDest][colDest] = 11; // gidilecek yere 11 koy ...
						updateFile(rowDest, colDest, 11);
					}
					else { // siyah dama ise ...
						matris[rowDest][colDest] = 22; // gidilecek yere 22 koy ...
						updateFile(rowDest, colDest, 22);
					}

					return true;
				}

				return false; // yukaridaki islem gerceklesmezse ...
			}

			else { // soldan saga dogru hareket ... ***

				for (int i = colCurr + 1; i <= colDest; i++) { //oynanan yerin bir önünden oyananacak yere kadar bosluk var mi say ...

					if (matris[rowCurr][i] == 0)
						counter++;

				}

				if (counter == abs(colCurr - colDest)) { // dama tasinin gidecegi yere kadar bosluk kontrolu ...

					matris[rowCurr][colCurr] = 0; // hamle yerine 0 koy ...
					updateFile(rowCurr, colCurr, 0);

					if (movePiece == 11) { // beyaz dama ise ...
						matris[rowDest][colDest] = 11; // gidilecek yere 11 koy ...
						updateFile(rowDest, colDest, 11);
					}
					else { // siyah dama ise ...
						matris[rowDest][colDest] = 22; // gidilecek yere 22 koy ...
						updateFile(rowDest, colDest, 22);
					}

					return true;

				}

				return false; // yukaridaki islem gerceklesmezse ...

			}



		}


		else {

			return false; // gecersiz hamle ...

		}





	}
	else {

		return false; //gecersiz hamle ...

	}



}

bool eatRival() { // rakip tas yeme komutu ...

	int rowCurr = currentLoc[0] - 1;
	int colCurr = currentLoc[1] - 1; // matriste tasi bulmak icin row ve columnlar ...

	int rowDest = destination[0] - 1;
	int colDest = destination[1] - 1; // gidecegi yerin matris indisleri ...

	int movePiece = matris[rowCurr][colCurr]; // hangi tasi oynancagini tutar ...

	if (movePiece == 1 || movePiece == 11) {// beyaz icin ...

		if (movePiece == 1) { // normal beyaz tas icin yeme ...

			if ((matris[rowCurr - 1][colCurr] == 2 || matris[rowCurr - 1][colCurr] == 22) && matris[rowCurr - 2][colCurr] == 0) { 

				// beyazýn onunde rakip var mi ve beyazýn 2 onunde bosluk var mi ...

				matris[rowCurr][colCurr] = 0; // hamle yerine 0 koy ...
				matris[rowCurr - 1][colCurr] = 0; // yenilen tas yerine 0 koy ...

				if (rowCurr - 2 == 0) { // beyaz tas dama oldu mu ? ...
					matris[rowCurr - 2][colCurr] = 11;
					updateFile(rowCurr - 2, colCurr, 11);
				}
				else { // dama olmadiysa ...
					matris[rowCurr - 2][colCurr] = 1;
					updateFile(rowCurr - 2, colCurr, 1);
				}

				blackCount--; // siyah tas sayisini azalt ...

				updateFile(rowCurr, colCurr, 0);
				updateFile(rowCurr - 1, colCurr, 0);

				return true;

			}

			if ((matris[rowCurr][colCurr + 1] == 2 || matris[rowCurr][colCurr + 1] == 22) && matris[rowCurr][colCurr + 2] == 0 && colCurr - colDest < 0) { // son ifade ile yonu belirttik ...

				// beyazýn onunde rakip var mi ve beyazýn 2 onunde bosluk var mi ...

				matris[rowCurr][colCurr] = 0; // hamle yerine 0 koy ...
				matris[rowCurr][colCurr + 1] = 0; // yenilen tas yerine 0 koy ...
				matris[rowCurr][colCurr + 2] = 1; // gidilen yere 1 koy ...

				blackCount--; // siyah tas sayisini azalt ...

				updateFile(rowCurr, colCurr, 0);
				updateFile(rowCurr, colCurr + 1, 0);
				updateFile(rowCurr, colCurr + 2, 1);

				return true;

			}

			if ((matris[rowCurr][colCurr - 1] == 2 || matris[rowCurr][colCurr - 1] == 22) && matris[rowCurr][colCurr - 2] == 0 && colCurr - colDest > 0) { // son ifade yonu belirttik ...

				// beyazýn onunde rakip var mi ve beyazýn 2 onunde bosluk var mi ...

				matris[rowCurr][colCurr] = 0; // hamle yerine 0 koy ...
				matris[rowCurr][colCurr - 1] = 0; // yenilen tas yerine 0 koy ...
				matris[rowCurr][colCurr - 2] = 1; // gidilecek yere 1 koy ...

				blackCount--; // siyah tas sayisini azalt ...

				updateFile(rowCurr, colCurr, 0);
				updateFile(rowCurr, colCurr - 1, 0);
				updateFile(rowCurr, colCurr - 2, 1);

				return true;

			}


		}
		else { // beyaz dama tasi icin yeme ...


			int counter = 0; // dama tasinin önündeki yerlerin boslugu ... ve rakip tas ...

			if (colCurr == colDest && matris[rowDest][colDest] == 0) { // dikey yonde hareket ...

				if (rowCurr - rowDest > 0) { // yukarý yönde hareket ... ***

					for (int i = rowCurr - 1; i >= rowDest; i--) { //oynanan yerin bir önünden oyananacak yere kadar bosluk ve rakip tas var mi say ...

						if (matris[i][colCurr] == 0 || matris[i][colCurr] == 2 || matris[i][colCurr] == 22) // varsa ...
							counter++;

					}

					if (counter == abs(rowCurr - rowDest)) { // gideceði yol müsait mi ...

						matris[rowCurr][colCurr] = 0; // hamle yerine 0 koy ...
						matris[rowDest + 1][colDest] = 0; // yenilen tas yerine 0 koy ...
						matris[rowDest][colDest] = 11; // gidilen yere 11 koy ...
						
						blackCount--; // siyah tas sayisini azalt ...
						
						updateFile(rowCurr, colCurr, 0);
						updateFile(rowDest + 1, colDest, 0);
						updateFile(rowDest, colDest, 11);

						return true;

					}

					return false; // yukaridaki islemi yapmazsa ...

				}

				else { // asagi yonde hareket ...

					if (rowCurr - rowDest < 0) { // asagi yönde hareket ... ***

						for (int i = rowCurr + 1; i <= rowDest; i++) { //oynanan yerin bir önünden oyananacak yere kadar bosluk var mi say ...

							if (matris[i][colCurr] == 0 || matris[i][colCurr] == 2 || matris[i][colCurr] == 22) // varsa ...
								counter++;

						}

						if (counter == abs(rowCurr - rowDest)) { // dama tasinin gidecegi yere kadar bosluk kontrolu ...

							matris[rowCurr][colCurr] = 0; // hamle yerine 0 koy ...
							matris[rowDest - 1][colDest] = 0; // yenilen tasin yerine 0 koy ...
							matris[rowDest][colDest] = 11; // gidilecek yere 11 koy ...
							
							blackCount--; // siyah tas sayisini azalt ...

							updateFile(rowCurr, colCurr, 0);
							updateFile(rowDest - 1, colDest, 0);
							updateFile(rowDest, colDest, 11);

							return true;

						}

						return false; // yukaridaki islemi yapmazsa ...

					}

				}

			}
			else if (rowCurr == rowDest && matris[rowDest][colDest] == 0) { // yatay yonde hareket ... ***



				if (colCurr - colDest > 0) { // sagdan sola dogru hareket ... ***

					for (int i = colCurr - 1; i >= colDest; i--) { //oynanan yerin bir önünden oyananacak yere kadar bosluk var mi say ...

						if (matris[rowCurr][i] == 0 || matris[rowCurr][i] == 2 || matris[rowCurr][i] == 22)
							counter++;

					}

					if (counter == abs(colCurr - colDest)) { // dama tasinin gidecegi yere kadar bosluk kontrolu ...

						matris[rowCurr][colCurr] = 0; // hamle yerine o koy ...
						matris[rowDest][colDest + 1] = 0; // yenilen tas yerine 0 koy ...
						matris[rowDest][colDest] = 11; // gidilecek yere 11 koy ...
						
						blackCount--; // siyah tas sayisini azalt ...

						updateFile(rowCurr, colCurr, 0);
						updateFile(rowDest, colDest + 1, 0);
						updateFile(rowDest, colDest, 11);

						return true;


					}

					return false; // yukaridaki islem yapýlmazsa ...

				}

				else { // soldan saga dogru hareket ... ***

					for (int i = colCurr + 1; i <= colDest; i++) { //oynanan yerin bir önünden oyananacak yere kadar bosluk var mi say ...

						if (matris[rowCurr][i] == 0 || matris[rowCurr][i] == 2 || matris[rowCurr][i] == 22) // varsa ...
							counter++;

					}

					if (counter == abs(colCurr - colDest)) { // dama tasinin gidecegi yere kadar bosluk kontrolu ...

						matris[rowCurr][colCurr] = 0; // hamle yerine 0 koy ...
						matris[rowDest][colDest - 1] = 0; // yenilen tasin yerine 0 koy ...
						matris[rowDest][colDest] = 11; // gidilecek yere 11 koy ...
						
						blackCount--; // siyah tas sayisini azalt ...

						updateFile(rowCurr, colCurr, 0);
						updateFile(rowDest, colDest - 1, 0);
						updateFile(rowDest, colDest, 11);

						return true;

					}

					return false; // yukaridaki islem yapilmazsa ...

				}



			}


			else {

				return false; // gecersiz hamle ...

			}

		}

	}
	else if (movePiece == 2 || movePiece == 22) { // siyah

		if (movePiece == 2) { // normal siyah taþ ...

			if ((matris[rowCurr + 1][colCurr] == 1 || matris[rowCurr + 1][colCurr] == 11) && matris[rowCurr + 2][colCurr] == 0) { 

				// siyah tasin onunde rakip olacak ve siyah tasin 2 onu bos olacak ...

				matris[rowCurr][colCurr] = 0; // hamle yerine 0 koy ...
				updateFile(rowCurr, colCurr, 0);

				matris[rowCurr + 1][colCurr] = 0; // yenilecek tas yerine 0 koy ...
				updateFile(rowCurr + 1, colCurr, 0);

				if (rowCurr + 2 == 7) { // tas dama oldu mu ? ...
					matris[rowCurr + 2][colCurr] = 22;
					updateFile(rowCurr + 2, colCurr, 22);
				}
				else { // dama degilse ...
					matris[rowCurr + 2][colCurr] = 2;
					updateFile(rowCurr + 2, colCurr, 2);
				}

				whiteCount--; // beyaz tas sayisini azalt ...

				return true;

			}

			if ((matris[rowCurr][colCurr + 1] == 1 || matris[rowCurr][colCurr + 1] == 11) && matris[rowCurr][colCurr + 2] == 0 && colCurr - colDest < 0) { // son ifade ile yönü belirttik ...

				// siyah tasin onunde rakip olacak ve siyah tasin 2 onu bos olacak ...

				matris[rowCurr][colCurr] = 0; // hamle yerine 0 koy ...
				matris[rowCurr][colCurr + 1] = 0; // yenilen tasin yerine 0 koy ..
				matris[rowCurr][colCurr + 2] = 2; // gidilecek yere 2 koy ...
				
				whiteCount--; // beyaz tas sayisini azalt ...

				updateFile(rowCurr, colCurr, 0);
				updateFile(rowCurr, colCurr + 1, 0);
				updateFile(rowCurr, colCurr + 2, 2);

				return true;

			}

			if ((matris[rowCurr][colCurr - 1] == 1 || matris[rowCurr][colCurr - 1] == 11) && matris[rowCurr][colCurr - 2] == 0 && colCurr - colDest > 0) { // son ifadae ile yönü belirttik ...

				// siyah tasin onunde rakip olacak ve siyah tasin 2 onu bos olacak ...

				matris[rowCurr][colCurr] = 0; // hamle yerine 0 koy ...
				matris[rowCurr][colCurr - 1] = 0; // yenilen tasin yerine 0 koy ...
				matris[rowCurr][colCurr - 2] = 2; // gidilecek yere 2 koy ...
				whiteCount--;

				updateFile(rowCurr, colCurr, 0);
				updateFile(rowCurr, colCurr - 1, 0);
				updateFile(rowCurr, colCurr - 2, 2);

				return true;

			}

		}

		else { // siyah dama tasi icin ...


			int counter = 0; // dama tasinin önündeki yerlerin boslugu ... ve rakip tas ...

			if (colCurr == colDest && matris[rowDest][colDest] == 0) { // dikey yonde hareket ...

				// gidilen yerin column u = hamle yerinin column ve gidilecek yer 0 olacak ...

				if (rowCurr - rowDest > 0) { // yukarý yönde hareket ... ***

					for (int i = rowCurr - 1; i >= rowDest; i--) { //oynanan yerin bir önünden oyananacak yere kadar bosluk ve rakip tas var mi say ...

						if (matris[i][colCurr] == 0 || matris[i][colCurr] == 1 || matris[i][colCurr] == 11) // varsa ...
							counter++;

					}

					if (counter == abs(rowCurr - rowDest)) { // gideceði yol müsait mi ...

						matris[rowCurr][colCurr] = 0; // hamle yerine 0 koy ...
						matris[rowDest + 1][colDest] = 0; // yenilen tasin yerine 0 koy ...
						matris[rowDest][colDest] = 22; // gidilecek yere 22 koy ...
						
						whiteCount--; // beyaz tas sayisini azalt ...

						updateFile(rowCurr, colCurr, 0);
						updateFile(rowDest + 1, colDest, 0);
						updateFile(rowDest, colDest, 22);

						return true;

					}

					return false; // yukaridaki islem yapýlmazsa ...

				}

				else { // asagi yonde hareket ...

					if (rowCurr - rowDest < 0) { // asagi yönde hareket ... ***

						for (int i = rowCurr + 1; i <= rowDest; i++) { //oynanan yerin bir önünden oyananacak yere kadar bosluk var mi say ...

							if (matris[i][colCurr] == 0 || matris[i][colCurr] == 1 || matris[i][colCurr] == 11) // varsa
								counter++;

						}

						if (counter == abs(rowCurr - rowDest)) { // dama tasinin gidecegi yere kadar bosluk kontrolu ...

							matris[rowCurr][colCurr] = 0; // hamle yerine 0 koy ...
							matris[rowDest - 1][colDest] = 0; // yenilen tasin yerine 0 koy ...
							matris[rowDest][colDest] = 22; // gidilecek yere 22 koy ...

							whiteCount--; // beyaz tas sayisini azalt ...

							updateFile(rowCurr, colCurr, 0);
							updateFile(rowDest - 1, colDest, 0);
							updateFile(rowDest, colDest, 22);

							return true;

						}

						return false; // yukaridaki islem yapýlmazsa ...

					}

				}


			}
			else if (rowCurr == rowDest && matris[rowDest][colDest] == 0) { // yatay yonde hareket ... ***

				// gidilen yerin row = hamle yerinin row ve gidilecek yerde 0 olacak ...


				if (colCurr - colDest > 0) { // sagdan sola dogru hareket ... ***

					for (int i = colCurr - 1; i >= colDest; i--) { //oynanan yerin bir önünden oyananacak yere kadar bosluk var mi say ...

						if (matris[rowCurr][i] == 0 || matris[rowCurr][i] == 1 || matris[rowCurr][i] == 11) // varsa
							counter++;

					}

					if (counter == abs(colCurr - colDest)) { // dama tasinin gidecegi yere kadar bosluk kontrolu ...

						matris[rowCurr][colCurr] = 0; // hamle yerine 0 koy ...
						matris[rowDest][colDest + 1] = 0; // yenilecek tasin yerine  koy ...
						matris[rowDest][colDest] = 22; // gidilecek yere 22 koy ...

						whiteCount--; // beyaz tas sayisini azalt ...

						updateFile(rowCurr, colCurr, 0);
						updateFile(rowDest, colDest + 1, 0);
						updateFile(rowDest, colDest, 22);

						return true;


					}

					return false; // yukaridaki islem yapilmazsa ...

				}

				else { // soldan saga dogru hareket ... ***

					for (int i = colCurr + 1; i <= colDest; i++) { //oynanan yerin bir önünden oyananacak yere kadar bosluk var mi say ...

						if (matris[rowCurr][i] == 0 || matris[rowCurr][i] == 1 || matris[rowCurr][i] == 11) // varsa 
							counter++;

					}

					if (counter == abs(colCurr - colDest)) { // dama tasinin gidecegi yere kadar bosluk kontrolu ...

						matris[rowCurr][colCurr] = 0; // hamle yerine 0 koy ...
						matris[rowDest][colDest - 1] = 0; // yenilecek tas yerine 0 koy ...
						matris[rowDest][colDest] = 22; // gidilecek yere 22 koy ...
						
						whiteCount--; // beyaz tas sayisini azalt ...

						updateFile(rowCurr, colCurr, 0);
						updateFile(rowDest, colDest - 1, 0);
						updateFile(rowDest, colDest, 22);

						return true;

					}

					return false; // yukaridaki hamle yapilmazsa ...

				}



			}


			else {

				return false; // gecersiz hamle ...

			}


		}


	}
	else { // bos koordainat secilirse ... hamleTasi == 0 ...

		return false; // gecersiz hamle ...

	}

}

bool yenecekTasVarMi(char* siraKimde) {

	int controlPiece, controlCheckerPiece, rivalPiece, rivalCheckerPiece;
	int selectedRow, selectedCol; // dongude kontrol edilen tasin row ve col bilgilerini tutacak ...


	if (strcmp("W", siraKimde) == 0) { // sira beyazda ise ...
		controlPiece = 1;
		controlCheckerPiece = 11;
		rivalPiece = 2;
		rivalCheckerPiece = 22;

	}
	else { // sira siyahta ise ...
		controlPiece = 2;
		controlCheckerPiece = 22;
		rivalPiece = 1;
		rivalCheckerPiece = 11;

	}


	for (int i = 0; i < 8; i++) {  // tüm matrisi dolas ...
		for (int j = 0; j < 8; j++) {

			if (matris[i][j] == controlPiece) { // normal tas ise ...

				// dikey yonlu kontrol ...
				if (controlPiece == 1) { // beyaz tas icin yukari yonlu kontrol ... normal beyaz sadece yukari gider ...

					if (matris[i - 2][j] == 0 && (matris[i - 1][j] == rivalPiece || matris[i - 1][j] == rivalCheckerPiece)) {
						// hamle tasinin bir onu rakip tas ve iki onu bossa ...
						return true;
					}


				}
				else { // siyah tas icin asagi yonlu kontrol ... normal siyah sadece asagi gider ...

					if (matris[i + 2][j] == 0 && (matris[i + 1][j] == rivalPiece || matris[i + 1][j] == rivalCheckerPiece)) {
						// hamle tasinin bir onu rakip tas ve iki onu bossa ...
						return true;
					}


				}
				// yatay yonlu kontrol ...
				if ((matris[i][j - 2] == 0 && (matris[i][j - 1] == rivalPiece || matris[i][j - 1] == rivalCheckerPiece) && (j - 2 >= 0 && j - 1 >= 0)) ||
					(matris[i][j + 2] == 0 && (matris[i][j + 1] == rivalPiece || matris[i][j + 1] == rivalCheckerPiece) && (j + 2 <= 7 && j + 1 <= 7))
					) {																									// þu kisim kenar kontrolu ...
						// hamle tasinin bir onu rakip tas ve iki onu bossa ...

					return true;

				}



			}
			else if (matris[i][j] == controlCheckerPiece) { // dama tasi ise ...

				selectedRow = i;
				selectedCol = j;


				// dama tasinda : hamle tasi nerdeyse sagi solu asagý yukari tum gidilebilecek yerler kontrol edilecek ...

				// yukariya dogru kontrol ...
				
				int count = 0; // dongude 0 sayýsýný tutacak ...

				for (int t = 0; t < selectedRow; t++) {

					if (matris[t][selectedCol] == 0 && (matris[t + 1][selectedCol] == rivalPiece || matris[t + 1][selectedCol] == rivalCheckerPiece)) {
						
						// gidilecek yer bos ve gidilecek yerin bir arkasinda rakip tas olacak ...

						count = 0;
						
						for (int k = t + 2; k < selectedRow; k++) { // hamle yerinden gidilecek yere kadar 0 kontrolu ...

							if (matris[k][selectedCol] == 0) { 
								count++;
							}


						}

						if (count == (selectedRow - (t + 2))) // 0 sayýsý saglandiysa ...
							return true;

					}


				}
				// yukariya dogru kontrol ...


					//asagi dogru kontrol ...

				

				for (int t = 7; t > selectedRow; t--) {

					if (matris[t][selectedCol] == 0 && (matris[t - 1][selectedCol] == rivalPiece || matris[t - 1][selectedCol] == rivalCheckerPiece)) {
						
						// gidilecek yer bos ve gidilecek yerin bir arkasinda rakip tas olacak ...

						count = 0; // dongude 0 sayýsýný tutacak ...
						
						for (int k = t - 2; k > selectedRow; k--) { // hamle yerinden gidilecek yere kadar 0 kontrolu ...

							if (matris[k][selectedCol] == 0) {
								count++;
							}


						}

						if (count == ((t - 2)) - selectedRow) // 0 sayýsý saglandiysa ...
							return true;

					}


				}





				//asagi dogru kontrol ...



				// saga dogru kontrol ... // köþedeki bb yi unutma
				
				for (int t = 0; t < selectedCol; t++) {

					if (matris[selectedRow][t] == 0 && (matris[selectedRow][t + 1] == rivalPiece || matris[selectedRow][t + 1] == rivalCheckerPiece)) {
						
						// gidilecek yer bos ve gidilecek yerin bir arkasinda rakip tas olacak ...

						count = 0;
						
						for (int k = t + 2; k < selectedCol; k++) { // hamle yerinden gidilecek yere kadar 0 kontrolu ...

							if (matris[selectedRow][k] == 0) {
								count++;
							}


						}

						if (count == (selectedCol - (t + 2))) // 0 sayýsý saglandiysa ...
							return true;

					}


				}

				// saga dogru kontrol ...



				// sola dogru ...

				for (int t = 7; t > selectedCol; t--) {

					if (matris[selectedRow][t] == 0 && (matris[selectedRow][t - 1] == rivalPiece || matris[selectedRow][t - 1] == rivalCheckerPiece)) {

						// gidilecek yer bos ve gidilecek yerin bir arkasinda rakip tas olacak ...

						count = 0;

						for (int k = t - 2; k > selectedCol; k--) { // hamle yerinden gidilecek yere kadar 0 kontrolu ...

							if (matris[selectedRow][k] == 0) {
								count++;
							}


						}

						if (count == ((t - 2)) - selectedCol) // 0 sayýsý saglandiysa ...
							return true;

					}


				}

				// sola dogru ...




			}
			else { // 0 ve ya rakip tas durumlarinda hatayi gidermek icin ...

			}








		}
	}


 }

int isGameFinished() {

	if (whiteCount == 0) { // kazanan siyah ...
		return 2;
	}
	else if (blackCount == 0) { // kazanan beyaz ...
		return 1;
	}
	else if (whiteCount == blackCount && yenecekTasVarMi("W") == false && yenecekTasVarMi("B") == false) {// oyun berabere ...
		return 3;
	}
	else { // oyun devam ediyor ...
		return 0;
	}


}

void last_3_piece(int siraKimde) { // son 3 tas kalmissa hepsini dama yapar ...

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {

			if (matris[i][j] == siraKimde) {

				if (siraKimde == 1) { // beyaz icin ...
					matris[i][j] = 11;
					updateFile(i, j, 11);
				}
				else if (siraKimde == 2) { // siyah icin ...
					matris[i][j] = 22;
					updateFile(i, j, 22);
				}
				else {
					// bir sey yapma ...
				}

			}


		}
	}



}

bool playWhite() {

	if (yenecekTasVarMi("W") == true) { // yenecek tas varsa yeme fonksiyonu ...

		if (eatRival() == true) { // eger rakip basarili bir sekilde yenmis ise ...

			// yeme hamlesi yapildiktan sonra hala yenecek tas varsa devam ...
			if (yenecekTasVarMi("W") == true) { // yenecek tas varsa beyaz oynamaya devam edecek ...
				strcpy(orderderChar, "W"); 
				updateFile_rival(orderderChar);
			}
			else { // yoksa sira siyaha gececek ...
				strcpy(orderderChar, "B");
				updateFile_rival(orderderChar);
			}

			return true;

		}
		else {
			printf("\nWARNING : Invalid coordinat, please enter again command ...\n"); // gecersiz komut girilirse ...
			return true;
		}

	}
	else { // yenecek tas yoksa hareket et ... ve sirayi siyaha gecir ...

		if (movePiece() == true) { // hareket basarili bir sekilde gerceklesmis ise ...
			strcpy(orderderChar, "B"); // sirayi siyaha gecir ...
			updateFile_rival(orderderChar);
			return true;
		}
		else {
			printf("\nWARNING : Invalid coordinat, please enter again command ...\n"); // gecersiz komut ...
			return false;
		}

	}

}

bool playBlack() {

	if (yenecekTasVarMi("B") == true) { // yenecek tas varsa yeme fonksiyonu ...

		if (eatRival() == true) { // rakip basarili bir sekilde yenmis ise ...

			// hamle yapildiktan sonra hala yenecek tas varsa devam ...
			if (yenecekTasVarMi("B") == true) { // yenecek tas varsa siyah oynamaya devam edecek ...
				strcpy(orderderChar, "B");
				updateFile_rival(orderderChar);
			}
			else { // yoksa sira beyaza gececek ...
				strcpy(orderderChar, "W");
				updateFile_rival(orderderChar);
			}

			return true;

		}
		else {
			printf("\nWARNING : Invalid coordinat, please enter command ...\n"); // gecersiz komut ...
			return false;
		}

	}
	else { // yenecek tas yoksa hareket et ... ve sirayi beyaza gecir ...

		if (movePiece() == true) { // hareket basarili bir sekilde gerceklesmis ise ...
			strcpy(orderderChar, "W"); // sirayi beyaza gecir ...
			updateFile_rival(orderderChar);
			return true;
		}
		else {
			printf("\nWARNING : Invalid coordinat, please enter command ...\n"); // gecersiz komut ...
			return false;
		}

	}


}

void playGame() {

	for (;;) {

		printf("1) NEW GAME\n2)CONTINUE GAME\n3)EXIT GAME\nEnter number, please ? ...\n");

		scanf("%d", &selectedNum); // menu secim ...
		system("cls"); // konsolu temizle ...



		if (selectedNum == 1) { // yeni oyun icin temiz dosya ac ...
			createFile();
			moveFileWrite("", 1); // yeni oyun icin bos bir hamle dosyasi acar ... 

		}

		if (selectedNum == 3) { // cikis ...
			break;
		}


		fileToMatris(); // dosyadaki verileri matrise at ...

		for (;;) {
			showGameBoard(); // oyun tahtasini goster ...


			if (isGameFinished() == 2) { // kazanan siyah ise ...
				system("cls");
				printf("\nCONGURALATIONS BLACK\n");
				break;
			}
			else if (isGameFinished() == 1) { // kazanan beyaz ise ...
				system("cls");
				printf("\nCONGURALATIONS WHITE\n");
				break;
			}
			else if (isGameFinished() == 3) { // beraberlik ...
				system("cls");
				printf("\nDRAW\n");
				break;
			}
			else {

				printf("\nIf you want pause the game, you can press ' p ' ...\n");

				if (strcmp("W", orderderChar) == 0) { // sira beyazda ise ...
					printf("\nTeam White please enter your command ...\n", orderderChar);
				}
				if (strcmp("B", orderderChar) == 0) { // sira siyahta ise ...
					printf("\nTeam Black please enter your command ...\n", orderderChar);
				}
				scanf("%s", command); // hamle icin komut al ...
				commandToConvertNum(); // komutu int cevir ...
				
				if (strcmp("p", command) == 0) { // pause ...
					system("cls");
					break;
				}

				if (strcmp("W", orderderChar) == 0) { // beyaz icin ...

					if (commandControl(1, 11) == true) { // gecerli komut girildi mi ...

						if (playWhite() == true) { // basarli sekilde beyaz hamle yapti mi ...
							moveFileWrite("W", 2); // hamleyi dosyaya yaz ...
							system("cls");
						}

					}
					else {
						system("cls");
						printf("\nWARNING : You can use white checker...\n"); // gecersiz hamle ...
					}

					if (whiteCount <= 3) { // son taslari dama yapma kismi ...

						last_3_piece(1);

					}
					else if (blackCount <= 3) { // son taslari dama yapma kismi ...

						last_3_piece(2);

					}
					else {

						// devam

					}

				}
				else { // siyah icin ...

					if (commandControl(2, 22) == true) { // gecerli komut girildi mi ...
						
						if (playBlack() == true) { // siyah basarili bir sekilde oynadi mi ...
							system("cls");
							moveFileWrite("B", 2); // hamleyi dosyaya kaydet ...
						}

					}
					else {
						system("cls");
						printf("\nWARNING : You can use black checker...\n"); // gecersiz hamle ...
					}
					if (whiteCount <= 3) { // son taslari dama yapma kismi ...

						last_3_piece(1);

					}
					else if (blackCount <= 3) { // son taslari dama yapma kismi ...

						last_3_piece(2);

					}
					else {

						// devam

					}

				}
			}

		}



	}








}

int main() {

	playGame();

	system("pause");
	return 0;
}