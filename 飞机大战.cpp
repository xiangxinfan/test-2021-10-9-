#define _CRT_SECURE_NO_WARNINGS 1
/*
     ���ݣ�΢�ŷɻ���ս
	 ֪ʶ�㣺�ṹ�壬ѭ�����������


	 ͼƬ������������������Ͻ�Ϊԭ�㣬����Ϊx�ᣬ����Ϊy��
*/

#include <stdio.h>
#include <graphics.h>//���ǰ���ͼ�ο��ͷ�ļ������ֽ�EasyX
//��ȻҪ����EasyX,��Ȼ���ᱨ����Ϊ�Ҳ���
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")//���ض�ý���豸��


//�ǲ�ס���ݣ��Ǿ��ú궨�������
#define WIN_WIDTH 591
#define WIN_HEIGHT 864

//�����ӵ�����
#define PLAYER_BULLET 15
//�л��������
#define ENEMYPLANE_NUM 10

//����һ��ͼƬ
IMAGE background;
//�������
IMAGE img_player[4];
//�����ӵ�
IMAGE img_bullet[2];
//����л�
IMAGE img_enemy[2];

struct Plane
{
	int x;
	int y;
	bool flag;//�жϷɻ��Ƿ�����
	int width;//ͼƬ�Ŀ��
	int height;//ͼƬ�ĸ߶�
}player,player_bullet[PLAYER_BULLET],enemy[ENEMYPLANE_NUM];//�ӵ�,�л�Ҳ�иߣ����
//����ʱ��仯��

DWORD t1, t2;

//��ģ�鴦��ר�ŵ����飬��ר�ŵĺ���ȥ��

//��Ϸ��ʼ��
void GameInit()
{
	//1������һ��ͼ�δ���
	initgraph(WIN_WIDTH, WIN_HEIGHT);//�����Ǹ�ͼƬ�ĳ���
	
	//�������������
	srand(GetTickCount());

	//2��������ͼƬ\
	  //2.1����ͼƬ��background��
	//����
	loadimage(&background, "./�ɻ���ս�ز�/background.jpg");
	//�����ɻ�
	loadimage(&img_player[0], "./�ɻ���ս�ز�/planeNormal_1.jpg");
	loadimage(&img_player[1], "./�ɻ���ս�ز�/planeNormal_2.jpg");
	//��ը�ɻ�
	loadimage(&img_player[2], "./�ɻ���ս�ز�/planeBoom_1.jpg");
	loadimage(&img_player[3], "./�ɻ���ս�ز�/planeBoom_2.jpg");
	//�ӵ�
	loadimage(&img_bullet[0], "./�ɻ���ս�ز�/bullet1.jpg"); 
	loadimage(&img_bullet[1], "./�ɻ���ս�ز�/bullet2.jpg"); 
	//�л�
	loadimage(&img_enemy[0], "./�ɻ���ս�ز�/enemyPlane1.jpg");
	loadimage(&img_enemy[1], "./�ɻ���ս�ز�/enemyPlane2.jpg");

	//��Ϊ��һ��������ָ�룬���Ը���ȡ��ַ����
	//������Ҫ����Ŀ�����е��ַ����ĳ�ʹ�ö��ֽ��ַ���

	//3���������֣����ȴ����֣�Ȼ�󲥷�����
	mciSendString("open ./�ɻ���ս�ز�/game_music.mp3 alias BGM", 0, 0, 0);//��ȻҪ��ͷ�ļ�
	//alias����˼�Ǳ��������Ǹ������ַȡһ������
	mciSendString("play BGM repeat", 0, 0, 0);//repeat ���ظ�����

	//��ʼ��������ݣ����ɻ�һ��ʼ���õ�ͼƬ��������м�
	player.height = 120;
	player.width = 117;
	player.x = WIN_WIDTH / 2 - player.width / 2;
	player.y = WIN_HEIGHT - player.height;
	player.flag = true;

	//��ʼ���ӵ����ṹ������Ҳ�����飬��ʼ������Ҫ��ѭ��
	for (int i = 0; i < PLAYER_BULLET; i++)
	{
		player_bullet[i].flag = false;
		player_bullet[i].height = 24;
		player_bullet[i].width  = 19;
	}
	//��ʼ��ʱ�����
	t1 = t2 = GetTickCount();
	//GetTickCount���أ�retrieve���Ӳ���ϵͳ������������elapsed���ĺ����������ķ���ֵ��DWORD��

	//��ʼ���л�
	for (int i = 0; i < ENEMYPLANE_NUM; i++)
	{
		enemy[i].flag = false;
		enemy[i].width = 49;
		enemy[i].height = 40;
	}
}

void CreatBullet();
//���ݷɻ������п��ɻ�
void KeyControl(int speed)
{
	//���ǰ���������getch()
	//�����ƶ�    �߽紦��һ��
	if (GetAsyncKeyState(VK_UP) && player.y >= 0)
	{
		player.y -= speed;
	}
	//�����ƶ�
	if (GetAsyncKeyState(VK_DOWN) && player.y + player.height <= WIN_HEIGHT)//����ͼƬ�ĸ߶�Ҫ����
	{
		player.y += speed;
	}
	//�����ƶ�
	if (GetAsyncKeyState(VK_LEFT) && player.x + player.width / 2.5 >= 0)
	{
		player.x -= speed;
	}
	//�����ƶ�
	if (GetAsyncKeyState(VK_RIGHT) && player.x + player.width / 2 <= WIN_WIDTH)
	{
		player.x += speed;
	}
	//�����ӵ�
	if (GetAsyncKeyState(VK_SPACE) && t2 - t1 > 200)
	{
		mciSendString("close gun", 0, 0, 0);
		mciSendString("open ./�ɻ���ս�ز�/f_gun.mp3 alias gun", 0, 0, 0);
		mciSendString("play gun", 0, 0, 0);
		//����һ���ӵ�
		CreatBullet();
		printf("����������Ҫ�����ˣ�����");
		t1 = t2;
	}
	t2 = GetTickCount();
}

