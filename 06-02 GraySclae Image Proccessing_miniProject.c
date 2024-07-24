#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>

#define MSIZE 3

// 전역변수 선언
HWND hwnd; //윈도우 화면(외부)
HDC hdc; // 윈도우 화면 (내부)

int oriH, oriW, tarH=0, tarW=0;
unsigned char** oriImage = NULL;
unsigned char** tarImage = NULL;
char fileName[200];

// 함수 선언부
// 공통 함수
void loadImage(); void saveImage(); void displayOriImage(); void displayTarImage(); void printMenu(); void callocOriImage(); void callocTarImage(); void freeOriImage(); void freeTarImage();
double** callocDoubleArr(int h, int w); void freeDoubleArr(double** memory, int h);

// 메뉴 함수
void bwImageMenu(); void bitImageMenu(); void parabolaImageMenu();
void zoomOutMenu(); void zoomInMenu(); void mirroringMenu();
void sharpeMenu();

// 영상처리 함수
// 화소 점 처리 함수
void equalImage(); void addImage(); void reverseImage(); void bwImage1(); void bwImage2(); void bwImage3();
void andImage(); void orImage(); void notImage(); void xorImage(); void posterizingImage(); void gammaImage();
void rangeImage(); void parabolaImageCup(); void parabolaImageCap();

// 기하학적 처리 함수
void zoomOut(); void zoomOut2(); void zoomOut3(); void zoomOut4(); void zoomIn(); void zoomIn2(); void zoomIn3(); void moveImage(); void rotate3(); void mirroring(); void mirroring2();

// 히스토그램 처리 함수
void histoStretch(); void endIn(); void histoEqual();

// 화소 영역 처리 함수
void emboss(); void blurr(); void sharpe1(); void sharpe2(); void sharpe3();

// 경계선 검출 함수
void edge1(); void edge2(); void edge3(); void edge4();



void printMenu()
{
	system("cls");
	printf("\n GrayScale Image Proccessing (GA2)");
	printf("\n 1.열기 2.저장 9.종료 ");

	printf("\n\n 화소 점 처리");
	printf("\n A.원본불러오기 B.밝기조절 C.반전 D.이진화 E.감마 F.논리연산 G.포스터라이징 H.범위강조 I.파라볼라(cap,cup)");

	printf("\n\n 기하학적 처리");
	printf("\n J.축소 K.확대 L.이동 M.회전 N.대칭");

	printf("\n\n 히스토그램 처리");
	printf("\n O.스트레치 P.엔드-인 Q.평활화");

	printf("\n\n 화소영역 처리");
	printf("\n R.엠보싱 S.블러링 T.샤프닝");

	printf("\n\n 경계선 검출");
	printf("\n U.수평에지 V.수직에지 W.유사 연산자 X.차 연산자\n");
	displayOriImage();
	displayTarImage();
}

