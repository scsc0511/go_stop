#include "interface.h"

void print_scr(Game *game,int turn)
{
	for(int i=0;i<MAX_PLAYER;i++)
	{
		if(i == turn)
			putchar('*');
		else
			putchar(' ');
		printf("%c ",'A'+i);
		printf("	   :");
		printf("패: ");
		print_card_list(&(game->player[i].hand));
		printf("go: %d\n",game->player[i].go);
		printf(" 점수: %d ",game->player[i].score.sc);
		printf("   딴 화투 : ");
		print_card_list(&(game->player[i].gain));
		putchar('\n');
	}
	putchar('\n');
	printf(" 깔린 패) :");
	print_card_list(&(game->floor));

}

int reset_game(Game *game)
{
	if(game==NULL)		return 0;
	Data find ;

	for(Card *card=game->pile.head.next;card;card=card->next)	
	{
		find_card(&(game->pile),1,&find);
		delete_card(&(game->pile),find);
	}

	for(Card *card=game->floor.head.next;card!=NULL;)	
	{
		find_card(&(game->floor),1,&find);
		card = card->next;
		delete_card(&(game->floor),find);
	}

	for(Card *card=game->player[0].hand.head.next;card;)	
	{
		find_card(&(game->player[0].hand),1,&find);
		card = card->next;
		delete_card(&(game->player[0].hand),find);
	}

	for(Card *card=game->player[1].hand.head.next;card;)	
	{
		find_card(&(game->player[1].hand),1,&find);
		card = card->next;
		delete_card(&(game->player[1].hand),find);
	}

	for(Card *card=game->player[2].hand.head.next;card;)	
	{
		find_card(&(game->player[2].hand),1,&find);
		card = card->next;
		delete_card(&(game->player[2].hand),find);
	}

	for(Card *card=game->player[0].gain.head.next;card;)	
	{
		find_card(&(game->player[0].gain),1,&find);
		card = card->next;
		delete_card(&(game->player[0].gain),find);
	}

	for(Card *card=game->player[1].gain.head.next;card;)	
	{
		find_card(&(game->player[1].gain),1,&find);
		card = card->next;
		delete_card(&(game->player[1].gain),find);
	}

	for(Card *card=game->player[2].gain.head.next;card;)	
	{
		find_card(&(game->player[2].gain),1,&find);
		card = card->next;
		delete_card(&(game->player[2].gain),find);
	}

	return 0;
}

int save(Game *game)
{
	if(game==NULL)		return 0;

	FILE *fp = fopen("g_save.txt","w");
	Card *card = NULL;

	if(fp == NULL)		return 0;

	fprintf(fp,"%d\n",game->first);
	fprintf(fp,"%d\n",game->nagari);
	fprintf(fp,"%d\n",game->go_player);
	fprintf(fp,"%d\n",game->pre_go);
	fprintf(fp,"%d\n",game->victor);
	fprintf(fp,"%d\n",game->per_mon);
	fprintf(fp,"%d\n",game->pos_tog);
	fprintf(fp,"%d\n",game->guk_pi_ten);

	fflush(fp);
	for(card=game->pile.head.next;card;card=card->next)
		fprintf(fp,"%d %d\n",card->data.month,card->data.kind);

	fprintf(fp,"0 0\n");
	fflush(fp);
	for(card=game->floor.head.next;card;card=card->next)
		fprintf(fp,"%d %d\n",card->data.month,card->data.kind);

	fprintf(fp,"0 0\n");
	fflush(fp);
	for(card=game->player[0].hand.head.next;card;card=card->next)
		fprintf(fp,"%d %d\n",card->data.month,card->data.kind);

	fprintf(fp,"0 0\n");
	fflush(fp);
	for(card=game->player[0].gain.head.next;card;card=card->next)
		fprintf(fp,"%d %d\n",card->data.month,card->data.kind);

	fprintf(fp,"0 0\n");
	fflush(fp);
	for(card=game->player[1].hand.head.next;card;card=card->next)
		fprintf(fp,"%d %d\n",card->data.month,card->data.kind);

	fprintf(fp,"0 0\n");
	fflush(fp);
	for(card=game->player[1].gain.head.next;card;card=card->next)
		fprintf(fp,"%d %d\n",card->data.month,card->data.kind);

	fprintf(fp,"0 0\n");
	fflush(fp);
	for(card=game->player[2].hand.head.next;card;card=card->next)
		fprintf(fp,"%d %d\n",card->data.month,card->data.kind);

	fprintf(fp,"0 0\n");
	fflush(fp);
	for(card=game->player[2].gain.head.next;card;card=card->next)
		fprintf(fp,"%d %d\n",card->data.month,card->data.kind);

	fflush(fp);
	fclose(fp);
	return 1;
}

int load(Game *game)
{
	if(game == NULL)	return 0;
	FILE *fp = fopen("g_save.txt","r");
	Card *card = NULL;
	Data data;

	if(fp == NULL)		return 0;

	fscanf(fp,"%d",&(game->first));
	fscanf(fp,"%d",&(game->nagari));
	fscanf(fp,"%d",&(game->go_player));
	fscanf(fp,"%d",&(game->pre_go));
	fscanf(fp,"%d",&(game->victor));
	fscanf(fp,"%d",&(game->per_mon));
	fscanf(fp,"%d",&(game->pos_tog));
	fscanf(fp,"%d",&(game->guk_pi_ten));

	while(fscanf(fp,"%d %d",&(data.month),&(data.kind))!=0)
	{
		if(data.month==0 && data.kind == 0)		break;
		create_card(&card,&data);
		insert_card_list(&(game->pile),card);
	}

	while(fscanf(fp,"%d %d",&(data.month),&(data.kind))!=0)
	{
		if(data.month==0 && data.kind == 0)		break;
		create_card(&card,&data);
		insert_card_list(&(game->floor),card);
	}

	while(fscanf(fp,"%d %d",&(data.month),&(data.kind))!=0)
	{
		if(data.month==0 && data.kind == 0)		break;
		create_card(&card,&data);
		insert_card_list(&(game->player[0].hand),card);
	}

	while(fscanf(fp,"%d %d",&(data.month),&(data.kind))!=0)
	{
		if(data.month==0 && data.kind == 0)		break;
		create_card(&card,&data);
		insert_card_list(&(game->player[0].gain),card);
	}

	while(fscanf(fp,"%d %d",&(data.month),&(data.kind))!=0)
	{
		if(data.month==0 && data.kind == 0)		break;
		create_card(&card,&data);
		insert_card_list(&(game->player[1].hand),card);
	}

	while(fscanf(fp,"%d %d",&(data.month),&(data.kind))!=0)
	{
		if(data.month==0 && data.kind == 0)		break;
		create_card(&card,&data);
		insert_card_list(&(game->player[1].gain),card);
	}

	while(fscanf(fp,"%d %d",&(data.month),&(data.kind))!=0)
	{
		if(data.month==0 && data.kind == 0)		break;
		create_card(&card,&data);
		insert_card_list(&(game->player[2].hand),card);
	}

	while(fscanf(fp,"%d %d",&(data.month),&(data.kind))!=0)
	{
		if(data.month==0 && data.kind == 0)		break;
		create_card(&card,&data);
		insert_card_list(&(game->player[2].hand),card);
	}

	fclose(fp);

	return 1;
}


