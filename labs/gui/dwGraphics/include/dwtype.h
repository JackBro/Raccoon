#pragma once
namespace Dw
{
	union HTexture
	{
		uint32_t handle;			
		void*    lpvoid;
		HTexture()
		{
			handle = 0;
			lpvoid = NULL;
		}
	};

	enum CompositionType 	// ��ɫ��ϲ���
	{
		Composition_None    = (uint8_t)0x00,// �޻��
		Composition_Alpha,                  // Alpha���  
		Composition_KeyColor,               // �ؼ�ɫ���
		Composition_Multiply               // ��ͨ���˷�
	};

	struct CompositionMode
	{
		uint8_t blue;
		uint8_t green;             
		uint8_t red;               
		uint8_t CompositionType; // ���ģʽ
		CompositionMode()
		{
			blue   = 0;
			red    = 0;
			green  = 0;
			CompositionType = Composition_None ;
		}
	};

	enum DrawMode
	{
		DrawMode_STRETCH = 0x00000001,
		DrawMode_9GRID,		
		DrawMode_TILE,	
		DrawMode_CENTER,		
		DrawMode_9GRIDFILL,	
	};

	enum PaintType
	{
		Paint_Message = 0x00,
		NoPaint_Message,
	};
}




