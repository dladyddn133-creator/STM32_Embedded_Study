// 여기에 사용자 임의의 define을 작성하시오

#define GPIOA_MODER		(*(unsigned int*)0x40020000)
#define GPIOA_OTYPER	(*(unsigned int*)0x40020004)
#define GPIOA_ODR		(*(unsigned int*)0x40020014)

void Main(void)
{
	// LED GPA[5]를 출력(General Push Pull) 모드로 설정하시오

	GPIOA_MODER |= (0x1 << 10);
	GPIOA_OTYPER |= (0x0 << 5);

	// GPA[5] LED를 ON 시키도록 설정하시오

	volatile int  i;

	for(;;)
	{
		GPIOA_ODR |= (0x1 << 5);
		for(i=0;i<0x40000;i++);

		GPIOA_ODR &= (0x0 << 5);
		for(i=0;i<0x40000;i++);
	}
}
