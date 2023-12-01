#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#pragma warning (disable: 4996)

int arif_summ(int n) {
    return ((1 + n) * n) / 2;
}

int convert_to_DEC(char* binary_data, int i, int encryption) {
    int val = 0;
    for (int k = 0; k < encryption; k++) {
        if (binary_data[((i + 1) * encryption) - 1 - k] == '1') {
            val += pow(2, k);
        }
    }
    return val;
}

int read(char* fileName, int** MAIN_condition, int** MAIN_psi) {
    FILE* inputFile;
    inputFile = fopen(fileName, "r");
    int encryption = 0; // кодировка P2 (кол-во битов)
    if (inputFile != NULL) {
        int logic = 0;
        char PS;
        int vars_num = 0; //
        fscanf(inputFile, "%d ", &encryption);
        char* begin_condition = (char*)calloc(encryption, sizeof(char));
        char* binary_data = (char*)calloc(((pow(2, encryption) * 2) * encryption), sizeof(char));
        int* psi = (int*)calloc((pow(2, encryption) * 2), sizeof(int));
        int* conditions = (int*)calloc((pow(2, encryption) * 2), sizeof(int));
        for (int i = 0; i < encryption; i++) {
            fscanf(inputFile, "%c", &begin_condition[i]);
        }
        free(begin_condition);
        for (int i = 0; i < encryption; i++) {
            fscanf(inputFile, "%d ", &logic);
            fscanf(inputFile, "%d ", &vars_num);
            for (int j = 0; j < (pow(2, encryption) * 2); j++) {
                fscanf(inputFile, "%c ", &binary_data[(j * encryption) + i]);
            }
        }
        fscanf(inputFile, "%d ", &logic);
        fscanf(inputFile, "%d ", &vars_num);
        for (int i = 0; i < (pow(2, encryption) * 2); i++) {
            fscanf(inputFile, "%c", &PS);
            if (PS == '1') {
                psi[i] = 1;
            }
            else { psi[i] = 0; }
        }
        for (int i = 0; i < (pow(2, encryption) * 2); i++) {
            conditions[i] = convert_to_DEC(binary_data, i, encryption);
        }
        *MAIN_condition = conditions;
        *MAIN_psi = psi;
        fclose(inputFile);
        free(binary_data);
    }
    return encryption;
}

void logic(int* condition, int* psi, int encryption, char* file_Name) {
    int conditions_quantity = pow(2, encryption);
    int* table = (int*)calloc(arif_summ(conditions_quantity - 1), sizeof(int));
    for (int i = 0; i < conditions_quantity; i++) {
        for (int j = i + 1; j < conditions_quantity; j++) {
            if (psi[i * 2] != psi[j * 2] || psi[(i * 2) + 1] != psi[(j * 2) + 1]) {
                printf("%d ", arif_summ(j) + i - j);
                table[arif_summ(j) + i - j] = 1;
            }
        }
    }

    for (int i = 0; i < arif_summ(conditions_quantity - 1); i++) {
        printf("\n%d", table[i]);
    }
    int qqq = 0;
    int k = 0;
    int t = 0;
    int double_check = 1; // проверка и по 1
    int one_more_time = 1; // еще раз проверям таблицу
    while (one_more_time) {
        qqq++;
        one_more_time = 0;
        for (int i = 0; i < conditions_quantity; i++) {
            for (int j = i + 1; j < conditions_quantity; j++) {
                double_check = 1;
                if (!table[arif_summ(j) + i - j]) {
                    k = condition[i * 2];  //проверка по 0
                    t = condition[j * 2];
                    if (k > t) {
                        if (table[arif_summ(k) + t - k]) {
                            table[arif_summ(j) + i - j] = 1;
                            double_check = 0;
                            one_more_time = 1;
                        }
                    }
                    else if (t > k) {
                        if (table[arif_summ(t) + k - t]) {
                            table[arif_summ(j) + i - j] = 1;
                            double_check = 0;
                            one_more_time = 1;
                        }
                    }
                    if (double_check) { // проверка по 1
                        k = condition[(i * 2) + 1];
                        t = condition[(j * 2) + 1];
                        if (k > t) {
                            if (table[arif_summ(k) + t - k]) {
                                table[arif_summ(j) + i - j] = 1;
                                one_more_time = 1;
                            }
                        }
                        else if (t > k) {
                            if (table[arif_summ(t) + k - t]) {
                                table[arif_summ(j) + i - j] = 1;
                                one_more_time = 1;
                            }
                        }
                    }
                }
            }
        }
        printf("\n-------------------------------------\n");
        for (int i = 0; i < arif_summ(conditions_quantity - 1); i++) {
            printf("\n%d", table[i]);
        }
    }

    for (int i = 0; i < conditions_quantity; i++) {
        for (int j = i + 1; j < conditions_quantity; j++) {
            if (!table[arif_summ(j) + i - j]) {
                printf("\n%d %d", i, j);
            }
        }
    }
    printf("\niteration table:%d", qqq);

    FILE* fout = NULL;
    fout = fopen(file_Name, "w");
    if (fout != NULL) {
        int no_equals = 1;
        int remain = 0;
        int dec_number = 0;
        char* answerbin = (char*)calloc(encryption, sizeof(char));
        for (int i = 0; i < conditions_quantity; i++) {
            for (int j = i + 1; j < conditions_quantity; j++) {
                if (!table[arif_summ(j) + i - j]) {
                    no_equals = 0;
                    dec_number = i;
                    remain = 0;
                    for (int l = 0; l < encryption; l++) {
                        remain = dec_number % ((int)pow(2, l + 1));
                        dec_number -= remain;
                        if (remain) {
                            answerbin[encryption - 1 - l] = '1';
                        }
                        else { answerbin[encryption - 1 - l] = '0'; }
                    }
                    for (int l = 0; l < encryption; l++) {
                        fprintf(fout, "%c", answerbin[l]);
                    }
                    fprintf(fout, "%c", ' ');
                    remain = 0;
                    dec_number = j;
                    for (int l = 0; l < encryption; l++) {
                        remain = dec_number % ((int)pow(2, l + 1));
                        dec_number -= remain;
                        if (remain) {
                            answerbin[encryption - 1 - l] = '1';
                        }
                        else { answerbin[encryption - 1 - l] = '0'; }
                    }
                    for (int l = 0; l < encryption; l++) {
                        fprintf(fout, "%c", answerbin[l]);
                    }
                    fprintf(fout, "%c", ' ');
                }
            }
        }
        if (no_equals) {
            fprintf(fout, "%d", -1);
        }
        fclose(fout);
    }
}

int main(int argc, char* argv[]) {
	if (argc == 3) {
		int* MAIN_condition;
		int* MAIN_psi;
		int MAIN_encryption;
		MAIN_encryption = read(argv[1], &MAIN_condition, &MAIN_psi);
		logic(MAIN_condition, MAIN_psi, MAIN_encryption, argv[2]);
        free(MAIN_condition);
        free(MAIN_psi);
	} 
}