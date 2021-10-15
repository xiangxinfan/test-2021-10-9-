#define _CRT_SECURE_NO_WARNINGS 1
/*
     内容：微信飞机大战
	 知识点：结构体，循环，控制语句


	 图片在这里的坐标是以左上角为原点，向右为x轴，向下为y轴
*/

#include <stdio.h>
#include <graphics.h>//这是包含图形库的头文件，名字叫EasyX
//当然要下载EasyX,不然这句会报错，因为找不到
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")//加载多媒体设备库


//记不住数据，那就用宏定义来解决
#define WIN_WIDTH 591
#define WIN_HEIGHT 864

//最大的子弹数量
#define PLAYER_BULLET 15
//敌机最大数量
#define ENEMYPLANE_NUM 10

//定义一个图片
IMAGE background;
//定义玩家
IMAGE img_player[4];
//定义子弹
IMAGE img_bullet[2];
//定义敌机
IMAGE img_enemy[2];

struct Plane
{
	int x;
	int y;
	bool flag;//判断飞机是否死亡
	int width;//图片的宽度
	int height;//图片的高度
}player,player_bullet[PLAYER_BULLET],enemy[ENEMYPLANE_NUM];//子弹,敌机也有高，宽等
//定义时间变化量

DWORD t1, t2;

//分模块处理专门的事情，用专门的函数去做

//游戏初始化
void GameInit()
{
	//1、创建一个图形窗口
	initgraph(WIN_WIDTH, WIN_HEIGHT);//这是那个图片的长度
	
	//设置随机数种子
	srand(GetTickCount());

	//2、贴背景图片\
	  //2.1加载图片到background中
	//背景
	loadimage(&background, "./飞机大战素材/background.jpg");
	//正常飞机
	loadimage(&img_player[0], "./飞机大战素材/planeNormal_1.jpg");
	loadimage(&img_player[1], "./飞机大战素材/planeNormal_2.jpg");
	//爆炸飞机
	loadimage(&img_player[2], "./飞机大战素材/planeBoom_1.jpg");
	loadimage(&img_player[3], "./飞机大战素材/planeBoom_2.jpg");
	//子弹
	loadimage(&img_bullet[0], "./飞机大战素材/bullet1.jpg"); 
	loadimage(&img_bullet[1], "./飞机大战素材/bullet2.jpg"); 
	//敌机
	loadimage(&img_enemy[0], "./飞机大战素材/enemyPlane1.jpg");
	loadimage(&img_enemy[1], "./飞机大战素材/enemyPlane2.jpg");

	//因为第一个参数是指针，所以给它取地址即可
	//这里需要将项目属性中的字符集改成使用多字节字符集

	//3、播放音乐，首先打开音乐，然后播放音乐
	mciSendString("open ./飞机大战素材/game_music.mp3 alias BGM", 0, 0, 0);//当然要加头文件
	//alias的意思是别名，就是给这个地址取一个别名
	mciSendString("play BGM repeat", 0, 0, 0);//repeat 是重复播放

	//初始化玩家数据，将飞机一开始放置到图片下面的最中间
	player.height = 120;
	player.width = 117;
	player.x = WIN_WIDTH / 2 - player.width / 2;
	player.y = WIN_HEIGHT - player.height;
	player.flag = true;

	//初始化子弹，结构体数组也是数组，初始化数组要用循环
	for (int i = 0; i < PLAYER_BULLET; i++)
	{
		player_bullet[i].flag = false;
		player_bullet[i].height = 24;
		player_bullet[i].width  = 19;
	}
	//初始化时间变量
	t1 = t2 = GetTickCount();
	//GetTickCount返回（retrieve）从操作系统启动所经过（elapsed）的毫秒数，它的返回值是DWORD。

	//初始化敌机
	for (int i = 0; i < ENEMYPLANE_NUM; i++)
	{
		enemy[i].flag = false;
		enemy[i].width = 49;
		enemy[i].height = 40;
	}
}

