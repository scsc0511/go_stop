#include "card.h"
#include "score.h"

//score, person, game 구조체 초기화 함수 필요

int init_score(Score *psc)
{
	if(psc==NULL)	return 0;

	psc->gw_num = 0;
	psc->ten_num = 0;
	psc->fiv_num = 0;
	psc->pi_num = 0;
	psc->pre_sc = 0;
	psc->sc = 0;

	return 1;
}

int init_person(Person *player)
{
	if(player==NULL)	return 0;

	player->go = 0;
	player->go_aft_yg = 0;
	player->gwang_bak = 0;
	player->meong_bak = 0;
	player->chong_tong = 0;
	player->pi_bak = 0;
	player->ddi_bak = 0;
	player->go_bak = 0;
	player->eq_mon = 0;
	player->heun_deul = 0;
	player->balance = 100000;
	init_score(&(player->score));
	init_card_list(&(player->hand));
	init_card_list(&(player->gain));
	for(int i=0;i<3;i++)
		player->yeok_go[i] = 0;

	return 1;
}

int init_game(Game *pgame,int first,int guk_pi_ten)
{
	if(pgame==NULL)		return 0;

	Data data[USED_DESZ];
	
	init_deck_pre(data);
	shuffle_deck(data);

	for(int i=0;i<3;i++)
		init_person(pgame->player+i);
	pgame->nagari = 0;
	pgame->first = 0;
	pgame->guk_pi_ten = guk_pi_ten;
	pgame-> pos_tog = 1;
	pgame->pos_tog = 1;
	pgame->go_player = -1;
	pgame->pre_go = -1;
	pgame->victor = -1;
	pgame->per_mon = 100;

	init_card_list(&(pgame->pile));
	init_card_list(&(pgame->floor));
	init_deck(&(pgame->pile), data);

	return 1;
}

int get_gw_num(Card_List *gain, Card_List *tmp)
{
	if(gain==NULL)	return 0;
	if(gain->num_card==0)	return 0;

	Data find = {0,GWANG};
	int gw_num = is_exist_equal(gain,&find,tmp,1);

	return gw_num;
}

int get_score_gw(Card_List *gain, Person *player)
{
	if(gain==NULL)		return 0;
	if(gain->num_card==0)	return 0;

	int is_bi = 0, score = 0;
	Card_List tmp;
	Data bi_gwang = {12,GWANG};
	Data find;

	init_card_list(&tmp);
	player->score.gw_num = get_gw_num(gain,&tmp);
	

	if(player->score.gw_num == 3)
	{
		is_bi = is_exist_equal(&tmp,&bi_gwang,NULL,3);
		if(is_bi)
			score = 2;
		else
			score = 3;
	}
	else if(player->score.gw_num == 4)
		score = 4;
	else if(player->score.gw_num == 5)
		score = 15;

	for(int i=tmp.num_card;i>0;i--)
	{
		find_card(&tmp,1,&find);
		delete_card(&tmp,find);
	}

	return score;
}

int get_ten_num(Card_List *gain, Card_List *tmp)
{
	if(gain==NULL||tmp==NULL)		return 0;
	if(gain->num_card==0)			return 0;

	Data find = {0,TEN_END};
	int ten_num = is_exist_equal(gain,&find,tmp,1);

	return ten_num;		
}

int get_score_ten(Card_List *gain,Person *player)
{
	if(gain==NULL)		return 0;
	if(gain->num_card==0)	return 0;

	Card_List tmp;
	int score = 0, godori_cnt = 0;

	init_card_list(&tmp);
	player->score.ten_num = get_ten_num(gain, &tmp);
	Data godori[3] = {{2,TEN_END},{4,TEN_END},{8,TEN_END}};

	if(player->score.ten_num>=3)
	{
		for(int i=0;i<3;i++)
		{
			if(/*(godori_cnt>=i) && */(is_exist_equal(&tmp,godori+i,NULL,3)))
				godori_cnt++;
			else	
				break;
		}
	}

	if(godori_cnt>=3)
		score += 5;
	if(player->score.ten_num>=5)
		score += (player->score.ten_num - 5) + 1;

	return score;
}