//����һ��������ר��������ͼ
void GameDarw()
{
	//����֮�����е���
	// ����˫�����ͼ����ֹ������˸
	BeginBatchDraw();

	//2.2���ͼƬ
	putimage(0, 0, &background);
	if (player.flag)
	{
		putimage(player.x, player.y, &img_player[0], NOTSRCERASE);
		putimage(player.x, player.y, &img_player[1], SRCERASE);
	}

	//��������ӵ�
	for (int i = 0; i < PLAYER_BULLET; i++)
	{
		if (player_bullet[i].flag)
		{
			putimage(player_bullet[i].x, player_bullet[i].y, &img_bullet[0], NOTSRCERASE);
			putimage(player_bullet[i].x, player_bullet[i].y, &img_bullet[1], SRCERASE);
		}
	}
	//���Ƶл�
	for (int i = 0; i < ENEMYPLANE_NUM; i++)
	{
		if (enemy[i].flag)
		{
			putimage(enemy[i].x, enemy[i].y, &img_enemy[0], NOTSRCERASE);
			putimage(enemy[i].x, enemy[i].y, &img_enemy[1], SRCERASE);
		}
	}

	EndBatchDraw();

}

//����һ���ӵ�
void CreatBullet()
{
	for (int i = 0; i < PLAYER_BULLET; i++)
	{
		if (!player_bullet[i].flag)
		{
			player_bullet[i].x = player.x + 43;
			player_bullet[i].y = player.y;
			player_bullet[i].flag = true;
			break;
		}
	}
}
//����ӵ��ƶ�
void BulletMove(int speed)
{
	for (int i = 0; i < PLAYER_BULLET; i++)
	{
		if (player_bullet[i].flag)
		{
			player_bullet[i].y -= speed;
			if (player_bullet[i].y < 0)
			{
				player_bullet[i].flag = false;//�����ǽ�15���ӵ�ͬһʱ��ȫ�������ȥ��
				//����Ҫ��ʼ��ʱ�����
			}
		}
	}
}

//���ɵл�
void CreateEnemy()
{
	for (int i = 0; i < ENEMYPLANE_NUM; i++)
	{
		if (!enemy[i].flag)
		{
			enemy[i].y = 0;
			enemy[i].x = rand() % (WIN_WIDTH - enemy[i].width);
			enemy[i].flag = true;
			break;
		}
	}
}

//�л��ƶ�
void EnemyMove(int speed)
{
	for (int i = 0; i < ENEMYPLANE_NUM; i++)
	{
		if (enemy[i].flag)
		{
			enemy[i].y += speed;
			//�����߽紦��
			if (enemy[i].y > WIN_HEIGHT)
			{
				enemy[i].flag = false;
			}
		}
	}
}

//��ɻ�
void PlayPlane()
{
	//ÿ���о���ÿ���ӵ���Ҫ�Ƚ�
	for (int i = 0; i < ENEMYPLANE_NUM; i++)
	{
		if (!enemy[i].flag)
			continue;
		//�����ӵ�����
		for (int k = 0; k < PLAYER_BULLET; k++)
		{
			if (!player_bullet[k].flag)
				continue;
			//�ж��ӵ��Ƿ���ел�
			if (player_bullet[k].x > enemy[i].x && player_bullet[k].x <enemy[i].x + 
				enemy[i].width&& player_bullet[k].y>enemy[i].y && player_bullet[k].y 
				< enemy[i].y + enemy[i].height)
			{
				//�����˵л�Ҫ��ʧ���ӵ�ҲҪ��ʧ
				enemy[i].flag = false;
				player_bullet[i].flag = false;
				//����+=10
			}
		}
	}
}


int main()
{
	GameInit();
	////2.2���ͼƬ
	//putimage(0, 0, &background);//��һ��������ԭ��
	////putimage(0, 0, &player[1]);
	////���֮��ᷢ����ҷɻ��ı����Ǵ��ڵģ�������Ҫ����ҷɻ��ı����ĳ�͸����

	////�����ǽ��зɻ������ĸı�
	//   //putimage(0, 0, &img_player[0],NOTSRCERASE);
	//	  //�Ƚ�Դ����ͼ����Ŀ�����ͼ����в���"��"���㣬Ȼ���ٽ���ͼ����з���.
	//   //putimage(0, 0, &img_player[1],SRCERASE;
	//	 //��Ŀ�����ͼ����з��࣬����Դ����ͼ����в���"��"����


	////֮��Ҫ�ı�ͼƬ��λ�ã���Ҫ�ı������λ��
	////��ʱ���ֵ��ǽṹ�������

	//putimage(player.x, player.y, &img_player[0], NOTSRCERASE);
	//putimage(player.x, player.y, &img_player[1], SRCERASE);

	//KeyControl(1);
	////�������ǲ����ƶ��ģ���Ҫһ��ѭ��

	while (1)//����Ū����ѭ��
	{
		GameDarw();
		KeyControl(1);
		BulletMove(1);
		if (rand() % 50 == 1)
		{
			CreateEnemy();
		}
		PlayPlane();
		EnemyMove(1);
	}


	getchar();//��ֹ����
	return 0;
}