void CreatBullet();
//操纵飞机，进行开飞机
void KeyControl(int speed)
{
	//这是按键处理，用getch()
	//向上移动    边界处理一下
	if (GetAsyncKeyState(VK_UP) && player.y >= 0)
	{
		player.y -= speed;
	}
	//向下移动
	if (GetAsyncKeyState(VK_DOWN) && player.y + player.height <= WIN_HEIGHT)//还有图片的高度要加上
	{
		player.y += speed;
	}
	//向左移动
	if (GetAsyncKeyState(VK_LEFT) && player.x + player.width / 2.5 >= 0)
	{
		player.x -= speed;
	}
	//向右移动
	if (GetAsyncKeyState(VK_RIGHT) && player.x + player.width / 2 <= WIN_WIDTH)
	{
		player.x += speed;
	}
	//发射子弹
	if (GetAsyncKeyState(VK_SPACE) && t2 - t1 > 200)
	{
		mciSendString("close gun", 0, 0, 0);
		mciSendString("open ./飞机大战素材/f_gun.mp3 alias gun", 0, 0, 0);
		mciSendString("play gun", 0, 0, 0);
		//产生一个子弹
		CreatBullet();
		printf("啊啊啊，我要发射了！！！");
		t1 = t2;
	}
	t2 = GetTickCount();
}

//定义一个函数，专门用来画图
void GameDarw()
{
	//编译之后画面有点闪
	// 运用双缓冲绘图，防止画面闪烁
	BeginBatchDraw();

	//2.2输出图片
	putimage(0, 0, &background);
	if (player.flag)
	{
		putimage(player.x, player.y, &img_player[0], NOTSRCERASE);
		putimage(player.x, player.y, &img_player[1], SRCERASE);
	}

	//绘制玩家子弹
	for (int i = 0; i < PLAYER_BULLET; i++)
	{
		if (player_bullet[i].flag)
		{
			putimage(player_bullet[i].x, player_bullet[i].y, &img_bullet[0], NOTSRCERASE);
			putimage(player_bullet[i].x, player_bullet[i].y, &img_bullet[1], SRCERASE);
		}
	}
	//绘制敌机
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

//产生一个子弹
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
//玩家子弹移动
void BulletMove(int speed)
{
	for (int i = 0; i < PLAYER_BULLET; i++)
	{
		if (player_bullet[i].flag)
		{
			player_bullet[i].y -= speed;
			if (player_bullet[i].y < 0)
			{
				player_bullet[i].flag = false;//这里是将15颗子弹同一时间全部发射出去了
				//于是要初始化时间变量
			}
		}
	}
}

//生成敌机
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

//敌机移动
void EnemyMove(int speed)
{
	for (int i = 0; i < ENEMYPLANE_NUM; i++)
	{
		if (enemy[i].flag)
		{
			enemy[i].y += speed;
			//超出边界处理
			if (enemy[i].y > WIN_HEIGHT)
			{
				enemy[i].flag = false;
			}
		}
	}
}

//打飞机
void PlayPlane()
{
	//每个敌军和每个子弹都要比较
	for (int i = 0; i < ENEMYPLANE_NUM; i++)
	{
		if (!enemy[i].flag)
			continue;
		//遍历子弹数组
		for (int k = 0; k < PLAYER_BULLET; k++)
		{
			if (!player_bullet[k].flag)
				continue;
			//判断子弹是否击中敌机
			if (player_bullet[k].x > enemy[i].x && player_bullet[k].x <enemy[i].x + 
				enemy[i].width&& player_bullet[k].y>enemy[i].y && player_bullet[k].y 
				< enemy[i].y + enemy[i].height)
			{
				//击中了敌机要消失，子弹也要消失
				enemy[i].flag = false;
				player_bullet[i].flag = false;
				//分数+=10
			}
		}
	}
}


int main()
{
	GameInit();
	////2.2输出图片
	//putimage(0, 0, &background);//第一个参数是原点
	////putimage(0, 0, &player[1]);
	////输出之后会发现玩家飞机的背景是存在的，所以需要将玩家飞机的背景改成透明的

	////这里是进行飞机背景的改变
	//   //putimage(0, 0, &img_player[0],NOTSRCERASE);
	//	  //先将源矩形图象与目标矩形图象进行布尔"或"运算，然后再将得图象进行反相.
	//   //putimage(0, 0, &img_player[1],SRCERASE;
	//	 //将目标矩形图象进行反相，再与源矩形图象进行布尔"与"运算


	////之后要改变图片的位置，就要改变坐标的位置
	////这时体现的是结构体的作用

	//putimage(player.x, player.y, &img_player[0], NOTSRCERASE);
	//putimage(player.x, player.y, &img_player[1], SRCERASE);

	//KeyControl(1);
	////这样子是不会移动的，需要一个循环

	while (1)//将其弄成死循环
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


	getchar();//防止闪退
	return 0;
}