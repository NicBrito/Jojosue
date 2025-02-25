//#include "ACore.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "Player.h"
#include "ServerRule.h"
#include "server.h"

//ALA CHAT:
//  é possivel falarmos por aqui também qualquer coisa
//  bora primeiro ver se compila
//  eu n sei se o problema é no codigo ou no makeclient, pq tipo aparentemente ta tudo certinho aqui
//  taca printf em tudo. Em algum lugar tem q pegar alguns
// o makefile consertei aparetemente, ele chegou a commpilar, mas ao rodar ele dava core dumped
// gcc teste.c -o dale -lm -lallegro -lallegro_image -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec

#define MSG_MAX_SIZE 350
#define BUFFER_SIZE (MSG_MAX_SIZE + 100)
#define LOGIN_MAX_SIZE 13
#define MAX_CLIENTS 2

const char GAME_START = 99;
enum Game_state{
    WAITING_CON,
    IN_GAME,
    ENDGAME
};
    ALLEGRO_EVENT_QUEUE *eventQueue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_TIMER *decrementTimer = NULL;

void rodarServer(){

    char mapMatrix[][44] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,
                            0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,
                            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,1,1,
                            0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,1,1,1,0,0,0,0,0,0,0,
                            0,0,1,1,1,1,1,1,1,0,1,1,1,1,0,0,1,1,1,1,1,1,0,1,1,1,0,0,0,1,1,1,1,0,1,1,1,0,0,0,0,0,0,0,
                            0,0,1,1,1,1,1,1,1,0,1,1,1,1,0,0,1,1,1,1,1,1,0,1,1,1,0,0,0,1,1,1,1,0,1,1,1,0,0,1,1,1,0,0,
                            0,0,1,1,0,1,1,0,1,0,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,1,1,1,0,0,
                            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,
                            1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,1,0,0,1,1,1,0,
                            1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,0,1,1,1,1,0,0,0,1,1,1,0,0,1,1,1,1,0,0,1,1,1,0,
                            1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,
                            1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,
                            1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,
                            1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,
                            1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,0,0,0,1,1,1,1,0,0,1,1,1,
                            1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,0,0,0,1,1,1,1,0,0,0,1,1,1,1,0,0,1,1,1,
                            1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,1,1,1,0,1,1,1,0,0,0,1,1,1,1,0,0,0,1,1,1,1,0,0,0,0,0,
                            1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,1,1,1,
                            1,1,1,1,0,0,0,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,
                            1,1,1,1,0,0,0,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
                            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
                            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,0,0,0,1,1,1,0,0,1,1,1,1,1,
                            1,1,1,1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,0,0,0,1,1,1,0,0,1,1,1,1,1,
                            1,1,1,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,0,1,1,1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,
                            1,1,1,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,
                            1,1,1,1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0,1,1,0,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,1,1,1,1,
                            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,
                            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                            1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
                            
    char str_buffer[BUFFER_SIZE], aux_buffer[BUFFER_SIZE];
    Player_Data players[2];
    char nicknames[2][NICKNAME_MAX_SIZE];
    char skins[2];
    char serverState = WAITING_CON;

    char localdeentrega1[][2] = {{9, 2}, {14, 2}, {20, 2}, {24, 2}, {29, 0}, {35, 0}, {40, 3}, {4, 6}, {7, 6}, {11, 7}, {18, 8}, {24,6}, {30,7}, {35,6}, {40,7}, {4,14}, {10,14}, {13,14}, {29,11}, {35,12}, {41,11}, {7,17}, {1,21}, {8,21}, {12,21}, {24,21}, {29,18}, {35,19}, {41,20}, {1,27}, {5,27}, {10,27}, {16,26}, {21,26}, {28,25}, {34,24}, {33,28}};
    char localdeentrega2[][2] = {{9, 2}, {14, 2}, {20, 2}, {24, 2}, {29, 0}, {35, 0}, {41, 3}, {4, 6}, {7, 6}, {12, 7}, {19, 8}, {24,6}, {30,7}, {35,6}, {40,7}, {4,14}, {10,14}, {13,14}, {29,11}, {36,12}, {41,11}, {7,17}, {2,21}, {8,21}, {13,21}, {24,21}, {29,18}, {35,19}, {41,20}, {1,27}, {5,27}, {10,27}, {16,26}, {21,26}, {28,25}, {35,24}, {33,28}};

    puts("JOJOSUE SERVER IS UP, MAAAN\n");
    
    //printf("sizeof(Player_Data)=%d\n",sizeof(Player_Data));
    //char client_names[MAX_CHAT_CLIENTS][LOGIN_MAX_SIZE];
    
    
    if(!al_init())
       puts("Falha ao iniciar allegro\n");

    timer = al_create_timer(8.0); //bota sempre .0 nos numeros inteiros, pra ele interpretar como double
    if(!timer) {puts("timer == NULL");  exit(-1); }
    else puts("Timer funfou");
    decrementTimer = al_create_timer(1.0);
    if(!decrementTimer) {puts("decrementTimer == NULL"); exit(-1); }
    else puts("decrementTimer funfou");
    eventQueue = al_create_event_queue();

    //char mapMatrix[30][44];
    //readMap(mapMatrix);
    printf("OLA PROGRAMADOR\n");
    char players_connected=0;
    while (serverState != ENDGAME) {
        while(serverState == WAITING_CON){
            int id = acceptConnection();
            if (id != NO_CONNECTION) {
                if(players_connected++ < 2){
                    // recvMsgFromClient(players[id].nome, id, WAIT_FOR_IT);
                    // recvMsgFromClient(&players[id].skin, id, WAIT_FOR_IT);
                    recvMsgFromClient(nicknames[id],id,WAIT_FOR_IT);
                    recvMsgFromClient(&skins[id],id,WAIT_FOR_IT);
                    printf("%s connected id = %d and skin %d players_connected=%d\n", nicknames[id],id,skins[id],players_connected);
                    players[id].identifier = NO_CHANGE;
                    players[id].ID = id;
                    players[id].HP = 10;
                    players[id].reputation = 100;
                    players[id].face = DOWN;
                    players[id].money = 75;
                    players[id].posX = (id==0 ? 1:42);
                    players[id].posY = (id==0 ? 1:27);
                    int i;
                    for(i=0;i<3;i++) players[id].itemArray[i] = NO_ITEM;
                    for(i=0;i<5;i++) {
                        players[id].boxArray[i].type = NO_BOX;
                        players[id].boxArray[i].timeLast = 0;
                        players[id].boxArray[i].addIndex = 38;
                    }
                    players[id].holdingBoxes[0] = players[id].holdingBoxes[1] = -1;

                    sendMsgToClient((Player_Data *)&players[id],sizeof(Player_Data),id);
                }
            }
            struct msg_ret_t msg_ret = recvMsg(aux_buffer);
            if(msg_ret.status == DISCONNECT_MSG){
                --players_connected;
                printf("%s disconnected id: %d is free\n",nicknames[msg_ret.client_id], msg_ret.client_id);
                printf("players_connected: %d\n",players_connected);
            }

            if(players_connected==2){
                int i;
                Enemy_Data enemy;
                for(i=0;i<2;i++){
                    enemy.identifier = NO_CHANGE;
                    enemy.posX = players[i].posX;
                    enemy.posY = players[i].posY;
                    enemy.face = players[i].face;
                    enemy.skin = skins[i];
                    enemy.HP = 10;
                    strcpy(enemy.nome,nicknames[i]);
                    sendMsgToClient((Enemy_Data *)&enemy,sizeof(Enemy_Data),(i==0?1:0));
                }
                serverState = IN_GAME;
                broadcast((char *)&GAME_START,1);
            }
        }
        char typeOfChange;
        printf("Estado IN_GAME!\n");

        al_register_event_source(eventQueue, al_get_timer_event_source(timer));
        al_register_event_source(eventQueue, al_get_timer_event_source(decrementTimer));

        al_start_timer(timer);
        al_start_timer(decrementTimer);

        

        while(serverState == IN_GAME){

            struct msg_ret_t ret = recvMsg(&typeOfChange);

            if(ret.status==MESSAGE_OK){
                mapMatrix[players[0].posY][players[0].posX] = 0;
                mapMatrix[players[1].posY][players[1].posX] = 0;
                //printf("%d\n",typeOfChange);
                if(typeOfChange == UP_ARROW){
                    players[ret.client_id].identifier = POSITION;
                    players[ret.client_id].face = UP;

                    if(players[ret.client_id].posY-1>=0 && 
                    !(players[ret.client_id].posY-1==players[!ret.client_id].posY && players[ret.client_id].posX==players[!ret.client_id].posX) &&
                    mapMatrix[players[ret.client_id].posY-1][players[ret.client_id].posX]!=1){
                        players[ret.client_id].posY--;
                        if(mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX] == 'X') players[ret.client_id].HP -= 2;
                        else if(mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX] == 'Y') players[ret.client_id].HP -= 3;
                        else if(mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX] == 'Z') players[ret.client_id].HP -= 5;
                        
                    }

                    broadcast((Player_Data *)&players[ret.client_id],sizeof(Player_Data));
                }

                else if(typeOfChange == DOWN_ARROW){
                    players[ret.client_id].identifier = POSITION;
                    players[ret.client_id].face = DOWN;

                    if(players[ret.client_id].posY+1<30 && 
                    !(players[ret.client_id].posY+1==players[!ret.client_id].posY && players[ret.client_id].posX==players[!ret.client_id].posX) &&
                    mapMatrix[players[ret.client_id].posY+1][players[ret.client_id].posX]!=1){
                        players[ret.client_id].posY++;
                        if(mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX] == 'X') players[ret.client_id].HP -= 2;
                        else if(mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX] == 'Y') players[ret.client_id].HP -= 3;
                        else if(mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX] == 'Z') players[ret.client_id].HP -= 5;
                        
                    }
                    broadcast((Player_Data *)&players[ret.client_id],sizeof(Player_Data));
                }

                else if(typeOfChange == LEFT_ARROW){
                    players[ret.client_id].identifier = POSITION;
                    players[ret.client_id].face = LEFT;

                    if(players[ret.client_id].posX-1>=0 && 
                    !(players[ret.client_id].posX-1==players[!ret.client_id].posX && players[ret.client_id].posY==players[!ret.client_id].posY) &&
                    mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX-1]!=1){
                        players[ret.client_id].posX--;
                        if(mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX] == 'X') players[ret.client_id].HP -= 2;
                        else if(mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX] == 'Y') players[ret.client_id].HP -= 3;
                        else if(mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX] == 'Z') players[ret.client_id].HP -= 5;
                        
                    }
                    broadcast((Player_Data *)&players[ret.client_id],sizeof(Player_Data));
                }

                else if(typeOfChange == RIGHT_ARROW){
                    players[ret.client_id].identifier = POSITION;
                    players[ret.client_id].face = RIGHT;
                                                    
                    if(players[ret.client_id].posX+1<44 && 
                    !(players[ret.client_id].posX+1==players[!ret.client_id].posX && players[ret.client_id].posY==players[!ret.client_id].posY) &&
                    mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX+1]!=1){
                        players[ret.client_id].posX++;
                        if(mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX] == 'X') players[ret.client_id].HP -= 2;
                        else if(mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX] == 'Y') players[ret.client_id].HP -= 3;
                        else if(mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX] == 'Z') players[ret.client_id].HP -= 5;
                        
                    }
                    broadcast((Player_Data *)&players[ret.client_id],sizeof(Player_Data));
                }

                else if(typeOfChange == ACTION_MESSAGE){
                    //printf("ENTROU!!!\n");
                    //printf("x = %d ---- y = %d\n");
                    if((players[ret.client_id].posX==21&&players[ret.client_id].posY==11)||(players[ret.client_id].posX==21&&players[ret.client_id].posY==16)){

                        if(players[ret.client_id].HP<10 && players[ret.client_id].money >= 100){
                            players[ret.client_id].HP += 3;
                            players[ret.client_id].money -= 100;
                            if(players[ret.client_id].HP>10) players[ret.client_id].HP = 10;
                        }

                    }

                    else{
                        for(int i = 0;i<5;i++){
                            if((players[ret.client_id].posX==localdeentrega1[players[ret.client_id].boxArray[i].addIndex][0]&&players[ret.client_id].posY==localdeentrega1[players[ret.client_id].boxArray[i].addIndex][1])||(players[ret.client_id].posX==localdeentrega2[players[ret.client_id].boxArray[i].addIndex][0]&&players[ret.client_id].posY==localdeentrega2[players[ret.client_id].boxArray[i].addIndex][1])){
                                if(players[ret.client_id].boxArray[i].type == PAC){
                                    players[ret.client_id].money += 80;
                                    players[ret.client_id].boxArray[i].type = NO_BOX;
                                    players[ret.client_id].boxArray[i].timeLast = 0;
                                    players[ret.client_id].boxArray[i].addIndex = 38;
                                     if(players[ret.client_id].reputation+3<=100)players[ret.client_id].reputation += 3;
                                }
                                else if(players[ret.client_id].boxArray[i].type == SEDEX){
                                    players[ret.client_id].money += 130;
                                    players[ret.client_id].boxArray[i].type = NO_BOX;
                                    players[ret.client_id].boxArray[i].timeLast = 0;
                                    players[ret.client_id].boxArray[i].addIndex = 38;
                                     if(players[ret.client_id].reputation+5<=100)players[ret.client_id].reputation += 5;
                                }
                                else if(players[ret.client_id].boxArray[i].type == EXPRESS){
                                    players[ret.client_id].money += 200;
                                    players[ret.client_id].boxArray[i].type = NO_BOX;
                                    players[ret.client_id].boxArray[i].timeLast = 0;
                                    players[ret.client_id].boxArray[i].addIndex = 38;
                                    if(players[ret.client_id].reputation+7<=100)players[ret.client_id].reputation += 7;
                                }
                            }
                        }

                    }

                    broadcast((Player_Data *)&players[ret.client_id],sizeof(Player_Data));

                }

                else if(typeOfChange == ITEM1_BUTTON){

                    if(players[ret.client_id].itemArray[0] != NO_ITEM){    

                        char typeOfItem;
                        char flagsucesso=0;

                        if(players[ret.client_id].itemArray[0]==SHURICARD){

                            if(players[ret.client_id].face == UP){
                                for(int i = players[ret.client_id].posY; mapMatrix[i][players[ret.client_id].posX]!=1 && (players[ret.client_id].posY - i < 7);i--){
                                    if(players[!(ret.client_id)].posY == i && players[!(ret.client_id)].posX == players[ret.client_id].posX){
                                            players[!(ret.client_id)].HP -= 1;
                                            players[!(ret.client_id)].identifier = DAMAGE;
                                            break;
                                            }
                                }
                            }
                            else if(players[ret.client_id].face == DOWN){
                                for(int i = players[ret.client_id].posY; mapMatrix[i][players[ret.client_id].posX]!=1 && (i - players[ret.client_id].posY < 7);i++){
                                    if(players[!(ret.client_id)].posY == i && players[!(ret.client_id)].posX == players[ret.client_id].posX){
                                            players[!(ret.client_id)].HP -= 1;
                                            players[!(ret.client_id)].identifier = DAMAGE;
                                            break;
                                            }
                                }
                            }
                            else if(players[ret.client_id].face == RIGHT){
                                for(int i = players[ret.client_id].posX; mapMatrix[players[ret.client_id].posY][i]!=1 && (i - players[ret.client_id].posX < 7);i++){
                                    if(players[!(ret.client_id)].posY == players[ret.client_id].posY && players[!(ret.client_id)].posX == i){
                                            players[!(ret.client_id)].HP -= 1;
                                            players[!(ret.client_id)].identifier = DAMAGE;
                                            break;
                                            }
                                }
                            }
                            else{
                                for(int i = players[ret.client_id].posX; mapMatrix[players[ret.client_id].posY][i]!=1 && (players[ret.client_id].posX - i < 7);i--){
                                    if(players[!(ret.client_id)].posY == players[ret.client_id].posY && players[!(ret.client_id)].posX == i){
                                            players[!(ret.client_id)].HP -= 1;
                                            players[!(ret.client_id)].identifier = DAMAGE;
                                            break;
                                            }
                                }
                            }

                            flagsucesso = 1;

                        }

                        else{ 
                            if(players[ret.client_id].itemArray[0]==TRAP) typeOfItem = 'X';
                            else if(players[ret.client_id].itemArray[0]==BOMB) typeOfItem = 'Y';
                            else typeOfItem = 'Z';

                            if(players[ret.client_id].face == UP){
                                if((mapMatrix[players[ret.client_id].posY-1][players[ret.client_id].posX] != 1) && (players[ret.client_id].posY>0) 
                                && !((players[ret.client_id].posX==players[!ret.client_id].posX)&&(players[ret.client_id].posY-1==players[!ret.client_id].posY))
                                )
                                {
                                    mapMatrix[players[ret.client_id].posY-1][players[ret.client_id].posX] = typeOfItem;

                                    flagsucesso=1;
                                }
                            }

                            else if(players[ret.client_id].face == DOWN){
                                if((mapMatrix[players[ret.client_id].posY+1][players[ret.client_id].posX] != 1) && (players[ret.client_id].posY<30) 
                                && !((players[ret.client_id].posX==players[!ret.client_id].posX)&&(players[ret.client_id].posY+1==players[!ret.client_id].posY))
                                )
                                {
                                    mapMatrix[players[ret.client_id].posY+1][players[ret.client_id].posX] = typeOfItem;

                                    flagsucesso=1;
                                }
                            }

                            else if(players[ret.client_id].face == LEFT){
                                if((mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX-1] != 1) && (players[ret.client_id].posX>0)
                                && !((players[ret.client_id].posX-1==players[!ret.client_id].posX)&&(players[ret.client_id].posY==players[!ret.client_id].posY))
                                )
                                {
                                    mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX-1] = typeOfItem;

                                    flagsucesso=1;
                                }
                            }

                            else{
                                if((mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX+1] != 1) && (players[ret.client_id].posX<43) 
                                && !((players[ret.client_id].posX+1==players[!ret.client_id].posX)&&(players[ret.client_id].posY==players[!ret.client_id].posY))
                                )
                                {
                                    mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX+1] = typeOfItem;

                                    flagsucesso=1;
                                }
                            }
                        }

                        if(flagsucesso) {
                            players[ret.client_id].itemArray[0] = NO_ITEM;
                            players[ret.client_id].identifier = ITEM_USAGE; 
                        }
                        else players[ret.client_id].identifier = NO_CHANGE; 
                        for(int i = 0; i<2; i++)
                        broadcast((Player_Data *)&players[i],sizeof(Player_Data));
                    }

                }

                else if(typeOfChange == ITEM2_BUTTON){

                    if(players[ret.client_id].itemArray[1] != NO_ITEM){    

                        char typeOfItem;
                        char flagsucesso=0;

                        if(players[ret.client_id].itemArray[1]==SHURICARD){

                            if(players[ret.client_id].face == UP){
                                for(int i = players[ret.client_id].posY; mapMatrix[i][players[ret.client_id].posX]!=1 && (players[ret.client_id].posY - i < 7);i--){
                                    if(players[!(ret.client_id)].posY == i && players[!(ret.client_id)].posX == players[ret.client_id].posX){
                                        players[!(ret.client_id)].HP -= 1;
                                        players[!(ret.client_id)].identifier = DAMAGE;
                                        break;
                                        }
                                }
                            }
                            else if(players[ret.client_id].face == DOWN){
                                for(int i = players[ret.client_id].posY; mapMatrix[i][players[ret.client_id].posX]!=1 && (i - players[ret.client_id].posY < 7);i++){
                                    if(players[!(ret.client_id)].posY == i && players[!(ret.client_id)].posX == players[ret.client_id].posX){
                                        players[!(ret.client_id)].HP -= 1;
                                        players[!(ret.client_id)].identifier = DAMAGE;
                                        break;
                                        }
                                }
                            }
                            else if(players[ret.client_id].face == RIGHT){
                                for(int i = players[ret.client_id].posX; mapMatrix[players[ret.client_id].posY][i]!=1 && (i - players[ret.client_id].posX < 7);i++){
                                    if(players[!(ret.client_id)].posY == players[ret.client_id].posY && players[!(ret.client_id)].posX == i){
                                        players[!(ret.client_id)].HP -= 1;
                                        players[!(ret.client_id)].identifier = DAMAGE;
                                        break;
                                        }
                                }
                            }
                            else{
                                for(int i = players[ret.client_id].posX; mapMatrix[players[ret.client_id].posY][i]!=1 && (players[ret.client_id].posX - i < 7);i--){
                                    if(players[!(ret.client_id)].posY == players[ret.client_id].posY && players[!(ret.client_id)].posX == i){
                                        players[!(ret.client_id)].HP -= 1;
                                        players[!(ret.client_id)].identifier = DAMAGE;
                                        break;
                                        }
                                }
                            }  

                            flagsucesso = 1;
                        }
                        
                        else {
                            if(players[ret.client_id].itemArray[1]==TRAP) typeOfItem = 'X';
                            else if(players[ret.client_id].itemArray[1]==BOMB) typeOfItem = 'Y';
                            else typeOfItem = 'Z';

                            if(players[ret.client_id].face == UP){
                                if((mapMatrix[players[ret.client_id].posY-1][players[ret.client_id].posX] != 1) && (players[ret.client_id].posY>0) 
                                && !((players[ret.client_id].posX==players[!ret.client_id].posX)&&(players[ret.client_id].posY-1==players[!ret.client_id].posY))
                                )
                                {
                                    mapMatrix[players[ret.client_id].posY-1][players[ret.client_id].posX] = typeOfItem;

                                    flagsucesso=1;
                                }
                            }

                            else if(players[ret.client_id].face == DOWN){
                                if((mapMatrix[players[ret.client_id].posY+1][players[ret.client_id].posX] != 1) && (players[ret.client_id].posY<30) 
                                && !((players[ret.client_id].posX==players[!ret.client_id].posX)&&(players[ret.client_id].posY+1==players[!ret.client_id].posY))
                                )
                                {
                                    mapMatrix[players[ret.client_id].posY+1][players[ret.client_id].posX] = typeOfItem;

                                    flagsucesso=1;
                                }
                            }

                            else if(players[ret.client_id].face == LEFT){
                                if((mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX-1] != 1) && (players[ret.client_id].posX>0)
                                && !((players[ret.client_id].posX-1==players[!ret.client_id].posX)&&(players[ret.client_id].posY==players[!ret.client_id].posY))
                                )
                                {
                                    mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX-1] = typeOfItem;

                                    flagsucesso=1;
                                }
                            }

                            else{
                                if((mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX+1] != 1) && (players[ret.client_id].posX<43) 
                                && !((players[ret.client_id].posX+1==players[!ret.client_id].posX)&&(players[ret.client_id].posY==players[!ret.client_id].posY))
                                )
                                {
                                    mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX+1] = typeOfItem;

                                    flagsucesso=1;
                                }
                            }
                        }

                        if(flagsucesso) {
                            players[ret.client_id].itemArray[1] = NO_ITEM;
                            players[ret.client_id].identifier = ITEM_USAGE; 
                        }
                        for(int i = 0; i<2; i++)
                        broadcast((Player_Data *)&players[i],sizeof(Player_Data));
                    }

                }

                else if(typeOfChange == ITEM3_BUTTON){

                    if(players[ret.client_id].itemArray[2] != NO_ITEM){    

                        char typeOfItem;
                        char flagsucesso = 0;

                        if(players[ret.client_id].itemArray[2]==SHURICARD){

                            if(players[ret.client_id].face == UP){
                                for(int i = players[ret.client_id].posY; mapMatrix[i][players[ret.client_id].posX]!=1 && (players[ret.client_id].posY - i < 7);i--){
                                    if(players[!(ret.client_id)].posY == i && players[!(ret.client_id)].posX == players[ret.client_id].posX){
                                            players[!(ret.client_id)].HP -= 1;
                                            players[!(ret.client_id)].identifier = DAMAGE;
                                            break;
                                            }
                                }
                            }
                            else if(players[ret.client_id].face == DOWN){
                                for(int i = players[ret.client_id].posY; mapMatrix[i][players[ret.client_id].posX]!=1 && (i - players[ret.client_id].posY < 7);i++){
                                    if(players[!(ret.client_id)].posY == i && players[!(ret.client_id)].posX == players[ret.client_id].posX){
                                            players[!(ret.client_id)].HP -= 1;
                                            players[!(ret.client_id)].identifier = DAMAGE;
                                            break;
                                            }
                                }
                            }
                            else if(players[ret.client_id].face == RIGHT){
                                for(int i = players[ret.client_id].posX; mapMatrix[players[ret.client_id].posY][i]!=1 && (i - players[ret.client_id].posX < 7);i++){
                                    if(players[!(ret.client_id)].posY == players[ret.client_id].posY && players[!(ret.client_id)].posX == i){
                                            players[!(ret.client_id)].HP -= 1;
                                            players[!(ret.client_id)].identifier = DAMAGE;
                                            break;
                                            }
                                }
                            }
                            else{
                                for(int i = players[ret.client_id].posX; mapMatrix[players[ret.client_id].posY][i]!=1 && (players[ret.client_id].posX - i < 7);i--){
                                    if(players[!(ret.client_id)].posY == players[ret.client_id].posY && players[!(ret.client_id)].posX == i){
                                            players[!(ret.client_id)].HP -= 1;
                                            players[!(ret.client_id)].identifier = DAMAGE;
                                            break;
                                            }
                                }
                            }

                            flagsucesso = 1;
                        }

                        else{
                            if(players[ret.client_id].itemArray[2]==TRAP) typeOfItem = 'X';
                            else if(players[ret.client_id].itemArray[2]==BOMB) typeOfItem = 'Y';
                            else typeOfItem = 'Z';

                            if(players[ret.client_id].face == UP){
                                if((mapMatrix[players[ret.client_id].posY-1][players[ret.client_id].posX] != 1) && (players[ret.client_id].posY>0) 
                                && !((players[ret.client_id].posX==players[!ret.client_id].posX)&&(players[ret.client_id].posY-1==players[!ret.client_id].posY))
                                )
                                {
                                    mapMatrix[players[ret.client_id].posY-1][players[ret.client_id].posX] = typeOfItem;

                                    flagsucesso=1;
                                }
                            }

                            else if(players[ret.client_id].face == DOWN){
                                if((mapMatrix[players[ret.client_id].posY+1][players[ret.client_id].posX] != 1) && (players[ret.client_id].posY<30) 
                                && !((players[ret.client_id].posX==players[!ret.client_id].posX)&&(players[ret.client_id].posY+1==players[!ret.client_id].posY))
                                )
                                {
                                    mapMatrix[players[ret.client_id].posY+1][players[ret.client_id].posX] = typeOfItem;

                                    flagsucesso=1;
                                }
                            }

                            else if(players[ret.client_id].face == LEFT){
                                if((mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX-1] != 1) && (players[ret.client_id].posX>0)
                                && !((players[ret.client_id].posX-1==players[!ret.client_id].posX)&&(players[ret.client_id].posY==players[!ret.client_id].posY))
                                )
                                {
                                    mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX-1] = typeOfItem;

                                    flagsucesso=1;
                                }
                            }

                            else{
                                if((mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX+1] != 1) && (players[ret.client_id].posX<43) 
                                && !((players[ret.client_id].posX+1==players[!ret.client_id].posX)&&(players[ret.client_id].posY==players[!ret.client_id].posY))
                                )
                                {
                                    mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX+1] = typeOfItem;

                                    flagsucesso=1;
                                }
                            }
                        }

                        if(flagsucesso) {
                            players[ret.client_id].itemArray[2] = NO_ITEM;
                            players[ret.client_id].identifier = ITEM_USAGE; 
                        }
                        for(int i = 0; i<2; i++)
                        broadcast((Player_Data *)&players[i],sizeof(Player_Data));
                    }

                }

                else if(typeOfChange == BUY1){
                    printf("COMPROU ITEM 1\n");
                    if(ret.client_id==0){
                        if(players[ret.client_id].posX==1&&players[ret.client_id].posY==1){if(players[ret.client_id].money >= 50){
                       // printf("%d\n",players[ret.client_id].money);
                        for(int i=0;i<3;i++){
                            printf("%d\n",players[ret.client_id].itemArray[i]);
                            if(players[ret.client_id].itemArray[i] == NO_ITEM){
                                players[ret.client_id].itemArray[i] = SHURICARD;
                                players[ret.client_id].money -= 50;
                                break;
                            }
                        }
                    }
                    players[ret.client_id].identifier= BUY;
                    }
                    }
                    else{
                      if(players[ret.client_id].posX==42&&players[ret.client_id].posY==27){if(players[ret.client_id].money >= 50){
                       // printf("%d\n",players[ret.client_id].money);
                        for(int i=0;i<3;i++){
                            printf("%d\n",players[ret.client_id].itemArray[i]);
                            if(players[ret.client_id].itemArray[i] == NO_ITEM){
                                players[ret.client_id].itemArray[i] = SHURICARD;
                                players[ret.client_id].money -= 50;
                                break;
                            }
                        }
                    }
                    players[ret.client_id].identifier= BUY;
                    }  
                    }
                    broadcast((Player_Data *)&players[ret.client_id],sizeof(Player_Data));
                }

                else if(typeOfChange == BUY2){
                    if(ret.client_id==0){
                        if(players[ret.client_id].posX==1&&players[ret.client_id].posY==1){if(players[ret.client_id].money >= 75){
                       // printf("%d\n",players[ret.client_id].money);
                        for(int i=0;i<3;i++){
                            printf("%d\n",players[ret.client_id].itemArray[i]);
                            if(players[ret.client_id].itemArray[i] == NO_ITEM){
                                players[ret.client_id].itemArray[i] = TRAP;
                                players[ret.client_id].money -= 75;
                                break;
                            }
                        }
                    }
                    players[ret.client_id].identifier= BUY;
                    }
                    }
                    else{
                      if(players[ret.client_id].posX==42&&players[ret.client_id].posY==27){if(players[ret.client_id].money >= 75){
                       // printf("%d\n",players[ret.client_id].money);
                        for(int i=0;i<3;i++){
                            printf("%d\n",players[ret.client_id].itemArray[i]);
                            if(players[ret.client_id].itemArray[i] == NO_ITEM){
                                players[ret.client_id].itemArray[i] = TRAP;
                                players[ret.client_id].money -= 75;
                                break;
                            }
                        }
                    }
                    players[ret.client_id].identifier= BUY;
                    }  
                    }
                    broadcast((Player_Data *)&players[ret.client_id],sizeof(Player_Data));
                }

                else if(typeOfChange == BUY3){
                    if(ret.client_id==0){
                        if(players[ret.client_id].posX==1&&players[ret.client_id].posY==1){if(players[ret.client_id].money >= 100){
                       // printf("%d\n",players[ret.client_id].money);
                        for(int i=0;i<3;i++){
                            printf("%d\n",players[ret.client_id].itemArray[i]);
                            if(players[ret.client_id].itemArray[i] == NO_ITEM){
                                players[ret.client_id].itemArray[i] = BOMB;
                                players[ret.client_id].money -= 100;
                                break;
                            }
                        }
                    }
                    players[ret.client_id].identifier= BUY;
                    }
                    }
                    else{
                      if(players[ret.client_id].posX==42&&players[ret.client_id].posY==27){if(players[ret.client_id].money >= 100){
                       // printf("%d\n",players[ret.client_id].money);
                        for(int i=0;i<3;i++){
                            printf("%d\n",players[ret.client_id].itemArray[i]);
                            if(players[ret.client_id].itemArray[i] == NO_ITEM){
                                players[ret.client_id].itemArray[i] = BOMB;
                                players[ret.client_id].money -= 100;
                                break;
                            }
                        }
                    }
                    players[ret.client_id].identifier= BUY;
                    }  
                    }
                    broadcast((Player_Data *)&players[ret.client_id],sizeof(Player_Data));
                }

                else if(typeOfChange == BUY4){
                    if(ret.client_id==0){
                        if(players[ret.client_id].posX==1&&players[ret.client_id].posY==1){if(players[ret.client_id].money >= 150){
                       // printf("%d\n",players[ret.client_id].money);
                        for(int i=0;i<3;i++){
                            printf("%d\n",players[ret.client_id].itemArray[i]);
                            if(players[ret.client_id].itemArray[i] == NO_ITEM){
                                players[ret.client_id].itemArray[i] = DOG;
                                players[ret.client_id].money -= 150;
                                break;
                            }
                        }
                    }
                    players[ret.client_id].identifier= BUY;
                    }
                    }
                    else{
                      if(players[ret.client_id].posX==42&&players[ret.client_id].posY==27){if(players[ret.client_id].money >= 150){
                       // printf("%d\n",players[ret.client_id].money);
                        for(int i=0;i<3;i++){
                            printf("%d\n",players[ret.client_id].itemArray[i]);
                            if(players[ret.client_id].itemArray[i] == NO_ITEM){
                                players[ret.client_id].itemArray[i] = DOG;
                                players[ret.client_id].money -= 150;
                                break;
                            }
                        }
                    }
                    players[ret.client_id].identifier= BUY;
                    }  
                    }
                    broadcast((Player_Data *)&players[ret.client_id],sizeof(Player_Data));
                }

                else {
                    printf("Invalid message!\nret.status=%d\n%d\n",ret.status,typeOfChange);
                }
                //mapMatrix[players[0].posY][players[0].posX] = '+';
                //mapMatrix[players[1].posY][players[1].posX] = '*';
                printMap(mapMatrix);
                if(players[ret.client_id].HP <= 0 || players[ret.client_id].reputation <= 0){
                    players[ret.client_id].identifier = LOSE;
                    broadcast((Player_Data *)&players[ret.client_id],sizeof(Player_Data));
                }
            }
            else if(ret.status==DISCONNECT_MSG){
                --players_connected;
                printf("PLAYER(%d) DESCONECTOU\n",ret.client_id);
                if(players_connected == 0) serverState = ENDGAME;
            }
            
            while(!al_is_event_queue_empty(eventQueue)){
                //printf("ENTROU NA ALLEGRO!\n");
                ALLEGRO_EVENT event;
                al_wait_for_event(eventQueue, &event);

                if(event.type == ALLEGRO_EVENT_TIMER){
                    
                    if(event.timer.source == decrementTimer){
                        for(int j = 0;j<2;j++){
                            for(int i = 0;i<5;i++){
                                if(players[j].boxArray[i].type != NO_BOX && players[j].boxArray[i].timeLast >0) players[j].boxArray[i].timeLast -= 1;
                                
                                if(players[j].boxArray[i].type != NO_BOX && players[j].boxArray[i].timeLast == 0){
                                    players[j].boxArray[i].type = NO_BOX;
                                    players[j].reputation -= 10;
                                }
                            }
                        }
                    }

                    else if(event.timer.source == timer){
                        printf("ERA PRA ATT CAIXA!\n");
                        srand(event.timer.count*event.timer.count+event.timer.count);
                        int randomizer = rand()%4;
                        if(randomizer == 0) randomizer = 1;
                        switch(randomizer){

                            case 1: for(int i = 0; i<2; i++){
                                        for(int j = 0; j< 5; j++){
                                            if(players[i].boxArray[j].type == NO_BOX){
                                                players[i].boxArray[j].type = PAC;
                                                players[i].boxArray[j].timeLast = 80;
                                                players[i].boxArray[j].addIndex = rand()%37;
                                                break;
                                            }
                                        }
                                    }
                                    break; 

                            case 2: for(int i = 0; i<2; i++){
                                        for(int j = 0; j< 5; j++){
                                            if(players[i].boxArray[j].type == NO_BOX){
                                                players[i].boxArray[j].type = SEDEX;
                                                players[i].boxArray[j].timeLast = 60;
                                                players[i].boxArray[j].addIndex = rand()%37;
                                                break;
                                            }
                                        }
                                    }
                                    break; 
                            case 3: for(int i = 0; i<2; i++){
                                        for(int j = 0; j< 5; j++){
                                            if(players[i].boxArray[j].type == NO_BOX){
                                                players[i].boxArray[j].type = EXPRESS;
                                                players[i].boxArray[j].timeLast = 40;
                                                players[i].boxArray[j].addIndex = rand()%37;
                                                break;
                                            }
                                        }
                                    }
                                    break; 

                        }
                    }

                }

                for(int i = 0; i<2; i++){
                    players[i].identifier = BOX_CHANGE;
                    sendMsgToClient((Player_Data *)&players[i],sizeof(Player_Data),i);
                    //broadcast((Player_Data *)&players[i],sizeof(Player_Data));
                }

            }
            
            //11 e 21
            //serverState = ENDGAME;
        }
    }
    serverReset();
}

int main() {
    serverInit(MAX_CLIENTS);
    puts("Server is running!!\n");
    int res = 'y';
    while(1){
        rodarServer();
        
        // printf("Deseja reiniciar o server?[Y/n]\n");
		// while (res = tolower(getchar()), res != 'n' && res != 'y' && res != '\n'){
		// 	puts("anh???");
		// }
        // if(res == 'n') break;
        // else if(res == 'y') serverReset();
    }
    
    puts("MORRI");
    al_destroy_timer(timer);
    al_destroy_timer(decrementTimer);
    al_destroy_event_queue(eventQueue);
    return EXIT_SUCCESS;
}


