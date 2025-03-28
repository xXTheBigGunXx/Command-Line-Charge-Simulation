#include <iostream>
#include <cstdlib>
#include <ctime>
#include <array>
#include <utility>
#include <cmath>
#include <windows.h>

const int rows = 38;
const int columns = 77*2;

const int cellCount = 20;

float k = 1.3807 * pow(10, -23);
float singleCharge = 1;

const std::string space = " ";


const std::string positive = "+";
const std::string negative = "-";

struct cellInfo{
    bool positive;
    int xAxys;
    int yAxys;

    float xVec;
    float yVec;

    cellInfo(bool charge, int x, int y, float VecX, float VecY){
        xAxys = x;
        yAxys = y;

        xVec = VecX;
        yVec = VecY;

        positive = charge ? true : false;
    }
    cellInfo(){
        xAxys = 0;
        yAxys = 0;

        xVec = 0.0f;
        yVec = 0.0f;

        positive = false;    
    }
};

void placeValues(std::array<std::array<std::string, columns>, rows> &matrix, cellInfo charges[]);

void printMatrix(std::array<std::array<std::string, columns>, rows> &matrix);

void placeCharges(std::array<std::array<std::string, columns>, rows> &matrix, cellInfo charges[]);

void countChargeChanges(cellInfo charges[]);

float Force(cellInfo charges[], int i, int j);

int main()
{   
    std::srand(std::time(0));
    std::cout<<"\033[2J";
    std::cout << "\033[?25l";

    std::array<std::array<std::string, columns>, rows> matrix;
    cellInfo charges[cellCount];
    placeValues(matrix, charges);

    while(true){
        placeCharges(matrix, charges);

        countChargeChanges(charges);
        printMatrix(matrix);

        Sleep(400);
    }
    system("pause");
    std::cout<<"\033[2J";
}

void placeValues(std::array<std::array<std::string, columns>, rows> &matrix, cellInfo charges[]){
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            matrix[i][j] = space;
        }
    }

    for(int i = 0; i < cellCount; i++){
        int x = rand() % columns;
        int y = rand() % rows;

        int state = rand() % 2;

        if(matrix[y][x] == space){
            if(state == 1){
                charges[i] = cellInfo(true, x, y, 0.0f, 0.0f);
            }else{
                charges[i] = cellInfo(false, x, y, 0.0f, 0.0f);
            }
        }else{
            i--;
        }
    }
}

void printMatrix(std::array<std::array<std::string, columns>, rows>& matrix) {
    std::cout << "\033[H";

    for(int i = 0; i < columns + 2; i++){
        std::cout<<"-";
    }std::cout<<'\n';
    for (int i = 0; i < rows; i++) {
        std::cout<<'|';
        for (int j = 0; j < columns; j++) {
            std::cout << matrix[i][j];
            matrix[i][j] = space;
        }
        std::cout << '|' << '\n';
    }
    for(int i = 0; i < columns + 2; i++){
        std::cout<<"-";
    }std::cout<<'\n';
}


void placeCharges(std::array<std::array<std::string, columns>, rows> &matrix, cellInfo charges[]){
    for(int i = 0; i < cellCount; i++){
        int x = charges[i].xAxys;
        int y = charges[i].yAxys;

        while(matrix[y][x] != space){
            if(x < columns - 2 and x > 0){
                x += rand() & 2 == 0 ? 1:-1;
            }
            if(y < rows - 2 and y > 0){
                y += rand() & 2 == 0 ? 1:-1;
            }    
        }

        matrix[y][x] = charges[i].positive ? positive : negative;
    }
}