int get_fiv_num(Card_List *gain, Card_List *tmp)
{
	if(gain==NULL||tmp==NULL)	return 0;
	if(gain->num_card==0)		return 0;

	Data find = {0,FIV_END};
	int fiv_num = is_exist_equal(gain, &find,tmp,1);

	return fiv_num;	
}

int get_score_fiv(Card_List *gain,Person *player)
{
	if(gain==NULL)				return 0;
	if(gain->num_card==0)		return 0;

	int cnt = 0, score = 0;
	Card_List tmp;
	Data hong[3] = {{1,0},{2,0},{3,0}};
	Data cheong[3] = {{6,0},{9,0},{10,0}};
	Data cho[3] = {{4,0},{5,0},{7,0}};

	init_card_list(&tmp);
	player->score.fiv_num = get_fiv_num(gain, &tmp);

	if(player->score.fiv_num>=3)
	{
		for(int i=0;i<3;i++)
		{
			if(cnt>=i && is_exist_equal(&tmp,hong+i,NULL,3))
				cnt++;
			else
				break;
		}

		if(cnt>=3)
			score += 3;
		cnt = 0;
		for(int i=0;i<3;i++)
		{
			if(cnt>=i && is_exist_equal(&tmp,cheong+i,NULL,3))
				cnt++;
			else
				break;
		}

		if(cnt>=3)
			score += 3;

		cnt = 0;
		for(int i=0;i<3;i++)
		{
			if(cnt>=i && is_exist_equal(&tmp,cho+i,NULL,3))
				cnt++;
			else
				break;
		}

		if(cnt>=3)
			score += 3;
	}

	if(player->score.fiv_num>=5)
		score += (player->score.fiv_num-5)+1;

	return score;
}

int get_pi_num(Card_List *gain, Card_List *tmp)
{
	if(gain==NULL||tmp==NULL)	return 0;
	if(gain->num_card==0)		return 0;

	Data find_n = {0,NORM_PI};
	Data find_s = {0,SSANG_PI};

	int pi_num = is_exist_equal(gain,&find_n,tmp,1);
	pi_num += 2*(is_exist_equal(gain,&find_s,tmp,1));

	return pi_num;		
}

int get_score_pi(Card_List *gain,Person *player)
{
	if(gain==NULL)					return 0;
	if(gain->num_card==0)			return 0;

	int score = 0;
	Card_List tmp;

	init_card_list(&tmp);
	player->score.pi_num = get_pi_num(gain,&tmp);

	if(player->score.pi_num>=10)
		score += (player->score.pi_num-10)+1;

	return score;	
}

int nagari(Game *game)
{
	if(game == NULL)		return 0;
	if(game->victor!=-1)	return 0;
	if(game->floor.num_card != 0) 	return 0;
	
	(game->nagari)++;

	return 1;
}

int gwang_bak(Person *player, int victor)
{
	if(player==NULL)		return 0;
	if(player[victor].score.gw_num<3)	return 0;
	if(player[(victor+1)%3].score.gw_num==0)	
		player[(victor+1)%3].gwang_bak = 1;
	if(player[(victor+2)%3].score.gw_num==0)	
		player[(victor+2)%3].gwang_bak = 1;

	return 1;
}

int meong_teong_gu_ri(Person *player, int victor)
{
	if(player==NULL)		return 0;
	if(player[victor].score.ten_num<7)	return 0;
	if(player[(victor+1)%3].score.ten_num==0)
		player[(victor+1)%3].meong_bak = 1;
	if(player[(victor+2)%3].score.ten_num==0)
		player[(victor+2)%3].meong_bak = 1;
}

int pi_bak(Person *player, int victor)
{
	if(player==NULL)	return 0;
	if(player[victor].score.sc<3)		return 0;
	if(player[victor].score.pi_num<10)	return 0;

	if(player[(victor+1)%3].score.pi_num<5)
		player[(victor+1)%3].pi_bak = 1;

	if(player[(victor+2)%3].score.pi_num<5)
		player[(victor+2)%3].pi_bak = 1;

	return 1;
}

