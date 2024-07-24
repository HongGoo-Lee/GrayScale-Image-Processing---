#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>

#define MSIZE 3

// �������� ����
HWND hwnd; //������ ȭ��(�ܺ�)
HDC hdc; // ������ ȭ�� (����)

int oriH, oriW, tarH=0, tarW=0;
unsigned char** oriImage = NULL;
unsigned char** tarImage = NULL;
char fileName[200];

// �Լ� �����
// ���� �Լ�
void loadImage(); void saveImage(); void displayOriImage(); void displayTarImage(); void printMenu(); void callocOriImage(); void callocTarImage(); void freeOriImage(); void freeTarImage();
double** callocDoubleArr(int h, int w); void freeDoubleArr(double** memory, int h);

// �޴� �Լ�
void bwImageMenu(); void bitImageMenu(); void parabolaImageMenu();
void zoomOutMenu(); void zoomInMenu(); void mirroringMenu();
void sharpeMenu();

// ����ó�� �Լ�
// ȭ�� �� ó�� �Լ�
void equalImage(); void addImage(); void reverseImage(); void bwImage1(); void bwImage2(); void bwImage3();
void andImage(); void orImage(); void notImage(); void xorImage(); void posterizingImage(); void gammaImage();
void rangeImage(); void parabolaImageCup(); void parabolaImageCap();

// �������� ó�� �Լ�
void zoomOut(); void zoomOut2(); void zoomOut3(); void zoomOut4(); void zoomIn(); void zoomIn2(); void zoomIn3(); void moveImage(); void rotate3(); void mirroring(); void mirroring2();

// ������׷� ó�� �Լ�
void histoStretch(); void endIn(); void histoEqual();

// ȭ�� ���� ó�� �Լ�
void emboss(); void blurr(); void sharpe1(); void sharpe2(); void sharpe3();

// ��輱 ���� �Լ�
void edge1(); void edge2(); void edge3(); void edge4();



void printMenu()
{
	system("cls");
	printf("\n GrayScale Image Proccessing (GA2)");
	printf("\n 1.���� 2.���� 9.���� ");

	printf("\n\n ȭ�� �� ó��");
	printf("\n A.�����ҷ����� B.������� C.���� D.����ȭ E.���� F.������ G.�����Ͷ���¡ H.�������� I.�Ķ󺼶�(cap,cup)");

	printf("\n\n �������� ó��");
	printf("\n J.��� K.Ȯ�� L.�̵� M.ȸ�� N.��Ī");

	printf("\n\n ������׷� ó��");
	printf("\n O.��Ʈ��ġ P.����-�� Q.��Ȱȭ");

	printf("\n\n ȭ�ҿ��� ó��");
	printf("\n R.������ S.���� T.������");

	printf("\n\n ��輱 ����");
	printf("\n U.������ V.�������� W.���� ������ X.�� ������\n");
	displayOriImage();
	displayTarImage();
}

int main(void)
{
	hwnd = GetForegroundWindow();
	hdc = GetWindowDC(hwnd); //������ 10��. ������ 11 --> (hwnd --> NULL);

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

		//case 'o': case 'O':	����(); break;
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
	puts("����� ��¥�Դϴ�.\n");

	return 0;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////
// ���� �Լ�

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
	printf("���ϸ�: "); scanf("%s", tempName); // cat01, dog05, etc11
	strcat(fullname, tempName);
	strcat(fullname, ".raw");
	strcpy(fileName, fullname);

	// ���������� ũ�⸦ �ľ��ϴ°� �߿���
	FILE* rfp = NULL;
	rfp = fopen(fileName, "rb");
	if (rfp == NULL)
	{
		printf("���ϸ� Ʋ����\n");
		return;
	}
	fseek(rfp, 0L, SEEK_END); //������ ������ �̵�
	long long fsize = ftell(rfp);
	fclose(rfp);
	
	freeOriImage();
	oriH = oriW = sqrt(fsize);

	//2���� �޸� �Ҵ�
	callocOriImage();

	//���Ͽ��� �޸𸮷�
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
	printf("���� ���ϸ�: "); scanf("%s", tempName); // out01, out02
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
	printf("%s�� �����.", fileName);
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
// �޴� �Լ�

void bwImageMenu()
{
	printf("����ȭ ���� ����\n");
	printf("1.128	2.�̹����� ��հ�	3.�̹����� ������\n");
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
		printf("�߸��� �Է��Դϴ�.\n");
		break;
	}
}