void countChargeChanges(cellInfo charges[]){
    for(int i = 0; i < cellCount; i++){
        for(int j = i+1; j < cellCount; j++){
            float forceBetween = 0;
            if(i != j){
                forceBetween += Force(charges, i, j);
            }

            double c = sqrt(abs(charges[i].xAxys - charges[j].xAxys) + abs(charges[i].yAxys - charges[j].yAxys));

            //forceBetween*=2;
            //double angle_x = acos(abs(forceBetween) / abs(charges[i].xAxys - charges[j].xAxys + 0.01));
            //double angle_y = acos(abs(forceBetween) / abs(charges[i].yAxys - charges[j].yAxys + 0.01));

            //double angle_x = 1;//atan(abs(charges[i].yAxys - charges[j].yAxys + 0.01) / abs(charges[i].xAxys - charges[j].xAxys + 0.01));
            //double angle_y = 1;//atan(abs(charges[i].xAxys - charges[j].xAxys + 0.01) / abs(charges[i].yAxys - charges[j].yAxys + 0.01));

            

            double angle_x = forceBetween > 0 ? abs(cos(abs(charges[i].xAxys - charges[j].xAxys+0.01) / c)) * log10(forceBetween) : abs(cos(abs(charges[i].xAxys - charges[j].xAxys+0.01) / c)) * log10(abs(forceBetween)); 
            double angle_y = forceBetween > 0 ? abs(sin(abs(charges[i].yAxys - charges[j].yAxys+0.01) / c)) * log10(forceBetween) : abs(sin(abs(charges[i].yAxys - charges[j].yAxys+0.01) / c)) * log10(abs(forceBetween));

            if(forceBetween > 0){
                //forceBetween = log10(abs(forceBetween));
                if(charges[i].xAxys > charges[j].xAxys){
                    charges[i].xVec += /*forceBetween * */angle_x;
                    charges[j].xVec -= /*forceBetween * */angle_x;
                }else{
                    charges[i].xVec -= /*forceBetween * */angle_x;
                    charges[j].xVec += /*forceBetween * */angle_x;
                }

                if(charges[i].yAxys > charges[j].yAxys){
                    charges[i].yVec += /*forceBetween * */angle_y;
                    charges[j].yVec -= /*forceBetween * */angle_y;;
                }else{
                    charges[i].yVec -= /*forceBetween * */angle_y;;
                    charges[j].yVec += /*forceBetween * */angle_y;;
                }
            }else{
                //forceBetween = log10(abs(forceBetween));

                if(charges[i].xAxys > charges[j].xAxys){
                    charges[i].xVec -= /*forceBetween * */angle_x;;
                    charges[j].xVec += /*forceBetween * */angle_x;;
                }
                else{

                    charges[i].xVec += /*forceBetween * */angle_x;;
                    charges[j].xVec -= /*forceBetween * */angle_x;;
                }

                if(charges[i].yAxys > charges[j].yAxys){

                    charges[i].yVec -= /*forceBetween * */angle_y;;
                    charges[j].yVec += /*forceBetween * */angle_y;;
                }
                else{
                    charges[i].yVec += /*forceBetween * */angle_y;;
                    charges[j].yVec -= /*forceBetween * */angle_y;;
                }
                    
            }
            //std::cout<<"Forces - "<<forceBetween<<". Angle x - "<<angle_x<<", angle y - "<<angle_y<<"\n";
        }

        charges[i].xAxys += round(charges[i].xVec);
        if (charges[i].xAxys < 0) {
            charges[i].xAxys = 0;
        } else if (charges[i].xAxys >= columns) {
            charges[i].xAxys = columns - 1;
        }

        charges[i].yAxys += round(charges[i].yVec);
        if (charges[i].yAxys < 0) {
            charges[i].yAxys = 0;
        } else if (charges[i].yAxys >= rows) {
            charges[i].yAxys = rows - 1;
        }


        charges[i].xVec = 0.0f;
        charges[i].yVec = 0.0f;
    }
}

float Force(cellInfo charges[], int i, int j){
    int q1 = charges[i].positive ? 1 : -1;
    int q2 = charges[j].positive ? 1 : -1;

    int x1 = charges[i].xAxys;
    int x2 = charges[j].xAxys;

    int y1 = charges[i].yAxys;
    int y2 = charges[j].yAxys;

    float distance = pow(abs(x1-x2), 2) + pow(abs(y1 - y2), 2);

    float force = (k * (q1 * singleCharge * q2 * singleCharge)) / distance;

    return force * pow(10, 28);
}

