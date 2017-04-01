#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "card.h"
#include "score.h"
#include "interface.h"

int main(void)
{
	Game game;
	Data find;
	Data tog[2] = {{9,SSANG_PI},{9,TEN_END}};

	char command[11];
	int turn;
	int is_pos_h = 0;
	int is_pos_b = 0;
	int flag = 0;
	int y_n = 0;


	srand(time(NULL));
	init_game(&game,0,0);

	while(1)
	{

		for(int i=0;i<7;i++)
		{
			find_card(&(game.pile),1,&find);
			move_another_list(&(game.pile),&(game.floor),&find);
		}

		for(int i=0;i<3;i++)
		{
			for(int j=0;j<7;j++)
			{
				find_card(&(game.pile),1,&find);
				move_another_list(&(game.pile),&(game.player[i].hand),&find);
			}
		}
		system("clear");
		printf("\n\n\n\n\n\n\n\n\n");
		printf("turn : %d\n",turn);
		print_scr(&game,turn%3);
		while(game.victor == -1)
		{
			printf("명령 : ");
			scanf("%s",&command);
			printf("command : %s\n",command);

			if((atoi(command)>=1)&& (atoi(command)<=7))
			{
				find_card(&(game.player[turn%3].hand),atoi(command),&find);
				is_pos_h = is_pos_heun(game.player+turn%3,find.month);
				if(is_pos_h==3)						//흔들기, 폭탄
				{
					printf("흔들기를 하시겠습니까(yes=1,no=0) : ");
					scanf("%d",&y_n);	
					if(y_n==1)
					{
						is_pos_b = is_pos_bomb(&(game.floor),find.month);
						heun_deul_gi(game.player,&(game.pile),&(game.floor),atoi(command),is_pos_b,turn									%3);
					}
					else
					{
						if(take_card(game.player, &(game.pile),&(game.floor),atoi(command),turn%3)==0)
						{
							printf("오류 입니다. 다시 입력하세요\n");
							continue;
						}
					}

				}
				else if(is_pos_h==4)				//총통
				{
					game.victor = turn % 3;
					game.player[turn%3].score.sc = 10;
					game.player[turn%3].chong_tong = 1;
					break;
				}
				else
				{
					if(take_card(game.player, &(game.pile),&(game.floor),atoi(command),turn%3)==0)
					{
						printf("오류 입니다. 다시 입력하세요\n");
						continue;
					}
				}
			}
			else if(strcmp(command,"9")==0)
			{
				if(game.pos_tog)
				{
					if(game.guk_pi_ten == 0)
						game.guk_pi_ten = 1;
					else 
						game.guk_pi_ten = 0;
					for(int i=0;i<3;i++)
					{
						for(Card *card=game.player[i].hand.head.next;card;card=card->next)
						{
							if(card->data.month==tog[1-game.guk_pi_ten].month 
									&& card->data.kind==tog[1-game.guk_pi_ten].kind)
							{
								card->data.month = tog[game.guk_pi_ten].month;
								card->data.kind  = tog[game.guk_pi_ten].kind;
								break;
							}
						}
						for(Card *card=game.player[i].gain.head.next;card;card=card->next)
						{
							if(card->data.month==tog[1-game.guk_pi_ten].month 
									&& card->data.kind==tog[1-game.guk_pi_ten].kind)
							{
								card->data.month = tog[game.guk_pi_ten].month;
								card->data.kind  = tog[game.guk_pi_ten].kind;
								break;
							}
						}
					}	
					for(Card *card=game.floor.head.next;card;card=card->next)
					{
						if(card->data.month==tog[1-game.guk_pi_ten].month 
								&& card->data.kind==tog[1-game.guk_pi_ten].kind)
						{
							printf("in floor\n");
							card->data.month = tog[game.guk_pi_ten].month;
							card->data.kind  = tog[game.guk_pi_ten].kind;
							break;
						}
					}
					for(Card *card=game.pile.head.next;card;card=card->next)
					{
						if(card->data.month==tog[1-game.guk_pi_ten].month 
								&& card->data.kind==tog[1-game.guk_pi_ten].kind)
						{
							printf("in pile\n");
							card->data.month = tog[game.guk_pi_ten].month;
							card->data.kind  = tog[game.guk_pi_ten].kind;
							break;
						}
					}
					game.pos_tog = 0;
				}
				else 
					printf("토글이 불가능 합니다.\n");
				game.player[turn%3].score.pre_sc = game.player[turn%3].score.sc;
				game.player[(turn+1)%3].score.pre_sc = game.player[(turn+1)%3].score.sc;
				game.player[(turn+2)%3].score.pre_sc = game.player[(turn+2%3)].score.sc;

				game.player[turn%3].score.sc = get_score(game.player + turn%3);
				game.player[(turn+1)%3].score.sc = get_score(game.player + (turn+1)%3);
				game.player[(turn+2)%3].score.sc = get_score(game.player + (turn+2)%3);
				printf("turn : %d\n",turn);
				//system("clear");
				print_scr(&game,turn%3);
				continue;
			}
			else if(strcmp(command,"e")==0)
				break;
			else if(strcmp(command,"b")==0)
				printf("\n");
			else if(strcmp(command,"h")==0)
				printf("\n");
			else if(strcmp(command,"save")==0)
			{
				save(&game);
				continue;
			}
			else if(strcmp(command,"load")==0)
			{
				reset_game(&game);
				load(&game);
				system("clear");
				printf("\n\n\n\n\n\n\n\n\n");
				printf("turn : %d\n",turn);
				print_scr(&game,turn%3);
				continue;
			}
			else
			{
				printf("오류 입니다. 다시 입력하세요\n");
				continue;
			}

			game.player[turn%3].score.pre_sc = game.player[turn%3].score.sc;
			game.player[(turn+1)%3].score.pre_sc = game.player[(turn+1)%3].score.sc;
			game.player[(turn+2)%3].score.pre_sc = game.player[(turn+2%3)].score.sc;


			game.player[turn%3].score.sc = get_score(game.player + turn%3);
			game.player[(turn+1)%3].score.sc = get_score(game.player + (turn+1)%3);
			game.player[(turn+2)%3].score.sc = get_score(game.player + (turn+2)%3);

			//마지막 턴
			if((game.pile.num_card==0)&&(game.floor.num_card==0))
				flag = 1;
			else if(game.floor.num_card==0)
				sseul(game.player, turn%3);

			//난 경우
			if(game.player[turn%3].score.sc>=3 && game.player[turn%3].score.sc 
					> game.player[turn%3].score.pre_sc)
			{
				if(flag!=1)
				{
					printf("go하시겠습니까?(g=go,s=stop) : ");
					scanf("%s",command);	
				}
				else
					strcpy(command,"s");

				//역고
				if(game.go_player!=-1 && game.go_player!=turn%3)
				{
					yeok_go(&game, turn%3);
					if(strcmp(command,"s")==0)	
					{
						game.victor = turn%3;		
						break;
					}
				}
				//그냥 stop 	
				else if(strcmp(command,"s")==0)	
				{
					game.victor = turn%3;		
					break;
				}
				//그냥 고 	
				else if(strcmp(command,"g")==0)
				{
					go(&game, turn%3);
					game.go_player = turn%3;
				}

			}
			else if(flag == 1)		//마지막 턴에 아무도 점수 안난 경우 >> 나가리
			{
				//			system("clear");
				printf("turn : %d\n",turn);
				print_scr(&game,turn%3);
				nagari(&game);
				break;
			}
			//system("clear");
			turn++;
			printf("\n\n\n\n\n\n\n\n\n\n");
			printf("turn : %d\n",turn);
			print_scr(&game,turn%3);

			while((getchar()!='\n'))	{}
		}
		cal_multiple(&game);
		take_money(&game);
		if(game.victor!=-1)
			init_game(&game,0,game.first);
		else
			init_game(&game,0,game.victor);
	}

	/*
	   printf("nagari : %d\n",game.nagari);
	   printf("victor : %d\n",game.victor);
	   for(int i=0;i<3;i++)
	   {
	   printf("player[%d]\n",i);
	   printf("go : %d\n",game.player[i].go);
	   printf("go_aft_yg : %d\n",game.player[i].go_aft_yg);
	   printf("gwang_bak : %d\n",game.player[i].gwang_bak);
	   printf("meong_bak : %d\n",game.player[i].meong_bak);
	   printf("ddi_bak : %d\n",game.player[i].ddi_bak);
	   printf("go_bak : %d\n",game.player[i].go_bak);
	   printf("heun_deul : %d\n",game.player[i].heun_deul);
	   printf("chong_tong : %d\n",game.player[i].chong_tong);
	   printf("balance : %d\n",game.player[i].balance);
	   printf("score : %d\n",game.player[i].score.sc);
	   printf("\n\n\n");
	   }

	   printf("[0].gain : \n");
	   print_card_list(&(game.player[0].gain))	;
	   printf("[1].gain : \n");
	   print_card_list(&(game.player[1].gain))	;
	   printf("[2].gain : \n");
	   print_card_list(&(game.player[2].gain))	;
	   */
	return 0;
}
