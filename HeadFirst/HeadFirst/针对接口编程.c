

#include<stdio.h>
#include<string.h>

/*
 *  针对接口编程，不要针对实现编程
 */

//接口定义
struct fly_interface
{
	void(*flying)();
};

struct quack_interface
{
	void(*quacking)();
};

/* 
 * 接口的实现的函数，由函数指针指向，其实就相当于class里面的方法，
 * 只是由于结构体原因放在外面了,java在实现是只需要new就可以了，这里实现时，
 * 还需要绑定函数指针，多了这么一步
*/
void mallard_duck_flying(void)
{
	printf("i'm a mallard duck, i can fly !\n");
}

void wooden_duck_flying(void)
{
	printf("i'm a wooden duck, i can't fly !\n");
}

void mallard_duck_quacking(void)
{
	printf("i'm a mallard duck, i can quack:ga ga ga ~~~~\n");
}

void wooden_duck_quacking(void)
{
	printf("i'm a wooden duck, i cant quack: wu wu wu ....\n");
}

//接口开始实现
struct fly_interface fly_mallard_imp =  //相当于new，但是还需要绑定方法
{
	&mallard_duck_flying               //由于结构体里面不能有函数，只有使用函数指针
};
struct fly_interface fly_wooden_imp =
{
	&wooden_duck_flying
};
struct quack_interface quack_mallard_imp =
{
	&mallard_duck_quacking
};
struct quack_interface quack_wooden_imp =
{
	&wooden_duck_quacking
};


/*
 * 超类定义
 *
 * 构造方法，结构体创建对象时，没有构造方法，需要在构造方法里面完成的就放到init函数里面
 * 其实构造方法就是init函数
 *
 */
struct duck_t
{
	struct fly_interface     * fly_interimp;
	struct quack_interface   * quack_interimp;

	void(*performfly)(struct duck_t *);  /**< 接口编程方法 ,类的方法总是能够看见自己的属性和方法的*/
	void(*performquack)(struct duck_t *);/**< 接口编程方法 */
	void(*display)(void);                /**< 抽象方法 */
};

//超类的方法
void performfly(struct duck_t * duck)
{
	duck->fly_interimp->flying();
}

void performquack(struct duck_t * duck)
{
	duck->quack_interimp->quacking();
}

void display(void)
{
	printf("beautiful !!!\n");
}


//创建子类
struct duck_sub_t
{
	struct duck_t *duck;              /**< 继承超类 */
	void(*init)(struct duck_sub_t *); /**< 子类构造方法，C编译器不会编译成创建就调用，需显示调用 */
	void(*sub_function)();
};

void sub_function(void)
{
	printf("it is subclass funion!");
}

//子类的构造方法, 在构造方法里实现接口的动态加载 
void init_mallard(struct duck_sub_t * self)
{
	self->duck->fly_interimp   = &fly_mallard_imp;     //new 接口类
	self->duck->quack_interimp = &quack_mallard_imp;   //new 接口类

	/* 超类的方法都是一样的，结构体兑现创建后需要帮定下函数指针，交由构造函数处理*/
	self->duck->performfly   = performfly;       
	self->duck->performquack = performquack;
}
 
void init_wooden(struct duck_sub_t * self)
{
	self->duck->fly_interimp = &fly_wooden_imp;
	self->duck->quack_interimp = &quack_wooden_imp;

	/* 超类的方法都是一样的，结构体对象创建后需要帮定下函数指针，交由构造函数处理*/
	self->duck->performfly = performfly;       
	self->duck->performquack = performquack;
}

//new duck子类
struct duck_t mallard_duck_father;  //申请内存，new其实就是申请内存
struct duck_sub_t mallard_duck =
{
	&mallard_duck_father,
	&init_mallard,
	&sub_function
};

struct duck_t wooden_duck_father;  //申请内存，new其实就是申请内存
struct duck_sub_t wooden_duck =
{
	&wooden_duck_father,
	&init_wooden,
	&sub_function
};


//鸭圈
struct duck_sub_t *ducks[] =
{
	&mallard_duck,
	&wooden_duck
};


main()
{
	int i = 0;

	wooden_duck.init(&wooden_duck);           /**< 调用构造方法 ,构造方法总是能看见自己的属性的，所以把自己传进去*/
	mallard_duck.init(&mallard_duck);     /**< 调用构造方法 */

	mallard_duck.duck->performfly(mallard_duck.duck);
	mallard_duck.duck->performquack(mallard_duck.duck);

	wooden_duck.duck->performfly(wooden_duck.duck);
	wooden_duck.duck->performquack(wooden_duck.duck);

	/*----------------------------------------------------------------------*/
	printf("-------------------------------------------------------------\n");
	/* 不加区分的处理每一个对象，针对接口，不针对具体*/
	
	for (i = 0; i < sizeof(ducks) / sizeof(ducks[0]); i++)
	{
		ducks[i]->init(ducks[i]);
		ducks[i]->duck->performfly  (ducks[i]->duck);
		ducks[i]->duck->performquack(ducks[i]->duck);
	}

	getchar();
}