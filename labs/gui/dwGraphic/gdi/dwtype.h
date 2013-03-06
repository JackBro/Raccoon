// ��ɫ���˲���
#define FILTER_NONE       (uint8_t)0x00   // �޹���
#define FILTER_ALPHA      (uint8_t)0x01   // Alpha����
#define FILTER_KEYCOLOR   (uint8_t)0x02   // �ؼ�ɫ����
#define FILTER_MULTIPLY   (uint8_t)0x03   // ��ͨ���˷�

typedef union tagHGOBJ
{
	DWORD handle;			
	VOID* lpvoid;
	tagHGOBJ()
	{
		handle = NULL;
		lpvoid = NULL;
	}
}HGTEXTURE,HGCANVAS;



typedef struct tagFILTERMODE
{
	uint8_t blue;
	uint8_t green;             
	uint8_t red;               
	uint8_t filter;   // ����ģʽ

	tagFILTERMODE()
	{
		blue = 0;
		red  = 0;
		green = 0;
		filter = FILTER_NONE;
	}
}FILTERMODE, * LPFILTERMODE;


// ����ģʽ
#define DRAWMODE_STRETCH	0x00000000
#define DRAWMODE_9GRID		0x00000001
#define DRAWMODE_TILE		0x00000002
#define DRAWMODE_CENTER		0x00000003
#define DRAWMODE_9GRIDFILL	0x00000004

typedef struct tagDRAWMODE
{
	int	dwMode;			// ����ģʽ
	int		dwParam1;		// ����1�����ھŹ���Left
	int		dwParam2;		// ����2�����ھŹ���Top
	int		dwParam3;		// ����3�����ھŹ���Right
	int		dwParam4;		// ����4�����ھŹ���Bottom

	tagDRAWMODE()
	{
		dwMode = DRAWMODE_STRETCH;
		dwParam1 = 0;
		dwParam2 = 0;
		dwParam3 = 0;
		dwParam4 = 0;
	}
}DRAWMODE, * LPDRAWMODE;





