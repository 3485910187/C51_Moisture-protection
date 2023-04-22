#include"key.h"

unsigned char get_key()
{
    unsigned char row, col;
    // ������̰���ӳ������
    unsigned char keypad[4][4] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    };

    // �������̵���
    for(col = 0; col < 4; col++)
    {
        // ���õ�ǰ��Ϊ����͵�ƽ
        keydata = ~(0x10 << col);

        // �������̵���
        for(row = 0; row < 4; row++)
        {
            // ��⵱ǰ���Ƿ��������ź�
            if(!(keydata & (1 << row)))
            {
                // ���ض�Ӧ�İ���ֵ
                return keypad[row][col];
            }
        }
    }

    // ���û�а������£�����0xff
    return 0xff;
}