int main(void)
{
	hwnd = GetForegroundWindow();
	hdc = GetWindowDC(hwnd); //윈도우 10용. 윈도우 11 --> (hwnd --> NULL);

	char key = 0;
	while (key != '9')
	{
		printMenu();
		key = _getch();
		system("cls");
		switch (key)
		{
		case '1':			loadImage(); break;
		case '2':			saveImage(); break;
		case '9':			freeOriImage(); freeTarImage(); break;
		case 'a': case 'A':	equalImage(); break;
		case 'b': case 'B':	addImage();	break;
		case 'c': case 'C':	reverseImage(); break;
		case 'd': case 'D':	bwImageMenu(); break;
		case 'e': case 'E':	gammaImage(); break;
		case 'f': case 'F': bitImageMenu();	break;
		case 'g': case 'G':	posterizingImage(); break;
		case 'h': case 'H':	rangeImage(); break;
		case 'i': case 'I':	parabolaImageMenu(); break;
		case 'j': case 'J': zoomOutMenu(); break;
		case 'k': case 'K':	zoomInMenu(); break;
		case 'l': case 'L':	moveImage(); break;
		case 'm': case 'M':	rotate3(); break;
		case 'n': case 'N':	mirroringMenu(); break;

		//case 'o': case 'O':	워핑(); break;
		case 'o': case 'O':histoStretch(); break;
		case 'p': case 'P':	endIn(); break;
		case 'q': case 'Q':	histoEqual(); break;

		case 'r': case 'R': emboss(); break;
		case 's': case 'S':	blurr(); break;
		case 't': case 'T':	sharpeMenu(); break;
		case 'u': case 'U':	edge1(); break;
		case 'v': case 'V':	edge2(); break;
		case 'w': case 'W':	edge3(); break;
		case 'x': case 'X':	edge4(); break;
		default:			break;
		}
	}
	puts("도비는 공짜입니다.\n");

	return 0;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////
// 공통 함수

void callocOriImage()
{
	if (oriImage == NULL)
	{
		oriImage = (unsigned char**)calloc(oriH, sizeof(unsigned char*));
		for (int i = 0; i < oriH; i++)
		{
			oriImage[i] = (unsigned char*)calloc(oriW, sizeof(unsigned char));
		}
	}
}

void callocTarImage()
{
	if (tarImage == NULL)
	{
		tarImage = (unsigned char**)calloc(tarH, sizeof(unsigned char*));
		for (int i = 0; i < tarH; i++)
		{
			tarImage[i] = (unsigned char*)calloc(tarW, sizeof(unsigned char));
		}
	}
}

void freeOriImage()
{
	if (oriImage != NULL)
	{
		for (int i = 0; i < oriH; i++)
		{
			free(oriImage[i]);
		}
		free(oriImage);
		oriImage = NULL;
	}
}

void freeTarImage()
{
	if (tarImage != NULL)
	{
		for (int i = 0; i < tarH; i++)
		{
			free(tarImage[i]);
		}
		free(tarImage);
		tarImage = NULL;
	}
}

void loadImage()
{
	char fullname[200] = "C:/RAW/Etc_Raw(squre)/";
	char tempName[50];
	printf("파일명: "); scanf("%s", tempName); // cat01, dog05, etc11
	strcat(fullname, tempName);
	strcat(fullname, ".raw");
	strcpy(fileName, fullname);

	// 원본영상의 크기를 파악하는게 중요함
	FILE* rfp = NULL;
	rfp = fopen(fileName, "rb");
	if (rfp == NULL)
	{
		printf("파일명 틀렸음\n");
		return;
	}
	fseek(rfp, 0L, SEEK_END); //파일의 끝으로 이동
	long long fsize = ftell(rfp);
	fclose(rfp);
	
	freeOriImage();
	oriH = oriW = sqrt(fsize);

	//2차원 메모리 할당
	callocOriImage();

	//파일에서 메모리로
	rfp = fopen(fileName, "rb");
	for (int i = 0; i < oriH; i++)
	{
		fread(oriImage[i], sizeof(unsigned char), oriW, rfp);
	}
	fclose(rfp);
	equalImage();
}

void saveImage()
{
	char fullname[200] = "C:/RAW/Pet_RAW(squre)/Pet_RAW(512x512)/";
	char tempName[50];
	printf("저장 파일명: "); scanf("%s", tempName); // out01, out02
	strcat(fullname, tempName);
	strcat(fullname, "_512.raw");
	strcpy(fileName, fullname);

	FILE* wfp;
	wfp = fopen(fileName, "wb");
	for (int i = 0; i < tarH; i++)
	{
		fwrite(tarImage[i], sizeof(unsigned char), tarW, wfp);
	}
	fclose(wfp);
	printf("%s로 저장됨.", fileName);
}

void displayOriImage()
{
	for (int i = 0; i < oriH; i++)
	{
		for (int j = 0; j < oriW; j++)
		{
			int px = oriImage[i][j];
			SetPixel(hdc, j + 600, i + 450, RGB(px, px, px));
		}
	}
}

void displayTarImage()
{
	for (int i = 0; i < tarH; i++)
	{
		for (int j = 0; j < tarW; j++)
		{
			int px = tarImage[i][j];
			SetPixel(hdc, j + 600+oriW+50, i + 450, RGB(px, px, px));
		}
	}
}

double** callocDoubleArr(int h, int w)
{
	double** memory = (double**)calloc(h, sizeof(double*));
	for (int i = 0; i < h; i++)
		memory[i] = (double*)calloc(h, sizeof(double));
	return memory;
}

void freeDoubleArr(double** memory, int h)
{
	for (int i = 0; i < h; i++)
		free(memory[i]);
	free(memory);
	memory = NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 메뉴 함수

void bwImageMenu()
{
	printf("이진화 기준 선택\n");
	printf("1.128	2.이미지의 평균값	3.이미지의 중위값\n");
	int menu = 0;
	scanf("%d", &menu);
	switch (menu)
	{
	case 1:
		bwImage1();
		break;
	case 2:
		bwImage2();
		break;
	case 3:
		bwImage3();
		break;
	default:
		printf("잘못된 입력입니다.\n");
		break;
	}
}

void bitImageMenu()
{
	printf("비트연산 선택(숫자입력)\n");
	printf("1.and  2.or  3.not  4.xor\n");
	int menu = 0;
	scanf("%d", &menu);
	switch (menu)
	{
	case 1:
		andImage();
		break;
	case 2:
		orImage();
		break;
	case 3:
		notImage();
		break;
	case 4:
		xorImage();
		break;
	default:
		printf("잘못된 입력입니다.\n");
		break;
	}
}

void parabolaImageMenu()
{
	printf("파라볼라 처리 선택\n");
	printf("1.cap	2.cup\n");
	int menu = 0;
	scanf("%d", &menu);
	switch (menu)
	{
	case 1:
		parabolaImageCap();
		break;
	case 2:
		parabolaImageCup();
		break;
	default:
		printf("잘못된 입력입니다.\n");
		break;
	}
}

void zoomOutMenu()
{
	printf("축소 방법 선택(숫자입력)\n");
	printf("1.백워딩  2.포워딩  3.평균값  4.중위값\n");
	int menu = 0;
	scanf("%d", &menu);
	switch (menu)
	{
	case 1:
		zoomOut();
		break;
	case 2:
		zoomOut2();
		break;
	case 3:
		zoomOut3();
		break;
	case 4:
		zoomOut4();
		break;
	default:
		printf("잘못된 입력입니다.\n");
		break;
	}
}

void zoomInMenu()
{
	printf("축소 방법 선택(숫자입력)\n");
	printf("1.포워딩  2.백워딩  3.양선형보간\n");
	int menu = 0;
	scanf("%d", &menu);
	switch (menu)
	{
	case 1:
		zoomIn();
		break;
	case 2:
		zoomIn2();
		break;
	case 3:
		//printf("미구현\n");
		zoomIn3();
		break;
	default:
		printf("잘못된 입력입니다.\n");
		break;
	}
}

void mirroringMenu()
{
	printf("대칭 처리 선택\n");
	printf("1.좌우대칭	2.상하대칭\n");
	int menu = 0;
	scanf("%d", &menu);
	switch (menu)
	{
	case 1:
		mirroring();
		break;
	case 2:
		mirroring2();
		break;
	default:
		printf("잘못된 입력입니다.\n");
		break;
	}
}

void sharpeMenu()
{
	printf("샤프닝 마스크 선택(숫자입력)\n");
	printf("1.마스크1  2.마스크2  3.고주파\n");
	int menu = 0;
	scanf("%d", &menu);
	switch (menu)
	{
	case 1:
		sharpe1();
		break;
	case 2:
		sharpe2();
		break;
	case 3:
		sharpe3();
		break;
	default:
		printf("잘못된 입력입니다.\n");
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 화소 점 처리 함수

void equalImage()
{
	freeTarImage();
	// (중요!) 결과 영상의 크기를 결정해야함 --> 알고리즘에 따라 크기가 달라짐
	tarH = oriH;
	tarW = oriW;
	// 결과 영상 메모리 할당
	callocTarImage();

	// 진짜 영상처리 알고리즘
	for (int i = 0; i < oriH; i++)
	{
		for (int j = 0; j < oriW; j++)
		{
			tarImage[i][j] = oriImage[i][j];
		}
	}
}

void addImage()
{
	int value;
	printf("밝게할 값: "); scanf("%d", &value);
	for (int i = 0; i < tarH; i++) {
		for (int j = 0; j < tarW; j++) {
			if (oriImage[i][j] + value > 255)
				tarImage[i][j] = 255;
			else if (oriImage[i][j] + value < 0)
				tarImage[i][j] = 0;
			else
				tarImage[i][j] = oriImage[i][j] + value;
		}
	}
}

void reverseImage()
{
	for (int i = 0; i < oriH; i++) {
		for (int j = 0; j < oriW; j++) {
			tarImage[i][j] = 255 - oriImage[i][j];
		}
	}
}

void bwImage1()
{
	for (int i = 0; i < tarH; i++) {
		for (int j = 0; j < tarW; j++) {
			if (oriImage[i][j] > 127)
				tarImage[i][j] = 255;
			else
				tarImage[i][j] = 0;
		}
	}
}

int get_avg()
{
	long long sum = 0;
	for (int i = 0; i < tarH; i++)
	{
		for (int j = 0; j < tarW; j++)
		{
			sum += oriImage[i][j];
		}
	}
	sum /= tarH * tarW;
	return sum;
}

void bwImage2()
{
	int avg = get_avg();
	for (int i = 0; i < tarH; i++) {
		for (int j = 0; j < tarW; j++) {
			if (oriImage[i][j] > avg)
				tarImage[i][j] = 255;
			else
				tarImage[i][j] = 0;
		}
	}
}

int compare(const void* a, const void* b)    // 오름차순 비교 함수 구현
{
	unsigned char num1 = *(unsigned char*)a;    // void 포인터를 int 포인터로 변환한 뒤 역참조하여 값을 가져옴
	unsigned char num2 = *(unsigned char*)b;    // void 포인터를 int 포인터로 변환한 뒤 역참조하여 값을 가져옴

	if (num1 < num2)    // a가 b보다 작을 때는
		return -1;      // -1 반환

	if (num1 > num2)    // a가 b보다 클 때는
		return 1;       // 1 반환

	return 0;    // a와 b가 같을 때는 0 반환
}

int get_median()
{
	unsigned char* tempArr = (unsigned char*)calloc(oriH * oriW, sizeof(unsigned char));
	for (int i = 0; i < oriH; i++)
	{
		for (int j = 0; j < oriW; j++)
		{
			tempArr[i * oriW + j] = oriImage[i][j];
		}
	}
	qsort(tempArr, sizeof(oriImage) / sizeof(oriImage[0]), sizeof(oriImage[0]), compare);
	int median;
	int t = (oriH * oriW) / 2;
	if ((oriH * oriW) % 2 == 0)
	{
		median = (tempArr[t] + tempArr[t + 1]) / 2;
	}
	else
	{
		median = tempArr[t + 1];
	}
	free(tempArr);
	return median;
}

void bwImage3()
{
	int median = get_median();
	for (int i = 0; i < tarH; i++) {
		for (int j = 0; j < tarW; j++) {
			if (oriImage[i][j] > median)
				tarImage[i][j] = 255;
			else
				tarImage[i][j] = 0;
		}
	}
}

void andImage()
{
	for (int i = 0; i < tarH; i++) {
		for (int j = 0; j < tarW; j++) {
			//연산하는 사진 대신에 조건문으로 처리
			if (i > tarH/4.0 && i<3.0*tarH/4 && j>tarW/4.0 && j<3.0*tarW/4)
				tarImage[i][j] = oriImage[i][j] & 255;
			else
				tarImage[i][j] = oriImage[i][j] & 0;
		}
	}
}

void orImage()
{
	for (int i = 0; i < tarH; i++) {
		for (int j = 0; j < tarW; j++) {
			//연산하는 사진 대신에 조건문으로 처리
			if (i>tarH/4.0&&i<3.0*tarH/4&&j>tarW/4.0&&j<3.0*tarW/4)
				tarImage[i][j] = oriImage[i][j] | 255;
			else
				tarImage[i][j] = oriImage[i][j] | 0;
		}
	}
}

void notImage()
{
	for (int i = 0; i < tarH; i++) {
		for (int j = 0; j < tarW; j++) {
				tarImage[i][j] = ~oriImage[i][j];
		}
	}
}

void xorImage()
{
	for (int i = 0; i < tarH; i++) {
		for (int j = 0; j < tarW; j++) {
			//연산하는 사진 대신에 조건문으로 처리
			if (i>tarH/4.0&&i<3.0*tarH/4&&j>tarW/4.0&&j<3.0*tarW/4)
				tarImage[i][j] = oriImage[i][j] ^ 255;
			else
				tarImage[i][j] = oriImage[i][j] ^ 0;
		}
	}
}

void posterizingImage()
{
	int sector;
	printf("몇 구간으로 나누겠습니까?");
	scanf("%d", &sector);

	int value = 0, sector_size = 0;
	value = 255 / (sector - 1); // 값
	sector_size = 255 / sector; // 구간별 범위
	for (int i = 0; i < tarH; i++) {
		for (int j = 0; j < tarW; j++) {
			tarImage[i][j] = (value * (oriImage[i][j] / sector_size));
		}
	}
}

void gammaImage()
{
	double gamma;
	printf("감마 값을 적어주세요: ");
	scanf("%lf", &gamma);
	for (int i = 0; i < tarH; i++) {
		for (int j = 0; j < tarW; j++) {
			if (pow(oriImage[i][j], 1.0 / gamma) > 255)
				tarImage[i][j] = 255;
			else if (pow(oriImage[i][j], 1.0 / gamma) < 0)
				tarImage[i][j] = 0;
			else
				tarImage[i][j] = pow(oriImage[i][j], 1.0 / gamma);
		}
	}
}

void rangeImage()
{
	int range1, range2;
	printf("입력예시: 50 150\n");
	printf("강조할 범위(0~255)의 시작과 끝을 적어주세요: ");
	scanf("%d %d", &range1, &range2);
	for (int i = 0; i < tarH; i++)
	{
		for (int j = 0; j < tarW; j++)
		{
			if (oriImage[i][j] >= range1 && oriImage[i][j] <= range2)
				tarImage[i][j] = 255;
		}
	}
}

void parabolaImageCup()
{
	for (int i = 0; i < tarH; i++)
	{
		for (int j = 0; j < tarW; j++)
		{
			if (255 * pow(((double)oriImage[i][j] / 127 - 1), 2) > 255)
				tarImage[i][j] = 255;
			else if (255 * pow(((double)oriImage[i][j] / 127 - 1), 2) < 0)
				tarImage[i][j] = 0;
			else
				tarImage[i][j] = 255 * pow(((double)oriImage[i][j] / 127 - 1), 2);
		}
	}
}

void parabolaImageCap()
{
	for (int i = 0; i < tarH; i++)
	{
		for (int j = 0; j < tarW; j++)
		{
			if (255 - 255 * pow(((double)oriImage[i][j] / 127 - 1), 2) > 255)
				tarImage[i][j] = 255;
			else if (255 - 255 * pow(((double)oriImage[i][j] / 127 - 1), 2) < 0)
				tarImage[i][j] = 0;
			else
				tarImage[i][j] = 255 - 255 * pow(((double)oriImage[i][j] / 127 - 1), 2);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// 기하학적 처리 함수

void zoomIn()
{
	freeTarImage();
	int scale;
	printf("몇 배 확대하시겠습니까? ");
	scanf("%d", &scale);
	tarH = (int)(oriH * scale);
	tarW = (int)(oriW * scale);
	callocTarImage();

	for (int i = 0; i < oriH; i++) {
		for (int j = 0; j < oriW; j++) {
			for (int k = 0; k < scale; k++) {
				tarImage[i * scale + k][j * scale + k] = oriImage[i][j];
			}
		}
	}
}

void zoomIn2()
{
	freeTarImage();
	int scale;
	printf("몇 배 확대하시겠습니까? ");
	scanf("%d", &scale);
	tarH = (int)(oriH * scale);
	tarW = (int)(oriW * scale);
	callocTarImage();

	for (int i = 0; i < tarH; i++) {
		for (int j = 0; j < tarW; j++) {
			tarImage[i][j] = oriImage[i / scale][j / scale];
		}
	}
}

void zoomIn3()
{
	freeTarImage();
	int scale;
	printf("몇 배 확대하시겠습니까? ");
	scanf("%d", &scale);
	tarH = (int)(oriH * scale);
	tarW = (int)(oriW * scale);
	callocTarImage();

	for (int i = 0; i < tarH; i++)
	{
		for (int j = 0; j < tarW; j++)
		{
			tarImage[i][j] = oriImage[i / scale][j / scale];
		}
	}
	int e=0, f=0, n=0, x=0, y=0;

	for (int i = 0; i < tarH - scale; i += scale)
	{
		for (int j = 0; j < tarW - scale; j += scale)
		{
			for (int k = 0; k <= scale; k++)
			{
				for (int l = 0; l <= scale; l++)
				{
					y = i + k;
					x = j + l;
					if (1)
					{
						e = tarImage[i][j] + x * (tarImage[i][j + scale] - tarImage[i][j]);
						f = tarImage[i + scale][j] + x * (tarImage[i + scale][j + scale] - tarImage[i + scale][j]);
						n = e + y * (f - e);
						tarImage[y][x] = n;
					}
				}
			}
		}
	}
}

void zoomOut()
{
	freeTarImage();
	int scale;
	printf("몇 배 축소하시겠습니까? ");
	scanf("%d", &scale);
	tarH = (int)(oriH / scale);
	tarW = (int)(oriW / scale);
	callocTarImage();

	for (int i = 0; i < oriH; i++) {
		for (int j = 0; j < oriW; j++) {
			tarImage[i / scale][j / scale] = oriImage[i][j];
		}
	}
}

void zoomOut2()
{
	freeTarImage();
	int scale;
	printf("몇 배 축소하시겠습니까? ");
	scanf("%d", &scale);
	tarH = (int)(oriH / scale);
	tarW = (int)(oriW / scale);
	callocTarImage();

	for (int i = 0; i < tarH; i++) {
		for (int j = 0; j < tarW; j++) {
			tarImage[i][j] = oriImage[i * scale][j * scale];
		}
	}
}

void zoomOut3()
{
	freeTarImage();
	int scale;
	printf("몇 배 축소하시겠습니까? ");
	scanf("%d", &scale);
	tarH = (int)(oriH / scale);
	tarW = (int)(oriW / scale);
	callocTarImage();
	int sum;
	for (int i = 0; i < oriH; i += scale) {
		for (int j = 0; j < oriW; j += scale) {
			sum = 0;
			for (int k = 0; k < scale; k++) {
				for (int l = 0; l < scale; l++) {
					sum += oriImage[i + k][j + l];
				}
			}
			tarImage[i / scale][j / scale] = sum / (scale * scale);
		}
	}
}

void zoomOut4()
{
	freeTarImage();
	int scale;
	printf("몇 배 축소하시겠습니까? ");
	scanf("%d", &scale);
	tarH = (int)(oriH / scale);
	tarW = (int)(oriW / scale);
	callocTarImage();
	int median;
	for (int i = 0; i < oriH; i += scale) {
		for (int j = 0; j < oriW; j += scale) {
			// 범위 내의 중위값을 구하기 위한 임시 배열
			unsigned char* temp = (unsigned char*)calloc(scale * scale, sizeof(unsigned char));
			int t = (scale * scale) / 2;
			for (int k = 0; k < scale; k++) {
				for (int l = 0; l < scale; l++) {
					temp[k * scale + l] = oriImage[i + k][j + l];
				}
			}
			qsort(temp, scale * scale, sizeof(temp[0]), compare);
			if ((scale * scale) % 2 == 0)
				median = (temp[t] + temp[t + 1]) / 2;
			else
				median = temp[t + 1];
			free(temp);
			tarImage[i / scale][j / scale] = median;
		}
	}
}

void moveImage()
{
	freeTarImage();
	callocTarImage();
	printf("입력예시: 20 -30\n");
	printf("이동할 값: ");
	int x, y;
	scanf("%d %d", &x, &y);
	for (int i = 0; i < tarH; i++) {
		for (int j = 0; j < tarW; j++) {
			int h, w;
			h = i + y;
			w = j + x;
			if (i - y >= 0 && j - x >= 0 && i - y < oriH && j - x < oriW)
				tarImage[i][j] = oriImage[i - y][j - x];
		}
	}
}

void rotate3()
{
	int angle;
	printf("%d %d\n", tarH, tarW);
	printf("회전할 각도 입력: ");
	scanf("%d", &angle);
	// 회전 수식
	// oriX = cos * (tarX-Cx) + sin * (tarY-Cy) + Cx
	// tarY = sin * (tarX-Cx) + cos * (tarY-Cy) + Cy
	int tarX, tarY, oriX, oriY;
	double radian = angle * 3.141592 / 180.0;
	double temp = (90 - angle) * 3.141592 / 180.0;

	freeTarImage();
	// (중요!) 결과 영상의 크기를 결정 ---> 알고리즘에 의존.
	tarH = abs(oriH * cos(radian)) + abs(oriW * cos(temp));
	tarW = abs(oriH * cos(temp)) + abs(oriW * cos(radian));
	printf("%d %d\n", tarH, tarW);
	// 결과 영상 메모리 할당
	// tarImage[tarH][tarW] 크기로 할당
	callocTarImage();
	int tarCx = tarH / 2;
	int tarCy = tarW / 2;
	int oriCx = oriH / 2;
	int oriCy = oriW / 2;
	// *** 진짜 영상처리 알고리즘 ****
	for (int i = 0; i < tarH; i++) {
		for (int k = 0; k < tarW; k++) {
			tarX = i;
			tarY = k;
			oriX = cos(radian) * (tarX - tarCx) + sin(radian) * (tarY - tarCy) + oriCx;
			oriY = -sin(radian) * (tarX - tarCx) + cos(radian) * (tarY - tarCy) + oriCy;

			if (oriX >= 0 && oriX < oriH && oriY >= 0 && oriY < oriW)
				tarImage[tarX][tarY] = oriImage[oriX][oriY];
		}
	}
}

void mirroring()
{
	freeTarImage();
	tarH = oriH;
	tarW = oriW;
	callocTarImage();
	for (int i = 0; i < tarH; i++) {
		for (int j = 0; j < tarW; j++) {
			tarImage[i][j] = oriImage[i][oriW - 1 - j];
		}
	}
}

void mirroring2()
{
	freeTarImage();
	tarH = oriH;
	tarW = oriW;
	callocTarImage();
	for (int i = 0; i < tarH; i++) {
		for (int j = 0; j < tarW; j++) {
			tarImage[i][j] = oriImage[oriH - 1 - i][j];
		}
	}
}


/////////////////////////////////////////////////////////////////////////////////////////
// 히스토그램 함수

void histoStretch() {
	// (중요!) 결과 영상의 크기를 결정 ---> 알고리즘에 의존.
	freeTarImage();
	tarH = oriH;
	tarW = oriW;
	// 결과 영상 메모리 할당
	// tarImage[tarH][tarW] 크기로 할당
	callocTarImage();

	// 수식
	// new = (old - low)/(high-low)*255.0;

	int low, high;
	low = high = oriImage[0][0];
	for (int i = 1; i < oriH; i++) {
		for (int k = 1; k < oriW; k++) {
			if (low > oriImage[i][k])
				low = oriImage[i][k];
			else if (high < oriImage[i][k])
				high = oriImage[i][k];
		}
	}

	// *** 진짜 영상처리 알고리즘 ****
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			tarImage[i][k] = (double)(oriImage[i][k] - low) / (high - low) * 255.0;
		}
	}
}

void endIn() {
	// (중요!) 결과 영상의 크기를 결정 ---> 알고리즘에 의존.
	freeTarImage();
	tarH = oriH;
	tarW = oriW;
	// 결과 영상 메모리 할당
	// tarImage[tarH][tarW] 크기로 할당
	callocTarImage();

	// 수식
	// new = (old - low)/(high-low)*255.0;

	int low, high;
	low = high = oriImage[0][0];
	for (int i = 1; i < oriH; i++) {
		for (int k = 1; k < oriW; k++) {
			if (low > oriImage[i][k])
				low = oriImage[i][k];
			else if (high < oriImage[i][k])
				high = oriImage[i][k];
		}
	}
	low += 50;
	high -= 50;
	// *** 진짜 영상처리 알고리즘 ****
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			double newVal = (double)(oriImage[i][k] - low) / (high - low) * 255.0;
			if (newVal < 0.0)
				newVal = 0;
			if (newVal > 255.0)
				newVal = 255;
			tarImage[i][k] = newVal;

		}
	}
}

void histoEqual()
{
	// (중요!) 결과 영상의 크기를 결정 ---> 알고리즘에 의존.
	freeTarImage();
	tarH = oriH;
	tarW = oriW;
	// 결과 영상 메모리 할당
	// tarImage[tarH][tarW] 크기로 할당
	callocTarImage();

	int hist[256] = { 0 };
	int sum[256] = { 0 };
	int n[256] = { 0 };

	//1단계
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			hist[oriImage[i][k]] += 1;
		}
	}
	for (int i = 0; i < 256; i++) {
		//2단계
		sum[i] = hist[i];
		if (i != 0)
			sum[i] += sum[i - 1];

		//3단계
		n[i] = (double)sum[i] * (1.0 / (double)(oriH * oriW)) * 255.0;
	}

	for (int i = 0; i < tarH; i++)
	{
		for (int j = 0; j < tarW; j++)
		{
			tarImage[i][j] = n[oriImage[i][j]];
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 화소 영역 처리 함수

void emboss() {
	// (중요!) 결과 영상의 크기를 결정 ---> 알고리즘에 의존.
	freeTarImage();
	tarH = oriH;
	tarW = oriW;
	// 결과 영상 메모리 할당
	// tarImage[tarH][tarW] 크기로 할당
	callocTarImage();

	// *** 진짜 영상처리 알고리즘 ****
	//const int MSIZE = 3; 마스크 크기 (위에 정의해둠)
	double mask[MSIZE][MSIZE] = {
		{ 1.0, 0.0, 0.0 },
		{ 0.0, 0.0, 0.0 },
		{ 0.0, 0.0, -1.0} };
	//임시 입출력 메모리 준비
	double** tempOri, ** tempTar;
	tempOri = callocDoubleArr(oriH + 2, oriW + 2);
	tempTar = callocDoubleArr(tarH, tarW);
	for (int i = 0; i < oriH + 2; i++)
	{
		for (int j = 0; j < oriW + 2; j++)
		{
			tempOri[i][j] = 127.0;
		}
	}
	for (int i = 0; i < oriH; i++)
	{
		for (int j = 0; j < oriW; j++)
		{
			tempOri[i + 1][j + 1] = oriImage[i][j];
		}
	}
	double s = 0.0;
	for (int i = 0; i < oriH; i++) {
		for (int j = 0; j < oriW; j++) {
			s = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					s += mask[m][n] * tempOri[i + m][j + n];
			tempTar[i][j] = s;
		}
	}
	// post proccessing 후처리 마스크 합계가 0이면 127을 더함.
	for (int i = 0; i < tarH; i++)
	{
		for (int j = 0; j < tarW; j++)
		{
			tempTar[i][j] += 127.0;
		}
	}
	//임시 타겟 -> 타겟
	for (int i = 0; i < tarH; i++)
	{
		for (int j = 0; j < tarW; j++)
		{
			double v = tempTar[i][j];
			if (v > 255)
				v = 255;
			if (v < 0)
				v = 0;
			tarImage[i][j] = v;
		}
	}

	freeDoubleArr(tempOri, oriH + 2);
	freeDoubleArr(tempTar, tarH);
}

void blurr() {
	// (중요!) 결과 영상의 크기를 결정 ---> 알고리즘에 의존.
	freeTarImage();
	tarH = oriH;
	tarW = oriW;
	// 결과 영상 메모리 할당
	// tarImage[tarH][tarW] 크기로 할당
	callocTarImage();

	// *** 진짜 영상처리 알고리즘 ****
	//const int MSIZE = 3; 마스크 크기 (위에 정의해둠)
	double mask[MSIZE][MSIZE] = {
		{ 1.0 / 9, 1.0 / 9, 1.0 / 9 },
		{ 1.0 / 9, 1.0 / 9, 1.0 / 9 },
		{ 1.0 / 9, 1.0 / 9, 1.0 / 9 } };
	//임시 입출력 메모리 준비
	double** tempOri, ** tempTar;
	tempOri = callocDoubleArr(oriH + 2, oriW + 2);
	tempTar = callocDoubleArr(tarH, tarW);
	for (int i = 0; i < oriH + 2; i++)
	{
		for (int j = 0; j < oriW + 2; j++)
		{
			tempOri[i][j] = 127.0;
		}
	}
	for (int i = 0; i < oriH; i++)
	{
		for (int j = 0; j < oriW; j++)
		{
			tempOri[i + 1][j + 1] = oriImage[i][j];
		}
	}
	double s = 0.0;
	for (int i = 0; i < oriH; i++)
	{
		for (int j = 0; j < oriW; j++)
		{
			s = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					s += mask[m][n] * tempOri[i + m][j + n];
			tempTar[i][j] = s;
		}
	}

	//임시 타겟 -> 타겟
	for (int i = 0; i < tarH; i++)
	{
		for (int j = 0; j < tarW; j++)
		{
			double v = tempTar[i][j];
			if (v > 255)
				v = 255;
			if (v < 0)
				v = 0;
			tarImage[i][j] = v;
		}
	}

	freeDoubleArr(tempOri, oriH + 2);
	freeDoubleArr(tempTar, tarH);
}

void sharpe1()
{
	freeTarImage();
	tarH = oriH;
	tarW = oriW;
	// 결과 영상 메모리 할당
	// tarImage[tarH][tarW] 크기로 할당
	callocTarImage();

	// *** 진짜 영상처리 알고리즘 ****
	//const int MSIZE = 3; 마스크 크기 (위에 정의해둠)
	double mask[MSIZE][MSIZE] = {
		{ -1.0, -1.0, -1.0 },
		{ -1.0,  9.0, -1.0 },
		{ -1.0, -1.0, -1.0 } };

	//임시 입출력 메모리 준비
	double** tempOri, ** tempTar;
	tempOri = callocDoubleArr(oriH + 2, oriW + 2);
	tempTar = callocDoubleArr(tarH, tarW);
	for (int i = 0; i < oriH + 2; i++)
	{
		for (int j = 0; j < oriW + 2; j++)
		{
			tempOri[i][j] = 127.0;
		}
	}
	for (int i = 0; i < oriH; i++)
	{
		for (int j = 0; j < oriW; j++)
		{
			tempOri[i + 1][j + 1] = oriImage[i][j];
		}
	}
	double s = 0.0;
	for (int i = 0; i < oriH; i++)
	{
		for (int j = 0; j < oriW; j++)
		{
			s = 0.0;
			for (int m = 0; m < MSIZE; m++)
			{
				for (int n = 0; n < MSIZE; n++)
				{
					s += mask[m][n] * tempOri[i + m][j + n];
				}
			}
			tempTar[i][j] = s;
		}
	}

	//임시 타겟 -> 타겟
	for (int i = 0; i < tarH; i++)
	{
		for (int j = 0; j < tarW; j++)
		{
			double v = tempTar[i][j];
			if (v > 255)
				v = 255;
			if (v < 0)
				v = 0;
			tarImage[i][j] = v;
		}
	}

	freeDoubleArr(tempOri, oriH + 2);
	freeDoubleArr(tempTar, tarH);
}

void sharpe2()
{
	freeTarImage();
	tarH = oriH;
	tarW = oriW;
	// 결과 영상 메모리 할당
	// tarImage[tarH][tarW] 크기로 할당
	callocTarImage();

	// *** 진짜 영상처리 알고리즘 ****
	//const int MSIZE = 3; 마스크 크기 (위에 정의해둠)
	double mask[MSIZE][MSIZE] = {
		{  0.0, -1.0,  0.0 },
		{ -1.0,  5.0, -1.0  },
		{  0.0, -1.0,  0.0 } };

	//임시 입출력 메모리 준비
	double** tempOri, ** tempTar;
	tempOri = callocDoubleArr(oriH + 2, oriW + 2);
	tempTar = callocDoubleArr(tarH, tarW);
	for (int i = 0; i < oriH + 2; i++)
	{
		for (int j = 0; j < oriW + 2; j++)
		{
			tempOri[i][j] = 127.0;
		}
	}
	for (int i = 0; i < oriH; i++)
	{
		for (int j = 0; j < oriW; j++)
		{
			tempOri[i + 1][j + 1] = oriImage[i][j];
		}
	}
	double s = 0.0;
	for (int i = 0; i < oriH; i++)
	{
		for (int j = 0; j < oriW; j++)
		{
			s = 0.0;
			for (int m = 0; m < MSIZE; m++)
			{
				for (int n = 0; n < MSIZE; n++)
				{
					s += mask[m][n] * tempOri[i + m][j + n];
				}
			}
			tempTar[i][j] = s;
		}
	}

	//임시 타겟 -> 타겟
	for (int i = 0; i < tarH; i++)
	{
		for (int j = 0; j < tarW; j++)
		{
			double v = tempTar[i][j];
			if (v > 255)
				v = 255;
			if (v < 0)
				v = 0;
			tarImage[i][j] = v;
		}
	}

	freeDoubleArr(tempOri, oriH + 2);
	freeDoubleArr(tempTar, tarH);
}

void sharpe3()
{
	freeTarImage();
	tarH = oriH;
	tarW = oriW;
	// 결과 영상 메모리 할당
	// tarImage[tarH][tarW] 크기로 할당
	callocTarImage();

	// *** 진짜 영상처리 알고리즘 ****
	//const int MSIZE = 3; 마스크 크기 (위에 정의해둠)
	double mask[MSIZE][MSIZE] = {
		{ -1.0 / 9, -1.0 / 9, -1.0 / 9 },
		{ -1.0 / 9, 8.0 / 9, -1.0 / 9 },
		{ -1.0 / 9, -1.0 / 9, -1.0 / 9 } };

	//임시 입출력 메모리 준비
	double** tempOri, ** tempTar;
	tempOri = callocDoubleArr(oriH + 2, oriW + 2);
	tempTar = callocDoubleArr(tarH, tarW);
	for (int i = 0; i < oriH + 2; i++)
	{
		for (int j = 0; j < oriW + 2; j++)
		{
			tempOri[i][j] = 127.0;
		}
	}

	for (int i = 0; i < oriH; i++)
	{
		for (int j = 0; j < oriW; j++)
		{
			tempOri[i + 1][j + 1] = oriImage[i][j];
		}
	}
	double s = 0.0;
	for (int i = 0; i < oriH; i++)
	{
		for (int j = 0; j < oriW; j++)
		{
			s = 0.0;
			for (int m = 0; m < MSIZE; m++)
			{
				for (int n = 0; n < MSIZE; n++)
				{
					s += mask[m][n] * tempOri[i + m][j + n];
				}
			}
			tempTar[i][j] = s;
		}
	}
	// post proccessing 후처리 마스크 합계가 0이면 127을 더함.
	for (int i = 0; i < tarH; i++)
	{
		for (int j = 0; j < tarW; j++)
		{
			tempTar[i][j] += 127.0;
		}
	}
	//임시 타겟 -> 타겟
	for (int i = 0; i < tarH; i++)
	{
		for (int j = 0; j < tarW; j++)
		{
			double v = tempTar[i][j];
			if (v > 255)
				v = 255;
			if (v < 0)
				v = 0;
			tarImage[i][j] = v;
		}
	}

	freeDoubleArr(tempOri, oriH + 2);
	freeDoubleArr(tempTar, tarH);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// 경계선 검출

void edge1() {
	freeTarImage();
	// (중요!) 결과 영상의 크기를 결정 ---> 알고리즘에 의존.
	tarH = oriH;
	tarW = oriW;
	// 결과 영상 메모리 할당
	// tarImage[tarH][tarW] 크기로 할당
	callocTarImage();

	// *** 진짜 영상처리 알고리즘 ****
	// const int MSIZE = 3; // 마스크 크기
	// 중요! 마스크 준비

	double mask[MSIZE][MSIZE] = {
		{  0.0, 0.0, 0.0 },
		{ -1.0, 1.0, 0.0 },
		{  0.0, 0.0, 0.0 } };
	// 임시 입출력 메모리 준비
	double** tmpOriImage, ** tmpTarImage;
	tmpOriImage = callocDoubleArr(oriH + 2, oriW + 2);
	tmpTarImage = callocDoubleArr(tarH, tarW);
	// 임시 원본 이미지에 127 채우기 (평균값이 더 나을듯...)
	for (int i = 0; i < oriH + 2; i++)
		for (int k = 0; k < oriW + 2; k++)
			tmpOriImage[i][k] = 127.0;
	// 원본 --> 임시 원본
	for (int i = 0; i < oriH; i++)
		for (int k = 0; k < oriW; k++)
			tmpOriImage[i + 1][k + 1] = oriImage[i][k];
	// 회선 연산 --> 마스크로 긁어가면서 계산하기
	double S = 0; // 각점에 대한 마스크 연산 합계
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			S = 0.0; // 누적 초기화
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += mask[m][n] * tmpOriImage[i + m][k + n];
			tmpTarImage[i][k] = S;
		}
	}
	// Post Processing(후처리). 마스크 합계가 0이면 127정도를 더함.
	//for (int i = 0; i < tarH; i++)
	//	for (int k = 0; k < tarW; k++)
	//		tmpTarImage[i][k] += 127.0;
	// 임시 타겟 --> 타겟
	for (int i = 0; i < tarH; i++) {
		for (int k = 0; k < tarW; k++) {
			double v = tmpTarImage[i][k];
			if (v < 0.0)
				v = 0.0;
			if (v > 255.0)
				v = 255.0;
			tarImage[i][k] = v;
		}
	}

	freeDoubleArr(tmpOriImage, oriH + 2);
	freeDoubleArr(tmpTarImage, tarH);
}

void edge2() {
	freeTarImage();
	// (중요!) 결과 영상의 크기를 결정 ---> 알고리즘에 의존.
	tarH = oriH;
	tarW = oriW;
	// 결과 영상 메모리 할당
	// tarImage[tarH][tarW] 크기로 할당
	callocTarImage();

	// *** 진짜 영상처리 알고리즘 ****
	// const int MSIZE = 3; // 마스크 크기
	// 중요! 마스크 준비
  
	double mask[MSIZE][MSIZE] = {
		{  0.0, -1.0, 0.0 },
		{  0.0,  1.0, 0.0 },
		{  0.0,  0.0, 0.0 } };
	// 임시 입출력 메모리 준비
	double** tmpOriImage, ** tmpTarImage;
	tmpOriImage = callocDoubleArr(oriH + 2, oriW + 2);
	tmpTarImage = callocDoubleArr(tarH, tarW);
	// 임시 원본 이미지에 127 채우기 (평균값이 더 나을듯...)
	for (int i = 0; i < oriH + 2; i++)
		for (int k = 0; k < oriW + 2; k++)
			tmpOriImage[i][k] = 127.0;
	// 원본 --> 임시 원본
	for (int i = 0; i < oriH; i++)
		for (int k = 0; k < oriW; k++)
			tmpOriImage[i + 1][k + 1] = oriImage[i][k];
	// 회선 연산 --> 마스크로 긁어가면서 계산하기
	double S = 0; // 각점에 대한 마스크 연산 합계
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			S = 0.0; // 누적 초기화
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += mask[m][n] * tmpOriImage[i + m][k + n];
			tmpTarImage[i][k] = S;
		}
	}
	//// Post Processing(후처리). 마스크 합계가 0이면 127정도를 더함.
	//for (int i = 0; i < tarH; i++)
	//	for (int k = 0; k < tarW; k++)
	//		tmpTarImage[i][k] += 127.0;
	// 
	// 임시 타겟 --> 타겟
	for (int i = 0; i < tarH; i++) {
		for (int k = 0; k < tarW; k++) {
			double v = tmpTarImage[i][k];
			if (v < 0.0)
				v = 0.0;
			if (v > 255.0)
				v = 255.0;
			tarImage[i][k] = v;
		}
	}

	freeDoubleArr(tmpOriImage, oriH + 2);
	freeDoubleArr(tmpTarImage, tarH);
}

void edge3() {
	freeTarImage();
	// (중요!) 결과 영상의 크기를 결정 ---> 알고리즘에 의존.
	tarH = oriH;
	tarW = oriW;
	// 결과 영상 메모리 할당
	// tarImage[tarH][tarW] 크기로 할당
	callocTarImage();

	// *** 진짜 영상처리 알고리즘 ****
	// const int MSIZE = 3; // 마스크 크기
	// 중요! 마스크 준비

	// 임시 입출력 메모리 준비
	double** tmpOriImage, ** tmpTarImage;
	tmpOriImage = callocDoubleArr(oriH + 2, oriW + 2);
	tmpTarImage = callocDoubleArr(tarH, tarW);
	// 임시 원본 이미지에 127 채우기 (평균값이 더 나을듯...)
	for (int i = 0; i < oriH + 2; i++)
		for (int k = 0; k < oriW + 2; k++)
			tmpOriImage[i][k] = 127.0;

	// 원본 --> 임시 원본
	for (int i = 0; i < oriH; i++)
		for (int k = 0; k < oriW; k++)
			tmpOriImage[i + 1][k + 1] = oriImage[i][k];

	// 회선 연산 --> 마스크로 긁어가면서 계산하기
	double maxV = 0, temp = 0; // 각점에 대한 마스크 연산 합계
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			maxV = 0.0; // 누적 초기화
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
				{
					temp = oriImage[i][k] - tmpOriImage[i + m][k + n];
					if (maxV < temp)
						maxV = temp;
				}
			tmpTarImage[i][k] = maxV;
		}
	}
	// Post Processing(후처리). 마스크 합계가 0이면 127정도를 더함.
	for (int i = 0; i < tarH; i++)
		for (int k = 0; k < tarW; k++)
			tmpTarImage[i][k] += 127.0;
	 
	// 임시 타겟 --> 타겟
	for (int i = 0; i < tarH; i++) {
		for (int k = 0; k < tarW; k++) {
			double v = tmpTarImage[i][k];
			if (v < 0.0)
				v = 0.0;
			if (v > 255.0)
				v = 255.0;
			tarImage[i][k] = v;
		}
	}

	freeDoubleArr(tmpOriImage, oriH + 2);
	freeDoubleArr(tmpTarImage, tarH);
}