int ddi_bak(Person *player, int victor)
{
	if(player==NULL)	return 0;
	if(player[victor].score.sc<3)	return 0;
	if(player[victor].score.fiv_num<5)	return 0;

	if(player[(victor+1)%3].score.fiv_num==0)
		player[(victor+1)%3].ddi_bak = 1;
	if(player[(victor+2)%3].score.fiv_num==0)
		player[(victor+2)%3].ddi_bak = 1;

	return 1;
}

int init_yeok_go(Person *player, int turn)
{
	if(player==NULL)		return 0;

	for(int i=0;i<3;i++)
	{
		player[i].go_bak = 0;
		if(i!=turn)
			for(int j=0;j<3;j++)
				player[i].yeok_go[j] = 0;
	}

	return 0;
}

int yeok_go(Game *game, int turn)
{
	if(game==NULL)		return 0;
	if(game->go_player==-1)	return 0;
	if((turn!=game->go_player) &&(turn!=game->pre_go))
			init_yeok_go(game->player,turn);

	game->pre_go = game->go_player;
	game->go_player = turn;

	game->player[turn].yeok_go[game->pre_go] = game->player[game->pre_go].yeok_go[turn]+1;

//	if(game->player[turn].yeok_go[game->pre_go]>2)
//			game->player[turn].yeok_go[game->pre_go] = 2;	

	game->player[game->pre_go].go_bak = 1;
	game->player[turn].go_bak = 0;
//	game->player[turn].score.pre_sc = game->player[turn].score.sc;
//	game->player[turn].score.sc *= (int)pow((double)2,(double)game->player[turn].yeok_go[game->pre_go]);

	return 1;
}

int go(Game *game, int turn)
{
	if(game == NULL)	return 0;

	if(game->go_player==-1)			//첫 고
	{
		game->go_player=turn;
		game->player[turn].score.sc++;
		game->player[turn].go=1;
	}
	else if(game->pre_go==-1)		// 역고 없이 계속 고
	{
		if(game->player[turn].go<2)
		{
			game->player[turn].go++;
			game->player[turn].score.sc++;
		}
		else						
		{
			game->player[turn].go++;
			game->player[turn].score.sc++;
		//	game->player[turn].score.sc*=2;
		}
	}
	else if(game->pre_go!=-1)			//역고 이후 계속 고
		game->player[turn].go_aft_yg++;

	return 1;
}

int sseul(Person *player, int turn)
{
	if(player == NULL)	return 0;

	give_PI_another_person(&(player[(turn+1)%3].gain), &(player[turn].gain));
	give_PI_another_person(&(player[(turn+2)%3].gain), &(player[turn].gain));

	return 1;
}

int is_pos_heun(Person *player, int month)
{
	if(player==NULL)		return 0;
	
	Data find;
	find.month = month;

	int num = is_exist_equal(&(player->hand),&find,NULL,3);
	if(num>=3)
	{
		player->eq_mon = month;
		return num;
	}

	return 0;
}

int is_pos_bomb(Card_List *floor, int month)
{
	if(floor==NULL)		return 0;

	Data find;
	find.month = month;

	int num = is_exist_equal(floor,&find,NULL,3);

	if(num==1)		return 1;

	return 0;
}

int bomb(Person *player, Card_List *floor, int turn)
{
	if(player==NULL||floor==NULL)		return 0;

	Card_List tmp_h, tmp_f;
	init_card_list(&tmp_h);
	init_card_list(&tmp_f);
	Data find;
	Data bomb_d = {0,BOMB};
	Card *bomb_c = NULL;
	find.month = player[turn].eq_mon;

	is_exist_equal(&(player[turn].hand),&find,&tmp_h,0);
	is_exist_equal(floor,&find,&tmp_f,0);
	for(int i=tmp_h.num_card;i>0;i--)
	{
		find_card(&tmp_h,1,&find);
		move_another_list(&(player[turn].hand),&(player[turn].gain),&find);
		delete_card(&tmp_h,find);
	}

	find_card(&tmp_f,1,&find);
	move_another_list(floor,&(player[turn].gain),&find);

	find_card(&tmp_f,1,&find);
	delete_card(&tmp_f,find);

	create_card(&bomb_c,&bomb_d);
	insert_card_list(&(player[turn].hand),bomb_c);
	create_card(&bomb_c,&bomb_d);
	insert_card_list(&(player[turn].hand),bomb_c);

	return 1;
}

