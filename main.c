#include "ui.h"
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include <unistd.h>

pthread_mutex_t lock;
int gold = 100;
int zombies = 1;
int soldiers = 0;
int health = 100;

void *minerfunc(void *ptr){
	while(1){
		pthread_mutex_lock(&lock);
			gold = gold + 10;
			print_gold(gold);
		pthread_mutex_unlock(&lock);
		sleep(1);
	}
}

void *zombiefunc(void *ptr){
	
	while(1){
		for(int i = 5; i > 0; i--){
			pthread_mutex_lock(&lock);
				print_zombies(i, zombies);
			pthread_mutex_unlock(&lock);
			sleep(1);
		}

		pthread_mutex_lock(&lock);
		if(zombies > soldiers){

			health = health - (zombies - soldiers);
			print_health(health);
			print_fail("Zombie attack succeded ;(!");

			if(health < 0){
				game_end(zombies);
			}

			zombies = zombies * 2;
		}
		else{
			print_succ("Zombie attack deflected! :)");
				zombies = zombies * 2;
		}
		pthread_mutex_unlock(&lock);
	}
}

int main() {
	init();
	print_gold(100);
	print_soldiers(0);
	print_zombies(5,1);
	print_health(100);

	pthread_t zombie;

	pthread_create(&zombie, NULL, zombiefunc, NULL);


	while(1) {
		int ch = get_input();
		pthread_t miner;
		switch(ch) {
			case 'q':
				pthread_mutex_lock(&lock);
				game_end(zombies);
				break;
				pthread_mutex_unlock(&lock);
			case 'm':
				pthread_mutex_lock(&lock);
				if(gold > 99){
					pthread_create(&miner, NULL, minerfunc, NULL);
					gold = gold - 100;       		
					print_gold(gold);
					print_msg("Miner created!");
				}
				else{
					print_fail("Not enough gold!");
				}
				pthread_mutex_unlock(&lock);
				break;
			case 's':
				pthread_mutex_lock(&lock);
				if(gold > 9){
					soldiers = soldiers + 1;
					gold = gold - 10;
					print_soldiers(soldiers);
					print_gold(gold);
					print_msg("Soldier created!");
				}
				else{
					print_fail("Not enough gold!");
				}
				pthread_mutex_unlock(&lock);
				break;
			case 'x':
				pthread_mutex_lock(&lock);
				if(gold > 99){
					soldiers = soldiers + 10;
					gold = gold - 100;
					print_soldiers(soldiers);
					print_gold(gold);
					print_msg("10 x soldier created!");
				}
				else{
					print_fail("Not enough gold!");
				}
				pthread_mutex_unlock(&lock);
				break;

		}
	}
}
