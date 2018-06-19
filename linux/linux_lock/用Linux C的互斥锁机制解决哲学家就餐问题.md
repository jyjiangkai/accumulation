用Linux C的互斥锁机制解决哲学家就餐问题（简单实现）

在1971年，著名的计算机科学家艾兹格·迪科斯彻提出了一个同步问题，即假设有五台计算机都试图访问五份共享的磁带驱动器。稍后，这个问题被托尼·霍尔重新表述为哲学家就餐问题。这个问题可以用来解释死锁和资源耗尽。

   「哲学家就餐说明」：有五个哲学家共用一张圆桌，分别坐在周围的五张椅子上，在圆桌上有五个筷子，他们的生活方式是交替进行思考和就餐，通常，一个哲学家饥饿时就会试图去取用其左右最靠近自己的筷子，只有拿到两个筷子才能就餐，就餐完毕，放下筷子继续思考！
   
   但其中存在这一中问题，当五个人同时饥饿的时候，每个人拿着自己左边或右边的一只筷子，就会产生「死锁」状态，当然我们可以给每个人加上时间，等待几分钟后就必须放下手中的筷子，这种情况貌似解决饿了死锁问题，但是没法避免所谓的「活锁」，在计算机中，五条线程同时进入同一状态，拿起五只筷子。所以，这种情况在计算机资源分配就会是不合理的。

   在实际的计算机问题中，缺乏餐叉可以类比为缺乏共享资源。计算机技术是常用的是所谓的资源加锁，用来保证在某个时刻，资源只能被一个程序或一段代码访问。当一个程序想要使用的资源已经被另一个程序锁定，它就等待资源解锁。当多个程序涉及到加锁的资源时，在某些情况下就有可能发生死锁。例如，某个程序需要访问两个文件，当两个这样的程序各锁了一个文件，那它们都在等待对方解锁另一个文件，而这永远不会发生。

  所以在这里我们用计算机中的五个条件变量来模拟五只筷子的状态，用五条线程模拟五个philosopher，他们所能进行的操作便是：


 while(1){
   thinking();         //思考
   take_forks();       //拿筷子
   eating();           //吃饭
   put_down_forks();  }  //放下筷子
   对于可能产生的死锁问题，我们这里采用一中解决的办法，那就是只有当哲学接的左右两只筷子均处于可用状态时，才允许他拿起筷子。这样就可以避免他们同时拿起筷子就餐，导致死锁。

   下面是利用Linux线程互斥锁机制对这个问题的一种代码实现：


#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define phi_num 5
#define think_time 2
#define eat_time 1
#define left (phi_id+phi_num-1)%phi_num
#define right (phi_id+1)%phi_num

enum { think , hungry , eat } phi_state[phi_num];
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t state[phi_num]={PTHREAD_MUTEX_INITIALIZER};

void Do_think(int phi_id){
	printf(" philosopher %d is thinking now !\n",phi_id);
	sleep(think_time);
}
void Do_eat(int phi_id){
	printf("philosopher %d is eating now !\n",phi_id);
	sleep(eat_time);
}
void check_phi_state(int phi_id){
	if(phi_state[phi_id]==hungry&&phi_state[left]!=eat&&phi_state[right]!=eat){
		phi_state[phi_id]=eat;
		pthread_mutex_unlock(&state[phi_id]);
	}
}
void Do_take_forks(int phi_id){
	pthread_mutex_lock(&mutex);
	phi_state[phi_id]=hungry;
	check_phi_state(phi_id);
	pthread_mutex_unlock(&mutex);
	pthread_mutex_lock(&state[phi_id]);
}
void Do_put_forks(int phi_id){
	pthread_mutex_lock(&mutex);
	phi_state[phi_id]=think;
	check_phi_state(left);
	check_phi_state(right);
	pthread_mutex_unlock(&mutex);
}
void *philosopher(void *arg){
	int phi_id=*(int *)arg;
	while(1){
		Do_think(phi_id);
		Do_take_forks(phi_id);
		Do_eat(phi_id);
		Do_put_forks(phi_id);
	}
	return NULL;
}
int main(int argc, char *argv[]){
	int num;
	pthread_t *phi=(pthread_t*)malloc(sizeof(pthread_t)*phi_num);
	int *id=(int *)malloc(sizeof(int)*phi_num);
	for(num=0;num<phi_num;num++){
		id[num]=num;
		pthread_create(&phi[num],NULL,philosopher,(void*)(&id[num]));
	}
	for(num=0;num<phi_num;num++){
		pthread_join(phi[num],NULL);
	}
	return 0;
}
编译：

clang -o philosopher philosopher.c -Wall -lpthread
     当然还有其他的策略，比如至多允许有四位哲学家同时去拿左边的筷子，最终可以保证至少有一位哲学家进餐，并在就餐完毕，释放他的筷子资源，其他的philosopher就有就会可以就餐了。


还有一中策略就是引入服务生的概念（Wiki上）：哲学家必须经过他的允许才能拿起餐叉。因为服务生知道哪只餐叉正在使用，所以他能够作出判断避免死锁。为了演示这种解法，假设哲学家依次标号为A至E。如果A和C在吃东西，则有四只餐叉在使用中。B坐在A和C之间，所以两只餐叉都无法使用，而D和E之间有一只空余的餐叉。假设这时D想要吃东西。如果他拿起了第五只餐叉，就有可能发生死锁。相反，如果他征求服务生同意，服务生会让他等待。这样，我们就能保证下次当两把餐叉空余出来时，一定有一位哲学家可以成功的得到一对餐叉，从而避免了死锁。