int heun_deul_gi(Person *player,Card_List *pile,Card_List *floor,int inp,bool is_pos_b,int turn)
{
	if(player==NULL||pile==NULL||floor==NULL)		return 0;

	Card_List tmp;
	Data find;

	if(is_pos_b==0)
		take_card(player, pile,floor,inp,turn);
	else
		bomb(player,floor,turn);

	player->heun_deul++;

	return 1;
}

int take_card_equal(Person *player,Card_List *floor, int month, int turn)
{
	if(floor == NULL || player == NULL)	{return 0;}	

	Card_List tmp;
	Data find = {0,0}; 
	find.month = month;
	init_card_list(&tmp);
	int equal_num = is_exist_equal(floor,&find,&tmp,0);
	Card *card = NULL;
	int i;

	switch(equal_num)
	{
		case 2:	//쪽
		case 4: //따닥
			give_PI_another_person(&(player[(turn+1)%3].gain), &(player[turn].gain));
			give_PI_another_person(&(player[(turn+2)%3].gain), &(player[turn].gain));
			for(i=tmp.num_card;i>0;i--)
			{
				find_card(&tmp,1,&find);
				move_another_list(floor,&(player[turn].gain),&find);
				delete_card(&tmp, find);
			}
			break;
		default : 
			for(i=tmp.num_card;i>0;i--)
			{
				find_card(&tmp,1,&find);
				delete_card(&tmp, find);
			}
	}

	return 1;
}

int take_card_nequal(Person *player, Card_List *floor, Data find/*int month*/, int turn, int opt)
{
	if(player==NULL||floor==NULL)	return 0;

	Card_List tmp;
	init_card_list(&tmp);
//	Data find = {0,0};
	Data selc;
	Card *card = NULL;

//	find.month = month;

	int equal_num = is_exist_equal(floor,&find,&tmp,0);
	int i, cnt;

	switch(equal_num)
	{
		case 3:	//두장 중 한장 선택해서 가져감>> find에 해당하는 카드는 무조건 가저감 >> 낸 패 opt 					 =0
			// 뻑 >> 뒤집은 패 opt = 1
			if(opt)		break;
			move_another_list(floor,&(player[turn].gain),&find);
			delete_card(&tmp,find);
			printf("가져갈 카드를 입력하세요(1,2):");
			scanf("%d",&cnt);		
			find_card(&tmp,cnt,&selc);
			move_another_list(floor,&(player[turn].gain),&selc);
			break;
		case 4: //4장 다 가져가고 피 1장씩 받음
			give_PI_another_person(&(player[(turn+1)%3].gain), &(player[turn].gain));
			give_PI_another_person(&(player[(turn+2)%3].gain), &(player[turn].gain));
		case 2:	//그냥 가져감
			for(i=tmp.num_card;i>0;i--)
			{
				find_card(&tmp,1,&find);
				move_another_list(floor,&(player[turn].gain),&find);
				delete_card(&tmp, find);
			}
			break;
	}
	if(equal_num%2!=0)
		for(i=tmp.num_card;i>0;i--)
		{
			find_card(&tmp,1,&find);
			delete_card(&tmp, find);
		}
}

int take_card(Person *player, Card_List *pile, Card_List *floor, int cnt, int turn)
{
	if(player == NULL||pile==NULL||floor==NULL)		return 0;
	if(cnt > player[turn].hand.num_card)					return 0;


	Data hand_out;
	Data rvrs = {0,0};
	Card_List tmp;
	int equal_num;

	init_card_list(&tmp);
	find_card(&(player[turn].hand),cnt,&hand_out);
	if(hand_out.kind != BOMB)
		move_another_list(&(player[turn].hand),floor,&hand_out);
	else
		delete_card(&(player[turn].hand),hand_out);

	pile_to_other(pile,floor,&rvrs);
	if(hand_out.month == rvrs.month)
		take_card_equal(player,floor,hand_out.month,turn);
	else
	{
		take_card_nequal(player,floor,hand_out/*hand_out.month*/,turn,0);
		take_card_nequal(player,floor,rvrs/*rvrs.month*/,turn,1);
	}

	return 1;
}

