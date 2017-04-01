#include <stdio.h>
#include <math.h>
#include "card.h"

#ifndef __SCORE_H__
#define __SCORE_H__

typedef struct tagScore
{
	int gw_num;
	int ten_num;
	int fiv_num;
	int pi_num;
	int pre_sc;
	int sc;
}Score;

typedef struct tagPerson
{
	int go;
	int yeok_go[3];
	int go_aft_yg;
	int gwang_bak;
	int meong_bak;
	int pi_bak;
	int ddi_bak;
	int go_bak;
	int eq_mon;
	int heun_deul;
	int chong_tong;
	int balance;
	Score score;
	Card_List hand;
	Card_List gain;
}Person;

typedef struct tagGame
{
	Person player[3];
	int first;
	int nagari;
	int go_player;
	int pre_go;
	int victor;
	int per_mon;
	int pos_tog;
	int guk_pi_ten;				//0>쌍피 1>열끗
	Card_List pile;
	Card_List floor;
}Game;

int init_game(Game *pgame,int turn,int guk_pi_ten);

int get_gw_num(Card_List *gain, Card_List *tmp);
int get_score_gw(Card_List *gain, Person *player);

int get_ten_num(Card_List *gain, Card_List *tmp);
int get_score_ten(Card_List *gain,Person *player);

int get_fiv_num(Card_List *gain, Card_List *tmp);
int get_score_fiv(Card_List *gain,Person *player);

int get_pi_num(Card_List *gain, Card_List *tmp);
int get_score_pi(Card_List *gain,Person *player);

int nagari(Game *game);

int gwang_bak(Person *player, int victor);
int meong_teong_gu_ri(Person *player, int victor);
int pi_bak(Person *player, int victor);
int ddi_bak(Person *player, int victor);

int init_yeok_go(Person *player, int turn);
int yeok_go(Game *game, int turn);
int go(Game *game, int turn);

int sseul(Person *player, int turn);

int is_pos_heun(Person *player, int month);
int is_pos_bomb(Card_List *floor, int month);
int bomb(Person *player, Card_List *floor, int turn);
int heun_deul_gi(Person *player,Card_List *pile,Card_List *floor,int inp,bool is_pos_b,int turn);

int take_card_equal(Person *player,Card_List *floor, int month, int turn);
int take_card_nequal(Person *player, Card_List *floor, Data find/*int month*/, int turn, int opt);
int take_card(Person *player, Card_List *pile, Card_List *floor, int cnt, int turn);

int get_score(Person *player);
int cal_multiple(Game *game);

int take_money(Game *game);

#endif
