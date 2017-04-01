#include "card.h"

Data Base[DECK_SIZE] = {{1,GWANG},{1,FIV_END},{1,NORM_PI},{1,NORM_PI}
					   ,{2,TEN_END},{2,FIV_END},{2,NORM_PI},{2,NORM_PI}
					   ,{3,GWANG},{3,FIV_END},{3,NORM_PI},{3,NORM_PI}
					   ,{4,TEN_END},{4,FIV_END},{4,NORM_PI},{4,NORM_PI}
					   ,{5,TEN_END},{5,FIV_END},{5,NORM_PI},{5,NORM_PI}
					   ,{6,TEN_END},{6,FIV_END},{6,NORM_PI},{6,NORM_PI}
					   ,{7,TEN_END},{7,FIV_END},{7,NORM_PI},{7,NORM_PI}
					   ,{8,GWANG},{8,TEN_END},{8,NORM_PI},{8,NORM_PI}
					   ,{9,SSANG_PI},{9,FIV_END},{9,NORM_PI},{9,NORM_PI}
					   ,{10,TEN_END},{10,FIV_END},{10,NORM_PI},{10,NORM_PI}
					   ,{11,GWANG},{11,NORM_PI},{11,NORM_PI},{11,SSANG_PI}
					   ,{12,GWANG},{12,TEN_END},{12,FIV_END},{12,SSANG_PI}
					   };

int create_card(Card **pcard, Data *pdata)
{
	if(((pcard) == NULL)||(pdata == NULL))	{return 0;}

	(*pcard) = (Card *)malloc(sizeof(Card));
	(*pcard)->data.month = pdata->month;
	(*pcard)->data.kind = pdata->kind;

	(*pcard)->next = NULL;

	return 1;
}

int init_card_list(Card_List *plist)
{
	if(plist == NULL)		{return 0;}

	plist->head.data.month = 0;
	plist->head.data.kind = 0;
	plist->num_card = 0;

	plist->head.next = NULL;

	return 1;
}

bool is_proper_pos(Card *pcomp, Card *pinsert)
{
	if((pcomp->data).month < (pinsert->data).month)	{return 0;}
	if((pcomp->data).month > (pinsert->data).month)	{return 1;}
	if((pcomp->data).kind < (pinsert->data).kind)	{return 0;}

	return 1;
}

int insert_card_list_deck(Card_List *plist, Card *pinsert)
{
	if((plist == NULL) || (pinsert == NULL))	{return 0;}

	Card *pcard = NULL;
	Card *prev = NULL;

	for(pcard=&(plist->head);pcard;)
	{
		prev = pcard;
		pcard = pcard->next;
	}

	pinsert->next = prev->next;
	prev->next = pinsert;
	plist->num_card++;

	return 1;
}
int insert_card_list(Card_List *plist, Card *pinsert)
{
	if((plist == NULL) || (pinsert == NULL))	{return 0;}

	Card *pcard = NULL;
	Card *prev = NULL;

	for(pcard=&(plist->head);pcard;)
	{
		if(is_proper_pos(pcard , pinsert))
			break;
		prev = pcard;
		pcard = pcard->next;
	}

	pinsert->next = prev->next;
	prev->next = pinsert;
	plist->num_card++;

	return 1;
}

int is_exist_equal/*_month*/(Card_List *plist, /*Card *pcomp*/Data *pcomp, Card_List *pfind, int opt) // opt == 0 >> compare month, opt == 1 >> compare kind, kind,opt == 3,4 >> pfind == NULL
{
	if((plist == NULL)||(pcomp==NULL))	{return 0;}
	if((opt>1) && (pcomp==NULL))		{return 0;}
	int cnt_equal = 0;

	Card *pcard = NULL;
	Card *pinsert = NULL;

	for(pcard=plist->head.next;pcard;pcard=pcard->next)
	{
		if(((opt==0)||(opt==3))&&(pcard->data.month == pcomp->month))
		{
			if(opt !=3 )
			{
				create_card(&pinsert, &(pcard->data));
				insert_card_list(pfind, pinsert);
			}
			cnt_equal++;
		}
		else if(((opt==1)||(opt==4))&&(pcard->data.kind == pcomp->kind))
		{
			if(opt!=4)
			{
				create_card(&pinsert, &(pcard->data));
				insert_card_list(pfind, pinsert);
			}
			cnt_equal++;
		}
	}

	return cnt_equal;
}

int find_card(Card_List *plist, int cnt, /*Card **pfind*/Data *pfind)
{
	if(plist->num_card < cnt)	{return 0;}
	if((plist==NULL)||(pfind==NULL))	{return 0;}
//	if(cnt < 1)		{return 0;}

	Card *pcard = plist->head.next;
	pfind->month = pcard->data.month;
	pfind->kind = pcard->data.kind;

	for(cnt;cnt>1;cnt--)
	{
		pcard = pcard->next;
		pfind->month = pcard->data.month;
		pfind->kind = pcard->data.kind;
	}
	return 1;
}