int get_score(Person *player)
{
	if(player==NULL)	return 0;
	if(player->gain.num_card==0)	return 0;

	int score = 0;

	//딴 패 점수
	score += get_score_gw(&(player->gain),player);
//	printf("gw_sc : %d\n",score);
	score += get_score_ten(&(player->gain),player);
//	printf("t_sc : %d\n",score);
	score += get_score_fiv(&(player->gain),player);
//	printf("f_sc : %d\n",score);
	score += get_score_pi(&(player->gain),player);
//	printf("p_sc : %d\n",score);

	//고해서 얻은 점수	
	score+=player->go;
//	if(player->go>=3)
//		score = score*(int)pow((double)2,(double)player->go-2);

	return score;
}

int cal_multiple(Game *game)
{
	if(game==NULL)	return 0;
	if(game->victor==-1)	return 0;
	int victor = game->victor;
	int pre_go = game->pre_go;
	int i,j;
	int mul;
	
	if(game->player[victor].go>=3)
		game->player[victor].score.sc *=(int)pow((double)2,(double)game->player[victor].go-2);
	//if(game->nagari!=0)

	//역고
	if(pre_go!=-1)
	{
		for(i=0;i<3;i++)
		{
			if(game->player[victor].yeok_go[i]%2==1)
			{
				j=1;
				mul = 2;
			}
			else
			{
				j=2;
				mul=4;
			}
			for(;j<=game->player[victor].yeok_go[i];j+=2)
			{
				if(j==1)
					mul = 2;
				else 
					mul = 4;
				game->player[victor].score.sc *= mul;
			}
		}
		game->player[victor].score.sc *= (int)pow((double)2,(double)game->player[victor].go_aft_yg);
	}

	//흔들기, 폭탄
	for(i=0;i<game->player[victor].heun_deul;i++)
		game->player[victor].score.sc *= 2;
	
	return 1;
}

int take_money(Game *game)
{
	if(game==NULL)		return 0;
	if(game->victor==-1)	return 0;
	
	int victor = game->victor;
	int money[3] = {0,0,0};
	int go_bak[3] = {0,0,0};
	int cur;
	int flag = 0;

	for(int i=1;i<3;i++)
	{
		cur = (victor+i)%3;
		money[cur] = game->per_mon * game->player[victor].score.sc;
		gwang_bak(game->player,victor);
		ddi_bak(game->player,victor);
		pi_bak(game->player,victor);
		meong_teong_gu_ri(game->player, victor);
		if(game->nagari)
			money[cur] *=2;
		if(game->player[cur].go_bak)
			go_bak[cur] = 1;
		if(game->player[cur].gwang_bak)
			money[cur] *= 2;
		if(game->player[cur].meong_bak)
			money[cur] *= 2;
		if(game->player[cur].pi_bak)
			money[cur] *= 2;
		if(game->player[cur].ddi_bak)
			money[cur] *= 2;
	}

	if((go_bak[(victor+1)%3]==0)&&(go_bak[(victor+2)%3]==1))
	{
		game->player[victor].balance += money[(victor+2)%3];
		game->player[(victor+2)%3].balance -= money[(victor+2)%3];
	}
	else if((go_bak[(victor+1)%3]==1)&&(go_bak[(victor+2)%3]==0))
	{
		game->player[victor].balance += money[(victor+1)%3];
		game->player[(victor+1)%3].balance -= money[(victor+1)%3];
	}
	else if((go_bak[(victor+1)%3]==0)&&(go_bak[(victor+2)%3]==0))
	{
		game->player[victor].balance += money[(victor+2)%3];
		game->player[(victor+2)%3].balance -= money[(victor+2)%3];

		game->player[victor].balance += money[(victor+1)%3];
		game->player[(victor+1)%3].balance -= money[(victor+1)%3];
	}
	

	return 1;
}