void bitImageMenu()
{
	printf("��Ʈ���� ����(�����Է�)\n");
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
		printf("�߸��� �Է��Դϴ�.\n");
		break;
	}
}

void parabolaImageMenu()
{
	printf("�Ķ󺼶� ó�� ����\n");
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
		printf("�߸��� �Է��Դϴ�.\n");
		break;
	}
}

void zoomOutMenu()
{
	printf("��� ��� ����(�����Է�)\n");
	printf("1.�����  2.������  3.��հ�  4.������\n");
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
		printf("�߸��� �Է��Դϴ�.\n");
		break;
	}
}

void zoomInMenu()
{
	printf("��� ��� ����(�����Է�)\n");
	printf("1.������  2.�����  3.�缱������\n");
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
		//printf("�̱���\n");
		zoomIn3();
		break;
	default:
		printf("�߸��� �Է��Դϴ�.\n");
		break;
	}
}

void mirroringMenu()
{
	printf("��Ī ó�� ����\n");
	printf("1.�¿��Ī	2.���ϴ�Ī\n");
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
		printf("�߸��� �Է��Դϴ�.\n");
		break;
	}
}

void sharpeMenu()
{
	printf("������ ����ũ ����(�����Է�)\n");
	printf("1.����ũ1  2.����ũ2  3.������\n");
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
		printf("�߸��� �Է��Դϴ�.\n");
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ȭ�� �� ó�� �Լ�

void equalImage()
{
	freeTarImage();
	// (�߿�!) ��� ������ ũ�⸦ �����ؾ��� --> �˰��� ���� ũ�Ⱑ �޶���
	tarH = oriH;
	tarW = oriW;
	// ��� ���� �޸� �Ҵ�
	callocTarImage();

	// ��¥ ����ó�� �˰���
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
	printf("����� ��: "); scanf("%d", &value);
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

int compare(const void* a, const void* b)    // �������� �� �Լ� ����
{
	unsigned char num1 = *(unsigned char*)a;    // void �����͸� int �����ͷ� ��ȯ�� �� �������Ͽ� ���� ������
	unsigned char num2 = *(unsigned char*)b;    // void �����͸� int �����ͷ� ��ȯ�� �� �������Ͽ� ���� ������

	if (num1 < num2)    // a�� b���� ���� ����
		return -1;      // -1 ��ȯ

	if (num1 > num2)    // a�� b���� Ŭ ����
		return 1;       // 1 ��ȯ

	return 0;    // a�� b�� ���� ���� 0 ��ȯ
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
			//�����ϴ� ���� ��ſ� ���ǹ����� ó��
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
			//�����ϴ� ���� ��ſ� ���ǹ����� ó��
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
			//�����ϴ� ���� ��ſ� ���ǹ����� ó��
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
	printf("�� �������� �����ڽ��ϱ�?");
	scanf("%d", &sector);

	int value = 0, sector_size = 0;
	value = 255 / (sector - 1); // ��
	sector_size = 255 / sector; // ������ ����
	for (int i = 0; i < tarH; i++) {
		for (int j = 0; j < tarW; j++) {
			tarImage[i][j] = (value * (oriImage[i][j] / sector_size));
		}
	}
}

void gammaImage()
{
	double gamma;
	printf("���� ���� �����ּ���: ");
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
	printf("�Է¿���: 50 150\n");
	printf("������ ����(0~255)�� ���۰� ���� �����ּ���: ");
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
// �������� ó�� �Լ�

void zoomIn()
{
	freeTarImage();
	int scale;
	printf("�� �� Ȯ���Ͻðڽ��ϱ�? ");
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
	printf("�� �� Ȯ���Ͻðڽ��ϱ�? ");
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
	printf("�� �� Ȯ���Ͻðڽ��ϱ�? ");
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
	printf("�� �� ����Ͻðڽ��ϱ�? ");
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
	printf("�� �� ����Ͻðڽ��ϱ�? ");
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
	printf("�� �� ����Ͻðڽ��ϱ�? ");
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
	printf("�� �� ����Ͻðڽ��ϱ�? ");
	scanf("%d", &scale);
	tarH = (int)(oriH / scale);
	tarW = (int)(oriW / scale);
	callocTarImage();
	int median;
	for (int i = 0; i < oriH; i += scale) {
		for (int j = 0; j < oriW; j += scale) {
			// ���� ���� �������� ���ϱ� ���� �ӽ� �迭
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
	printf("�Է¿���: 20 -30\n");
	printf("�̵��� ��: ");
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
	printf("ȸ���� ���� �Է�: ");
	scanf("%d", &angle);
	// ȸ�� ����
	// oriX = cos * (tarX-Cx) + sin * (tarY-Cy) + Cx
	// tarY = sin * (tarX-Cx) + cos * (tarY-Cy) + Cy
	int tarX, tarY, oriX, oriY;
	double radian = angle * 3.141592 / 180.0;
	double temp = (90 - angle) * 3.141592 / 180.0;

	freeTarImage();
	// (�߿�!) ��� ������ ũ�⸦ ���� ---> �˰��� ����.
	tarH = abs(oriH * cos(radian)) + abs(oriW * cos(temp));
	tarW = abs(oriH * cos(temp)) + abs(oriW * cos(radian));
	printf("%d %d\n", tarH, tarW);
	// ��� ���� �޸� �Ҵ�
	// tarImage[tarH][tarW] ũ��� �Ҵ�
	callocTarImage();
	int tarCx = tarH / 2;
	int tarCy = tarW / 2;
	int oriCx = oriH / 2;
	int oriCy = oriW / 2;
	// *** ��¥ ����ó�� �˰��� ****
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
// ������׷� �Լ�

void histoStretch() {
	// (�߿�!) ��� ������ ũ�⸦ ���� ---> �˰��� ����.
	freeTarImage();
	tarH = oriH;
	tarW = oriW;
	// ��� ���� �޸� �Ҵ�
	// tarImage[tarH][tarW] ũ��� �Ҵ�
	callocTarImage();

	// ����
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

	// *** ��¥ ����ó�� �˰��� ****
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			tarImage[i][k] = (double)(oriImage[i][k] - low) / (high - low) * 255.0;
		}
	}
}

void endIn() {
	// (�߿�!) ��� ������ ũ�⸦ ���� ---> �˰��� ����.
	freeTarImage();
	tarH = oriH;
	tarW = oriW;
	// ��� ���� �޸� �Ҵ�
	// tarImage[tarH][tarW] ũ��� �Ҵ�
	callocTarImage();

	// ����
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
	// *** ��¥ ����ó�� �˰��� ****
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
	// (�߿�!) ��� ������ ũ�⸦ ���� ---> �˰��� ����.
	freeTarImage();
	tarH = oriH;
	tarW = oriW;
	// ��� ���� �޸� �Ҵ�
	// tarImage[tarH][tarW] ũ��� �Ҵ�
	callocTarImage();

	int hist[256] = { 0 };
	int sum[256] = { 0 };
	int n[256] = { 0 };

	//1�ܰ�
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			hist[oriImage[i][k]] += 1;
		}
	}
	for (int i = 0; i < 256; i++) {
		//2�ܰ�
		sum[i] = hist[i];
		if (i != 0)
			sum[i] += sum[i - 1];

		//3�ܰ�
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
// ȭ�� ���� ó�� �Լ�

void emboss() {
	// (�߿�!) ��� ������ ũ�⸦ ���� ---> �˰��� ����.
	freeTarImage();
	tarH = oriH;
	tarW = oriW;
	// ��� ���� �޸� �Ҵ�
	// tarImage[tarH][tarW] ũ��� �Ҵ�
	callocTarImage();

	// *** ��¥ ����ó�� �˰��� ****
	//const int MSIZE = 3; ����ũ ũ�� (���� �����ص�)
	double mask[MSIZE][MSIZE] = {
		{ 1.0, 0.0, 0.0 },
		{ 0.0, 0.0, 0.0 },
		{ 0.0, 0.0, -1.0} };
	//�ӽ� ����� �޸� �غ�
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
	// post proccessing ��ó�� ����ũ �հ谡 0�̸� 127�� ����.
	for (int i = 0; i < tarH; i++)
	{
		for (int j = 0; j < tarW; j++)
		{
			tempTar[i][j] += 127.0;
		}
	}
	//�ӽ� Ÿ�� -> Ÿ��
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
	// (�߿�!) ��� ������ ũ�⸦ ���� ---> �˰��� ����.
	freeTarImage();
	tarH = oriH;
	tarW = oriW;
	// ��� ���� �޸� �Ҵ�
	// tarImage[tarH][tarW] ũ��� �Ҵ�
	callocTarImage();

	// *** ��¥ ����ó�� �˰��� ****
	//const int MSIZE = 3; ����ũ ũ�� (���� �����ص�)
	double mask[MSIZE][MSIZE] = {
		{ 1.0 / 9, 1.0 / 9, 1.0 / 9 },
		{ 1.0 / 9, 1.0 / 9, 1.0 / 9 },
		{ 1.0 / 9, 1.0 / 9, 1.0 / 9 } };
	//�ӽ� ����� �޸� �غ�
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

	//�ӽ� Ÿ�� -> Ÿ��
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
	// ��� ���� �޸� �Ҵ�
	// tarImage[tarH][tarW] ũ��� �Ҵ�
	callocTarImage();

	// *** ��¥ ����ó�� �˰��� ****
	//const int MSIZE = 3; ����ũ ũ�� (���� �����ص�)
	double mask[MSIZE][MSIZE] = {
		{ -1.0, -1.0, -1.0 },
		{ -1.0,  9.0, -1.0 },
		{ -1.0, -1.0, -1.0 } };

	//�ӽ� ����� �޸� �غ�
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

	//�ӽ� Ÿ�� -> Ÿ��
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
	// ��� ���� �޸� �Ҵ�
	// tarImage[tarH][tarW] ũ��� �Ҵ�
	callocTarImage();

	// *** ��¥ ����ó�� �˰��� ****
	//const int MSIZE = 3; ����ũ ũ�� (���� �����ص�)
	double mask[MSIZE][MSIZE] = {
		{  0.0, -1.0,  0.0 },
		{ -1.0,  5.0, -1.0  },
		{  0.0, -1.0,  0.0 } };

	//�ӽ� ����� �޸� �غ�
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

	//�ӽ� Ÿ�� -> Ÿ��
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
	// ��� ���� �޸� �Ҵ�
	// tarImage[tarH][tarW] ũ��� �Ҵ�
	callocTarImage();

	// *** ��¥ ����ó�� �˰��� ****
	//const int MSIZE = 3; ����ũ ũ�� (���� �����ص�)
	double mask[MSIZE][MSIZE] = {
		{ -1.0 / 9, -1.0 / 9, -1.0 / 9 },
		{ -1.0 / 9, 8.0 / 9, -1.0 / 9 },
		{ -1.0 / 9, -1.0 / 9, -1.0 / 9 } };

	//�ӽ� ����� �޸� �غ�
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
	// post proccessing ��ó�� ����ũ �հ谡 0�̸� 127�� ����.
	for (int i = 0; i < tarH; i++)
	{
		for (int j = 0; j < tarW; j++)
		{
			tempTar[i][j] += 127.0;
		}
	}
	//�ӽ� Ÿ�� -> Ÿ��
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
// ��輱 ����

void edge1() {
	freeTarImage();
	// (�߿�!) ��� ������ ũ�⸦ ���� ---> �˰��� ����.
	tarH = oriH;
	tarW = oriW;
	// ��� ���� �޸� �Ҵ�
	// tarImage[tarH][tarW] ũ��� �Ҵ�
	callocTarImage();

	// *** ��¥ ����ó�� �˰��� ****
	// const int MSIZE = 3; // ����ũ ũ��
	// �߿�! ����ũ �غ�

	double mask[MSIZE][MSIZE] = {
		{  0.0, 0.0, 0.0 },
		{ -1.0, 1.0, 0.0 },
		{  0.0, 0.0, 0.0 } };
	// �ӽ� ����� �޸� �غ�
	double** tmpOriImage, ** tmpTarImage;
	tmpOriImage = callocDoubleArr(oriH + 2, oriW + 2);
	tmpTarImage = callocDoubleArr(tarH, tarW);
	// �ӽ� ���� �̹����� 127 ä��� (��հ��� �� ������...)
	for (int i = 0; i < oriH + 2; i++)
		for (int k = 0; k < oriW + 2; k++)
			tmpOriImage[i][k] = 127.0;
	// ���� --> �ӽ� ����
	for (int i = 0; i < oriH; i++)
		for (int k = 0; k < oriW; k++)
			tmpOriImage[i + 1][k + 1] = oriImage[i][k];
	// ȸ�� ���� --> ����ũ�� �ܾ�鼭 ����ϱ�
	double S = 0; // ������ ���� ����ũ ���� �հ�
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			S = 0.0; // ���� �ʱ�ȭ
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += mask[m][n] * tmpOriImage[i + m][k + n];
			tmpTarImage[i][k] = S;
		}
	}
	// Post Processing(��ó��). ����ũ �հ谡 0�̸� 127������ ����.
	//for (int i = 0; i < tarH; i++)
	//	for (int k = 0; k < tarW; k++)
	//		tmpTarImage[i][k] += 127.0;
	// �ӽ� Ÿ�� --> Ÿ��
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
	// (�߿�!) ��� ������ ũ�⸦ ���� ---> �˰��� ����.
	tarH = oriH;
	tarW = oriW;
	// ��� ���� �޸� �Ҵ�
	// tarImage[tarH][tarW] ũ��� �Ҵ�
	callocTarImage();

	// *** ��¥ ����ó�� �˰��� ****
	// const int MSIZE = 3; // ����ũ ũ��
	// �߿�! ����ũ �غ�
  
	double mask[MSIZE][MSIZE] = {
		{  0.0, -1.0, 0.0 },
		{  0.0,  1.0, 0.0 },
		{  0.0,  0.0, 0.0 } };
	// �ӽ� ����� �޸� �غ�
	double** tmpOriImage, ** tmpTarImage;
	tmpOriImage = callocDoubleArr(oriH + 2, oriW + 2);
	tmpTarImage = callocDoubleArr(tarH, tarW);
	// �ӽ� ���� �̹����� 127 ä��� (��հ��� �� ������...)
	for (int i = 0; i < oriH + 2; i++)
		for (int k = 0; k < oriW + 2; k++)
			tmpOriImage[i][k] = 127.0;
	// ���� --> �ӽ� ����
	for (int i = 0; i < oriH; i++)
		for (int k = 0; k < oriW; k++)
			tmpOriImage[i + 1][k + 1] = oriImage[i][k];
	// ȸ�� ���� --> ����ũ�� �ܾ�鼭 ����ϱ�
	double S = 0; // ������ ���� ����ũ ���� �հ�
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			S = 0.0; // ���� �ʱ�ȭ
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += mask[m][n] * tmpOriImage[i + m][k + n];
			tmpTarImage[i][k] = S;
		}
	}
	//// Post Processing(��ó��). ����ũ �հ谡 0�̸� 127������ ����.
	//for (int i = 0; i < tarH; i++)
	//	for (int k = 0; k < tarW; k++)
	//		tmpTarImage[i][k] += 127.0;
	// 
	// �ӽ� Ÿ�� --> Ÿ��
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
	// (�߿�!) ��� ������ ũ�⸦ ���� ---> �˰��� ����.
	tarH = oriH;
	tarW = oriW;
	// ��� ���� �޸� �Ҵ�
	// tarImage[tarH][tarW] ũ��� �Ҵ�
	callocTarImage();

	// *** ��¥ ����ó�� �˰��� ****
	// const int MSIZE = 3; // ����ũ ũ��
	// �߿�! ����ũ �غ�

	// �ӽ� ����� �޸� �غ�
	double** tmpOriImage, ** tmpTarImage;
	tmpOriImage = callocDoubleArr(oriH + 2, oriW + 2);
	tmpTarImage = callocDoubleArr(tarH, tarW);
	// �ӽ� ���� �̹����� 127 ä��� (��հ��� �� ������...)
	for (int i = 0; i < oriH + 2; i++)
		for (int k = 0; k < oriW + 2; k++)
			tmpOriImage[i][k] = 127.0;

	// ���� --> �ӽ� ����
	for (int i = 0; i < oriH; i++)
		for (int k = 0; k < oriW; k++)
			tmpOriImage[i + 1][k + 1] = oriImage[i][k];

	// ȸ�� ���� --> ����ũ�� �ܾ�鼭 ����ϱ�
	double maxV = 0, temp = 0; // ������ ���� ����ũ ���� �հ�
	for (int i = 0; i < oriH; i++) {
		for (int k = 0; k < oriW; k++) {
			maxV = 0.0; // ���� �ʱ�ȭ
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
	// Post Processing(��ó��). ����ũ �հ谡 0�̸� 127������ ����.
	for (int i = 0; i < tarH; i++)
		for (int k = 0; k < tarW; k++)
			tmpTarImage[i][k] += 127.0;
	 
	// �ӽ� Ÿ�� --> Ÿ��
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
	// (�߿�!) ��� ������ ũ�⸦ ���� ---> �˰��� ����.
	tarH = oriH;
	tarW = oriW;
	// ��� ���� �޸� �Ҵ�
	// tarImage[tarH][tarW] ũ��� �Ҵ�
	callocTarImage();

	// *** ��¥ ����ó�� �˰��� ****
	// const int MSIZE = 3; // ����ũ ũ��
	// �߿�! ����ũ �غ�

	// �ӽ� ����� �޸� �غ�
	double** tmpOriImage, ** tmpTarImage;
	tmpOriImage = callocDoubleArr(oriH + 2, oriW + 2);
	tmpTarImage = callocDoubleArr(tarH, tarW);
	// �ӽ� ���� �̹����� 127 ä��� (��հ��� �� ������...)
	for (int i = 0; i < oriH + 2; i++)
		for (int k = 0; k < oriW + 2; k++)
			tmpOriImage[i][k] = 127.0;

	// ���� --> �ӽ� ����
	for (int i = 0; i < oriH; i++)
		for (int k = 0; k < oriW; k++)
			tmpOriImage[i + 1][k + 1] = oriImage[i][k];

	// ȸ�� ���� --> ����ũ�� �ܾ�鼭 ����ϱ�
	double maxV = 0, temp = 0; // ������ ���� ����ũ ���� �հ�
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
	// Post Processing(��ó��). ����ũ �հ谡 0�̸� 127������ ����.
	for (int i = 0; i < tarH; i++)
		for (int k = 0; k < tarW; k++)
			tmpTarImage[i][k] += 127.0;

	// �ӽ� Ÿ�� --> Ÿ��
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