void edge4() {
	freeTarImage();
	// (중요!) 결과 영상의 크기를 결정 ---> 알고리즘에 의존.
	tarH = oriH;
	tarW = oriW;
	// 결과 영상 메모리 할당
	// tarImage[tarH][tarW] 크기로 할당
	callocTarImage();

	// *** 진짜 영상처리 알고리즘 ****
	// const int MSIZE = 3; // 마스크 크기
	// 중요! 마스크 준비

	// 임시 입출력 메모리 준비
	double** tmpOriImage, ** tmpTarImage;
	tmpOriImage = callocDoubleArr(oriH + 2, oriW + 2);
	tmpTarImage = callocDoubleArr(tarH, tarW);
	// 임시 원본 이미지에 127 채우기 (평균값이 더 나을듯...)
	for (int i = 0; i < oriH + 2; i++)
		for (int k = 0; k < oriW + 2; k++)
			tmpOriImage[i][k] = 127.0;

	// 원본 --> 임시 원본
	for (int i = 0; i < oriH; i++)
		for (int k = 0; k < oriW; k++)
			tmpOriImage[i + 1][k + 1] = oriImage[i][k];

	// 회선 연산 --> 마스크로 긁어가면서 계산하기
	double maxV = 0, temp = 0; // 각점에 대한 마스크 연산 합계
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			maxV = tmpOriImage[i + 1][k] - tmpOriImage[i + 1][k + 2];
			for (int m = 0; m < MSIZE; m++) {
				temp = tmpOriImage[i][k + m] - tmpOriImage[i + (MSIZE - 1)][k + (MSIZE - m - 1)];
				if (maxV < temp)
					maxV = temp;
			}
			tmpTarImage[i][k] = maxV;
		}
	}
	// Post Processing(후처리). 마스크 합계가 0이면 127정도를 더함.
	for (int i = 0; i < tarH; i++)
		for (int k = 0; k < tarW; k++)
			tmpTarImage[i][k] += 127.0;

	// 임시 타겟 --> 타겟
	for (int i = 0; i < tarH; i++) {
		for (int k = 0; k < tarW; k++) {
			double v = tmpTarImage[i][k];
			if (v < 0.0)
				v = 0.0;
			if (v > 255.0)
				v = 255.0;
			tarImage[i][k] = v;
		}
	}

	freeDoubleArr(tmpOriImage, oriH + 2);
	freeDoubleArr(tmpTarImage, tarH);
}