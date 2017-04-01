#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#ifndef CARD_HEADER
#define CARD_HEADER

#define DECK_SIZE	55
#define USED_DESZ	48

#define GWANG		100
#define TEN_END		101
#define FIV_END		102
#define SSANG_PI	103
#define NORM_PI		104
#define BOMB		105


typedef struct tagData Data;

struct tagData
{
	int month;
	int kind;
};

typedef struct tagCard Card;

struct tagCard
{
	Data data;
	Card *next;
};

typedef struct tagCList Card_List;

struct tagCList
{
	Card head;
	int num_card;
};


int create_card(Card **pcard, Data *pdata);		//0->error, 1->ok
int init_card_list(Card_List *plist);				//0->error, 1->ok

bool is_proper_pos(Card *pcomp, Card *pinsert);				//0->error, 1->ok
int insert_card_list_deck(Card_List *plist, Card *pinsert);
int insert_card_list(Card_List *plist, Card *pinsert);		//0->error, 1->ok

int is_exist_equal(Card_List *plist, /*Card*/Data *pcomp, Card_List *pfind, int opt);
int find_card(Card_List *plist, int cnt, /*Card **pfind*/Data *pfind);
int delete_card(Card_List *plist, Data data/*, Card **pdel*/);

void init_deck_pre(Data *pdata);
void swap_data(Data *pd1, Data *pd2);
int shuffle_deck(Data *pdata);

int move_another_list(Card_List *pstart, Card_List *pdestin, Data *pdata);

int init_deck(Card_List *pdeck, Data *pdata);

int pile_to_other(Card_List *pile, Card_List *floor,Data *rvrs);

int hand_out_card(Card_List *hand, Card_List *gain, Card_List *floor,int cnt);

int give_PI_another_person(Card_List *give, Card_List *recv);

void print_card_list(Card_List *plist);
#endif