int delete_card(Card_List *plist, Data data/*, Card **pdel*/)
{
	if(plist==NULL)		{return 0;}
	if(plist->num_card==0)	{return 0;}

	Card *pcard = NULL;
	Card *prev = &(plist->head);

	for(pcard=plist->head.next;pcard;pcard=pcard->next)
	{
		if((pcard->data.month == data.month)
				&&(pcard->data.kind == data.kind))
		{
		//	(*pdel) = pcard;			find_card 수정 하면서 같이 바꿈
			prev->next = (pcard)->next; //find_card
			free(pcard);					//find_card
			//prev->next = (*pdel)->next;
			plist->num_card--;
			return 1;
		}

		prev = pcard;
	}

	return 0;
}

void init_deck_pre(Data *pdata)
{
	int i;

	for(i=0;i<USED_DESZ;i++)
	{
		pdata[i].month = Base[i].month;
		pdata[i].kind  = Base[i].kind;
	}
}

void swap_data(Data *pd1, Data *pd2)
{
	Data tmp;

	tmp.month = pd1->month;
	tmp.kind = pd1->kind;

	pd1->month = pd2->month;
	pd1->kind = pd2->kind;

	pd2->month = tmp.month;
	pd2->kind  = tmp.kind;
}

int shuffle_deck(Data *pdata)
{
	if(pdata==NULL)	{return 0;}

	int i, change;

	for(i=USED_DESZ;i>0;i--)
	{
		change=rand()%i;
		swap_data(pdata+i-1,pdata+change);
	}

	return 1;
}

int init_deck(Card_List *pdeck, Data *pdata)
{
	if(pdeck == NULL)	{return 0;}

	Card *pcard = NULL;
	int i;

	for(i=0;i<USED_DESZ;i++)
	{
		create_card(&pcard, pdata+i);
		insert_card_list_deck(pdeck, pcard);
	}

	return 1;
}

int move_another_list(Card_List *pstart, Card_List *pdestin, Data *pdata)
{
	if(pstart == NULL || pdestin == NULL || pdata == NULL)		{return 0;}
	if(pstart->num_card == 0)	{return 0;}

	Card *pcard = NULL;
	delete_card(pstart, (*pdata));

	create_card(&pcard, pdata);
	insert_card_list(pdestin, pcard);

	return 1;
}

int pile_to_other(Card_List *pile, Card_List *floor,Data *rvrs)
{
	if(pile==NULL || floor==NULL)	{return 0;}
	if(pile->num_card==0)			{return 0;}


	find_card(pile,1,rvrs);
	move_another_list(pile,floor,rvrs);

	return 1;
}

int hand_out_card(Card_List *hand, Card_List *gain, Card_List *floor,int cnt)
{
	if(hand==NULL||gain==NULL||floor==NULL)		{return 0;}
	if(hand->num_card==0||floor->num_card==0)	{return 0;}

	Card *pcard = NULL;
	Card_List tmp;
	Data find;
	int num_equal;

	init_card_list(&tmp);

	find_card(hand, cnt, &find);
	num_equal = is_exist_equal(floor, &find, &tmp, 0);

	if(num_equal == 0)		{return 0;}
	move_another_list(hand,gain,&find);

	if(num_equal>=2)
		scanf("%d", &cnt);
	else
		cnt = 1;

	find_card(&tmp,cnt,&find);
	move_another_list(&tmp,gain,&find);
	
	delete_card(floor, find);

	for(cnt=tmp.num_card;cnt>0;cnt--)
	{
		find_card(&tmp,1,&find);
		delete_card(&tmp,find);
	}

	return 1;
}

int give_PI_another_person(Card_List *give, Card_List *recv)
{
	if((give==NULL)||(recv==NULL))	{return 0;}
	if(give->num_card==0)	{return 0;}

	Data find = {0,NORM_PI};
	int num_equal = 0, cnt = 1;
	Card_List tmp;

	init_card_list(&tmp);

	num_equal = is_exist_equal(give,&find,&tmp,1);
	if(num_equal==0)
	{
		find.kind=SSANG_PI;
		num_equal=is_exist_equal(give,&find,&tmp,1);
	}

	if(num_equal==0)	return 0;
	if(num_equal>=2)	cnt=1;

	find_card(&tmp,cnt,&find);
	move_another_list(give,recv,&find);

	for(cnt=tmp.num_card;cnt>0;cnt--)
	{
		find_card(&tmp,1,&find);
		delete_card(&tmp,find);
	}

	return 1;
}

void print_card_list(Card_List *plist)
{
	Card *pcard = NULL;

	for(pcard=plist->head.next;pcard;pcard=pcard->next)
	{
		printf("%d ",pcard->data.month);
		switch(pcard->data.kind)
		{
			case 100:
				printf("광");
				break;
			case 101:
				printf("십");
				break;
			case 102:
				printf("오");
				break;
			case 103:
				printf("쌍피");
				break;
			case 104:
				printf("피");
				break;
			case 105:
				printf("폭탄");
				break;
		}
		if(pcard->next != NULL)
			printf(", ");
	}
	putchar('\n');
}
